#include "straight_skeleton/StraightSkeleton.h"
#include "straight_skeleton/Folding.h"
#include "straight_skeleton/event_management/SkeletonEventTriangle.h"
#include "straight_skeleton/event_management/EventHeap.h"
#include <map>
#include <vector>
#include <queue>
#include <functional> // For std::greater

#include <CGAL/version.h>
#include <iostream>
#include <straight_skeleton/event_management/SkeletonEvent.h>


namespace straight_skeleton {
    StraightSkeleton::StraightSkeleton(const std::vector<Point> &polygon_points) {
        processPolygon(polygon_points);
    }

    Line StraightSkeleton::computeRay(const Point &l_point,
                                      const Point &ll_point,
                                      const Point &r_point,
                                      const Point &rr_point) {
        // Build two lines, check for intersection
        const Line l_line(l_point, ll_point);
        const Line r_line(r_point, rr_point);

        // If lines are parallel or do not intersect
        auto result = CGAL::intersection(l_line, r_line);
        if (!result) {
            throw std::runtime_error("computeRay: No intersection found (parallel or degenerate lines).");
        }

        const Point *ipoint = std::get_if<Point>(&*result);
        if (!ipoint) {
            // In some degenerate case intersection can be a segment, handle or throw:
            throw std::runtime_error("computeRay: Intersection is not a single point (collinear overlap?).");
        }

        // Make sure vectors are not zero-length
        auto l_vect = ll_point - (*ipoint);
        auto r_vect = rr_point - (*ipoint);

        const double l_len2 = CGAL::to_double(l_vect.squared_length());
        const double r_len2 = CGAL::to_double(r_vect.squared_length());

        if (l_len2 < 1e-16 || r_len2 < 1e-16) {
            throw std::runtime_error("computeRay: Zero-length vectors in computeRay.");
        }

        // Normalize and scale
        double l_len = std::sqrt(l_len2);
        double r_len = std::sqrt(r_len2);
        l_vect = l_vect * (100.0 / l_len);
        r_vect = r_vect * (100.0 / r_len);

        // Midpoint of the two directions from intersection
        Point midpoint = CGAL::midpoint((*ipoint + l_vect), (*ipoint + r_vect));
        return Line(*ipoint, midpoint);
    }

    void StraightSkeleton::handleEdgeEvent(const std::shared_ptr<EdgeEvent>& edge_event) {
        std::cout << "handleEdgeEvent" << std::endl;

        TrianglePtr intersect_triangle = edge_event->edge_triangle;
        double min_time = edge_event->time;

        // Add the intersection vertex to the graph
        const CGAL::SM_Vertex_index intersect_vertex = graph.add_vertex(intersect_triangle->intersect_point);

        // add new edges to graph
        graph.add_edge(intersect_vertex, intersect_triangle->l_pol_vertex);
        graph.add_edge(intersect_vertex, intersect_triangle->r_pol_vertex);

        // Let’s get neighbors
        TrianglePtr l_triangle = intersect_triangle->l_triangle_ptr;
        TrianglePtr r_triangle = intersect_triangle->r_triangle_ptr;
        TrianglePtr ll_triangle = l_triangle->l_triangle_ptr;
        TrianglePtr rr_triangle = r_triangle->r_triangle_ptr;

        Line intersect_ray;
        try {
            intersect_ray = computeRay(graph.point(l_triangle->l_pol_vertex),
                                       graph.point(l_triangle->r_pol_vertex),
                                       graph.point(r_triangle->l_pol_vertex),
                                       graph.point(r_triangle->r_pol_vertex));
        } catch (const std::exception &e) {
            std::cerr << "[Warning] computeRay failed: " << e.what() << std::endl;
            // If we cannot form a valid ray, skip or break
            return;
        }

        // Next we find intersection with l_triangle_ray, r_triangle_ray
        Line l_triangle_ray(graph.point(l_triangle->l_pol_vertex), l_triangle->intersect_point);
        Line r_triangle_ray(graph.point(r_triangle->r_pol_vertex), r_triangle->intersect_point);

        auto l_intersect = CGAL::intersection(intersect_ray, l_triangle_ray);
        auto r_intersect = CGAL::intersection(intersect_ray, r_triangle_ray);

        if (!l_intersect || !r_intersect) {
            std::cerr << "[Warning] Could not intersect new ray with neighbor rays.\n";
            // Remove the old neighbors from heap anyway:
            //event_heap.remove(l_triangle);
            //minHeapTriangleRay.remove(r_triangle);
            return;
        }

        const Point *l_ip = std::get_if<Point>(&*l_intersect);
        const Point *r_ip = std::get_if<Point>(&*r_intersect);
        if (!l_ip || !r_ip) {
            std::cerr << "[Warning] One of the new intersections is not a single point.\n";
            //minHeapTriangleRay.remove(l_triangle);
            //minHeapTriangleRay.remove(r_triangle);
            return;
        }

        // Compute new distances
        double new_l_dist = min_time + std::sqrt(CGAL::to_double(
                                CGAL::squared_distance(*l_ip, intersect_triangle->intersect_point)));
        double new_r_dist = min_time + std::sqrt(CGAL::to_double(
                                CGAL::squared_distance(*r_ip, intersect_triangle->intersect_point)));

        // Create two new triangles
        std::shared_ptr<SkeletonEventTriangle> new_l_triangle = std::make_shared<SkeletonEventTriangle>();
        new_l_triangle->intersect_point = *l_ip;
        new_l_triangle->l_pol_vertex = l_triangle->l_pol_vertex;
        new_l_triangle->r_pol_vertex = intersect_vertex;
        new_l_triangle->l_triangle_ptr = ll_triangle;
        new_l_triangle->r_triangle_ptr = nullptr; // link later

        const EdgeEventPtr new_l_edge_event = std::make_shared<EdgeEvent>(new_l_dist, *l_ip, new_l_triangle);

        std::shared_ptr<SkeletonEventTriangle> new_r_triangle = std::make_shared<SkeletonEventTriangle>();
        new_r_triangle->intersect_point = *r_ip;
        new_r_triangle->r_pol_vertex = r_triangle->r_pol_vertex;
        new_r_triangle->l_pol_vertex = intersect_vertex;
        new_r_triangle->r_triangle_ptr = rr_triangle;
        new_r_triangle->l_triangle_ptr = nullptr; // link later

        const EdgeEventPtr new_r_edge_event = std::make_shared<EdgeEvent>(new_r_dist, *r_ip, new_r_triangle);

        // link new triangles
        new_l_triangle->r_triangle_ptr = new_r_triangle;
        new_r_triangle->l_triangle_ptr = new_l_triangle;

        // fix outer links
        ll_triangle->r_triangle_ptr = new_l_triangle;
        rr_triangle->l_triangle_ptr = new_r_triangle;

        // Insert new triangles into the min-heap
        event_heap.insert(new_l_edge_event, new_l_dist);
        event_heap.insert(new_r_edge_event, new_r_dist);

        // Remove the old neighbor triangles from the heap
        try {
            event_heap.remove(l_triangle->skeleton_event_ptr);
        } catch (const std::exception &e) {
            std::cerr << e.what() << std::endl;
        }

        try {
            event_heap.remove(r_triangle->skeleton_event_ptr);
        } catch (const std::exception &e) {
            std::cerr << e.what() << std::endl;
        }

        try {
            new_l_triangle->assertValide();
        } catch (const std::exception &e) {
            std::cerr << e.what() << std::endl;
        }

        try {
            new_r_triangle->assertValide();
        } catch (const std::exception &e) {
            std::cerr << e.what() << std::endl;
        }

        // We are done for this iteration. Move on.
    }

    void StraightSkeleton::handleSplitEvent(std::shared_ptr<SplitEvent> split_event) {

    }

    void StraightSkeleton::handleVertexEvent(std::shared_ptr<VertexEvent> vertex_event) {

    }

    bool StraightSkeleton::testPolygonConnection() {
        // Example placeholder
        return false;
    }

    void StraightSkeleton::processRayCalculation() {

    }

    void StraightSkeleton::processPolygon(const std::vector<Point> &polygon_points) {
        const size_t num_vertices = polygon_points.size();
        if (num_vertices < 3) {
            throw std::runtime_error("Cannot build skeleton for fewer than 3 vertices.");
        }

        // Store lines (rays) out of each polygon vertex
        std::vector<Line> rays;
        rays.reserve(num_vertices);

        // Store the index of each new vertex in the CGAL mesh
        std::vector<CGAL::SM_Vertex_index> vertices;
        vertices.reserve(num_vertices);

        // We store triangles in a vector, plus we’ll keep them in the min-heap
        // keyed by the distance from polygon edge to intersection.
        std::vector<std::shared_ptr<SkeletonEventTriangle> > triangles;
        triangles.reserve(num_vertices);

        // Build rays from each vertex
        for (int i = 0; i < static_cast<int>(num_vertices); ++i) {
            const Point &current_point = polygon_points[i];
            const Point& l_point = polygon_points[(i - 1 + num_vertices) % num_vertices];  // polygon-vertex to the left
            const Point& r_point = polygon_points[(i + 1) % num_vertices];                 // polygon-vertex to the right

            auto l_vect = l_point - current_point;      // difference-vector l_point and current_point
            auto r_vect = r_point - current_point;      // difference-vector r_point and current_point

            double l_sqrd_length = CGAL::to_double(l_vect.squared_length());   // squared length of l-vector
            double r_sqrd_length = CGAL::to_double(r_vect.squared_length());   // squared length of r-vector
            if (l_sqrd_length < 1e-16 || r_sqrd_length < 1e-16) {
                throw std::runtime_error("processPolygon: polygon has adjacent identical points or zero-length edges.");
            }

            double l_length = std::sqrt(l_sqrd_length);     // length of l-vector
            double r_length = std::sqrt(r_sqrd_length);     // length of r-vector

            // scaling to 100 more prune for numeric errors
            l_vect = l_vect * (100.0 / l_length);         // scale l-vect to length 100
            r_vect = r_vect * (100.0 / r_length);         // scale r-vect to length 100

            Point mid_point = current_point + l_vect + r_vect;

            // Shift l_point, r_point outward from 'point'
            //l_point = current_point + l_vect;
            //r_point = current_point + r_vect;

            // Midpoint for the direction
            //Point m_point = CGAL::midpoint(l_point, r_point);
            vertices.push_back(graph.add_vertex(current_point));

            rays.emplace_back(Line(current_point, mid_point));
        }

        // Build a min-heap for Triangles with distances
        //EventHeap<std::shared_ptr<SkeletonEventTriangle> > minHeapTriangleRay;
        EventHeap<std::shared_ptr<SkeletonEvent>> event_heap;

        // For each edge of the polygon (i.e. from i to i+1),
        // compute intersection of rays i and (i+1).
        for (int i = 0; i < static_cast<int>(num_vertices); ++i) {
            const Point &l_point = polygon_points[i];
            const Point &r_point = polygon_points[(i + 1) % num_vertices];

            const auto &l_ray = rays[i];
            const auto &r_ray = rays[(i + 1) % num_vertices];

            auto result = CGAL::intersection(l_ray, r_ray);
            if (!result) {
                std::cerr << "[Warning] processPolygon: No intersection found between rays i and i+1\n";
                continue;
            }

            const Point *intersect_point = std::get_if<Point>(&*result);
            if (!intersect_point) {
                std::cerr << "[Warning] processPolygon: Intersection is not a single point (possible segment)\n";
                continue;
            }

            std::shared_ptr<SkeletonEventTriangle> triangle = std::make_shared<SkeletonEventTriangle>();
            triangle->intersect_point = *intersect_point;
            triangle->l_pol_vertex = vertices[i];
            triangle->r_pol_vertex = vertices[(i + 1) % num_vertices];

            triangles.push_back(triangle);

            // Cut-edge line of current triangle
            auto cut_edge = Line(graph.point(triangle->l_pol_vertex), graph.point(triangle->r_pol_vertex));

            // Distance from intersection to the original cut-edge
            double sqrd_dist = CGAL::to_double(CGAL::squared_distance(*intersect_point, cut_edge));
            double time = std::sqrt(sqrd_dist);

            // create event and link edge-event and edge-event-triangle
            std::shared_ptr<EdgeEvent> edge_event = std::make_shared<EdgeEvent>(time, *intersect_point, triangle);
            triangle->skeleton_event_ptr = edge_event;
            event_heap.insert(edge_event, time);
        }

        // Link triangles in a ring
        const int triangle_count = static_cast<int>(num_vertices);
        for (int i = 0; i < triangle_count; i++) {
             auto& triangle = triangles[i];
            const auto& left_triangle = triangles[(i - 1 + triangle_count) % triangle_count];
            const auto& right_triangle = triangles[(i + 1) % triangle_count];
            triangle->l_triangle_ptr = left_triangle;
            triangle->r_triangle_ptr = right_triangle;
        }

        // Example of processing triangles from the min-heap
        std::cout << "size: " << event_heap.get_size() << std::endl;

        int iterationCount = 0;
        while (event_heap.get_size() > 2 && iterationCount++ < 50) {
            event_heap.print_heap();

            // Extract the triangle with minimum distance
            double min_dist = event_heap.get_min_weight();
            std::shared_ptr<SkeletonEvent> next_event = event_heap.extract_min();

            if (auto edge_event = std::dynamic_pointer_cast<EdgeEvent>(next_event)) {
                // Handle EdgeEvent
                handleEdgeEvent(edge_event);
            }
            else if (auto split_event = std::dynamic_pointer_cast<SplitEvent>(next_event)) {
                // Handle SplitEvent
                handleSplitEvent(split_event);
            }
            else if (auto vertex_event = std::dynamic_pointer_cast<VertexEvent>(next_event)) {
                // Handle VertexEvent
                handleVertexEvent(vertex_event);
            }
            else {
                // Handle unknown event type or throw an exception
                std::cerr << "Unknown event type encountered.\n";
            }

            /*

            std::shared_ptr<SkeletonEventTriangle> intersect_triangle = event_heap.extract_min();

            // (Potential extension: if multiple triangles share the same min_dist,
            // you could gather them in a loop.)

            // Add the intersection vertex to the graph
            const CGAL::SM_Vertex_index intersect_vertex =
                    graph.add_vertex(intersect_triangle->intersect_point);
            graph.add_edge(intersect_vertex, intersect_triangle->l_pol_vertex);
            graph.add_edge(intersect_vertex, intersect_triangle->r_pol_vertex);

            // Let’s get neighbors
            std::shared_ptr<SkeletonEventTriangle> l_triangle = intersect_triangle->l_triangle_ptr;
            std::shared_ptr<SkeletonEventTriangle> r_triangle = intersect_triangle->r_triangle_ptr;
            std::shared_ptr<SkeletonEventTriangle> ll_triangle = l_triangle->l_triangle_ptr;
            std::shared_ptr<SkeletonEventTriangle> rr_triangle = r_triangle->r_triangle_ptr;

            // Build a new “intersection_ray” from geometry
            // We wrap in try/catch in case lines are parallel or invalid
            Line intersect_ray;
            try {
                intersect_ray = computeRay(graph.point(l_triangle->l_pol_vertex),
                                           graph.point(l_triangle->r_pol_vertex),
                                           graph.point(r_triangle->l_pol_vertex),
                                           graph.point(r_triangle->r_pol_vertex));
            } catch (const std::exception &e) {
                std::cerr << "[Warning] computeRay failed: " << e.what() << std::endl;
                // If we cannot form a valid ray, skip or break
                continue;
            }

            // Next we find intersection with l_triangle_ray, r_triangle_ray
            Line l_triangle_ray(graph.point(l_triangle->l_pol_vertex), l_triangle->intersect_point);
            Line r_triangle_ray(graph.point(r_triangle->r_pol_vertex), r_triangle->intersect_point);

            auto l_tr_int = CGAL::intersection(intersect_ray, l_triangle_ray);
            auto r_tr_int = CGAL::intersection(intersect_ray, r_triangle_ray);

            if (!l_tr_int || !r_tr_int) {
                std::cerr << "[Warning] Could not intersect new ray with neighbor rays.\n";
                // Remove the old neighbors from heap anyway:
                minHeapTriangleRay.remove(l_triangle);
                minHeapTriangleRay.remove(r_triangle);
                continue;
            }

            const Point *l_ip = std::get_if<Point>(&*l_tr_int);
            const Point *r_ip = std::get_if<Point>(&*r_tr_int);
            if (!l_ip || !r_ip) {
                std::cerr << "[Warning] One of the new intersections is not a single point.\n";
                minHeapTriangleRay.remove(l_triangle);
                minHeapTriangleRay.remove(r_triangle);
                continue;
            }

            // Compute new distances
            double new_l_dist = min_dist + std::sqrt(CGAL::to_double(
                                    CGAL::squared_distance(*l_ip, intersect_triangle->intersect_point)));
            double new_r_dist = min_dist + std::sqrt(CGAL::to_double(
                                    CGAL::squared_distance(*r_ip, intersect_triangle->intersect_point)));

            // Create two new triangles
            std::shared_ptr<SkeletonEventTriangle> new_l_triangle = std::make_shared<SkeletonEventTriangle>();
            new_l_triangle->intersect_point = *l_ip;
            new_l_triangle->l_pol_vertex = l_triangle->l_pol_vertex;
            new_l_triangle->r_pol_vertex = intersect_vertex;
            new_l_triangle->l_triangle_ptr = ll_triangle;
            new_l_triangle->r_triangle_ptr = nullptr; // link later

            std::shared_ptr<SkeletonEventTriangle> new_r_triangle = std::make_shared<SkeletonEventTriangle>();
            new_r_triangle->intersect_point = *r_ip;
            new_r_triangle->r_pol_vertex = r_triangle->r_pol_vertex;
            new_r_triangle->l_pol_vertex = intersect_vertex;
            new_r_triangle->r_triangle_ptr = rr_triangle;
            new_r_triangle->l_triangle_ptr = nullptr; // link later

            // link new triangles
            new_l_triangle->r_triangle_ptr = new_r_triangle;
            new_r_triangle->l_triangle_ptr = new_l_triangle;

            // fix outer links
            ll_triangle->r_triangle_ptr = new_l_triangle;
            rr_triangle->l_triangle_ptr = new_r_triangle;

            // Insert new triangles into the min-heap
            minHeapTriangleRay.insert(new_l_triangle, new_l_dist);
            minHeapTriangleRay.insert(new_r_triangle, new_r_dist);

            // Remove the old neighbor triangles from the heap
            minHeapTriangleRay.remove(l_triangle);
            minHeapTriangleRay.remove(r_triangle);

            // (Optional) check validity
            new_l_triangle->assertValide();
            new_r_triangle->assertValide();

            // We are done for this iteration. Move on.
            */
        }
    }
} // namespace Geometry
