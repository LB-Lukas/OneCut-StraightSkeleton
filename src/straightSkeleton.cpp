#include "../include/StraightSkeleton/straightSkeleton.h"
#include "../include/StraightSkeleton/folding.h"
#include <map>
#include <vector>
#include <queue>
#include <functional> // For std::greater

#include <CGAL/version.h>
#include <iostream>

//todo: make header file for minheaptriangle
#include "MinHeapTriangleRay.cpp"
#include "Triangle.cpp"


namespace Geometry {
    typedef CGAL::Surface_mesh<Point>::Property_map<CGAL::SM_Vertex_index, double> DistMap;
    typedef CGAL::Surface_mesh<Point>::Property_map<CGAL::SM_Edge_index, Triangle> TriangleMap;

    // Custom comparator for a min-heap based on triangle area

    Line StraightSkeleton::computeRay(const Point& l_point, const Point& ll_point, const Point& r_point, const Point& rr_point) {
        const Line l_line = Line(l_point, ll_point);
        const Line r_line = Line(r_point, rr_point);

        const auto intersection = CGAL::intersection(l_line, r_line);

        if (!intersection) {
            std::cerr << "No intersection found between rays" << std::endl;
            return l_line;
        }

        const Point* intersect_point = std::get_if<Point>(&*intersection);

        auto l_vect = ll_point - *intersect_point;
        auto r_vect = rr_point - *intersect_point;

        l_vect *= 100 / std::sqrt(CGAL::to_double(l_vect.squared_length()));
        r_vect *= 100 / std::sqrt(CGAL::to_double(r_vect.squared_length()));

        const Point m_point = CGAL::midpoint(*intersect_point + l_vect, *intersect_point + r_vect);

        const Line ray = Line(*intersect_point, m_point);

        return ray;
    }

    void handleVertexEvent() {

    }

    void handleEdgeEvent() {

    }

    bool testPolygonConnection() {
        //Triangle max = triay
        return false;
    }

    StraightSkeleton::StraightSkeleton(const std::vector<Point>& polygon_points) {
        //TriangleMap triangleMap = graph.add_property_map<CGAL::SM_Edge_index, Triangle>("e:triangle-map").first;
        const size_t num_vertices = polygon_points.size();

        std::cout << CGAL_VERSION_MAJOR << "." << CGAL_VERSION_MINOR << std::endl;

        // Vector to store rays with the corresponding vertex_index
        std::vector<Line> rays;
        std::vector<CGAL::SM_Vertex_index> vertices;
        std::vector<std::shared_ptr<Triangle>> triangles;

        std::unordered_map<Triangle, size_t> index_map;

        // Create rays
        for (int i = 0; i < num_vertices; i++) {
            const auto& point = polygon_points[i];
            auto l_point = polygon_points[(i + num_vertices - 1) % num_vertices]; // Wrap-around
            auto r_point = polygon_points[(i + 1) % num_vertices];

            auto l_vect = l_point - point;
            auto r_vect = r_point - point;

            // Normalize vectors and adjust their length
            l_vect = l_vect * (100.0 / std::sqrt(CGAL::to_double(l_vect.squared_length())));
            r_vect = r_vect * (100.0 / std::sqrt(CGAL::to_double(r_vect.squared_length())));

            l_point = point + l_vect;
            r_point = point + r_vect;

            // Compute midpoint
            Point m_point = CGAL::midpoint(l_point, r_point);

            const CGAL::SM_Vertex_index vertex = graph.add_vertex(point);
            vertices.push_back(vertex);

            Line ray = Line(point, m_point);
            rays.push_back(ray);
        }

        MinHeapTriangleRay<std::shared_ptr<Triangle>> minHeapTriangleRay;

        for (int i = 0; i < num_vertices; i++) {
            const auto& l_point = polygon_points[i];
            const auto& r_point = polygon_points[(i + 1) % num_vertices];

            const auto& l_ray = rays[i];
            const auto& r_ray = rays[(i + 1) % num_vertices];

            const auto intersection = CGAL::intersection(l_ray, r_ray);

            if (!intersection) {
                std::cerr << "No intersection found between rays" << std::endl;
                continue;
            }

            const Point* intersect_point = std::get_if<Point>(&*intersection);
            if (!intersect_point) {
                std::cerr << "Intersection is not a point" << std::endl;
                continue;
            }

            std::shared_ptr<Triangle>  triangle = std::make_shared<Triangle>();
            triangle->intersect_point = *intersect_point;
            triangle->l_pol_vertex = vertices[i];
            triangle->r_pol_vertex = vertices[(i + 1) % num_vertices];

            triangles.push_back(triangle);


            //const CGAL::SM_Edge_index edge = graph.edge(graph.add_edge(vertices[i], vertices[(i + 1) % num_vertices]));
            //triangleMap[edge] = triangle;
            //triangle.corr_edge = edge;


            Line edge_line(l_point, r_point);
            double squared_distance = CGAL::to_double(CGAL::squared_length(l_point - *intersect_point));
            double distance = std::sqrt(squared_distance);

            minHeapTriangleRay.insert(triangle, distance);
        }

        // link triangles to each other
        for(int i = 0; i < triangles.size(); i++) {
            const std::shared_ptr<Triangle> triangle = triangles[i];

            const std::shared_ptr<Triangle> l_triangle = triangles[(i - 1 + triangles.size()) % triangles.size()];
            const std::shared_ptr<Triangle> r_triangle = triangles[(i + 1) % triangles.size()];

            triangle->l_triangle_ptr = l_triangle;
            triangle->r_triangle_ptr = r_triangle;
        }


        //auto test = minHeapTriangleRay.get_min();
        //auto test2 = graph.add_vertex(Point(100, 500));
        //graph.add_edge(test, test2);

        //return;

        // Process triangles in min-heap (example usage)

        std::cout << "size: " <<  minHeapTriangleRay.get_size() << std::endl;

        int j = 0;

        if(!graph.is_valid()) {
            std::cerr << "Invalid graph" << std::endl;
        } else {
            std::cout << "Valid graph" << std::endl;
        }

        //for(auto triangle: triangles) {
        //  minHeapTriangleRay.remove(triangle);
        //}

        //return;

        std::cout << minHeapTriangleRay.get_size() << std::endl;


        while (minHeapTriangleRay.get_size() > 2 && j++ < 2) {
            // get intersect triangle and minimal distance
            const double min_dist = minHeapTriangleRay.get_min_weight();
            const std::shared_ptr<Triangle> intersect_triangle = minHeapTriangleRay.extract_min();

            std::vector<std::shared_ptr<Triangle>> intersections;

            while(minHeapTriangleRay.get_size() > 2 && minHeapTriangleRay.get_min_weight() == min_dist) {
                intersections.push_back(intersect_triangle);
            }

            const CGAL::SM_Vertex_index intersect_vertex = graph.add_vertex(intersect_triangle->intersect_point);
            graph.add_edge(intersect_vertex, intersect_triangle->l_pol_vertex);
            graph.add_edge(intersect_vertex, intersect_triangle->r_pol_vertex);

            const std::shared_ptr<Triangle> l_triangle = intersect_triangle->l_triangle_ptr;
            const std::shared_ptr<Triangle> r_triangle = intersect_triangle->r_triangle_ptr;

            const std::shared_ptr<Triangle> ll_triangle = l_triangle->l_triangle_ptr;
            const std::shared_ptr<Triangle> rr_triangle = r_triangle->r_triangle_ptr;

            // calculate new ray leaving intersect_vertex

            const Line intersect_ray = computeRay(graph.point(l_triangle->l_pol_vertex),
                graph.point(l_triangle->r_pol_vertex),
                graph.point(r_triangle->l_pol_vertex),
                graph.point(r_triangle->r_pol_vertex));

            const Line l_triangle_ray = Line(graph.point(l_triangle->l_pol_vertex), l_triangle->intersect_point);
            const Line r_triangle_ray = Line(graph.point(r_triangle->r_pol_vertex), r_triangle->intersect_point);

            const auto l_triangle_intersection = CGAL::intersection(intersect_ray, l_triangle_ray);
            const auto r_triangle_intersection = CGAL::intersection(intersect_ray, r_triangle_ray);

            if (!l_triangle_intersection || !r_triangle_intersection) {
                std::cerr << "No intersection found between rays" << std::endl;
                return;
            }

            const Point* l_triangle_intersect_point = std::get_if<Point>(&*l_triangle_intersection);
            const Point* r_triangle_intersect_point = std::get_if<Point>(&*r_triangle_intersection);

            const double l_dist = min_dist + std::sqrt(CGAL::to_double(
                CGAL::squared_length(*l_triangle_intersect_point - intersect_triangle->intersect_point)));
            const double r_dist = min_dist +  std::sqrt(CGAL::to_double(
                CGAL::squared_length(*r_triangle_intersect_point - intersect_triangle->intersect_point)));

            const std::shared_ptr<Triangle> new_l_triangle = std::make_shared<Triangle>();
            const std::shared_ptr<Triangle> new_r_triangle = std::make_shared<Triangle>();

            new_l_triangle->intersect_point = *l_triangle_intersect_point;
            new_l_triangle->l_pol_vertex = l_triangle->l_pol_vertex;
            new_l_triangle->r_pol_vertex = intersect_vertex;
            new_l_triangle->l_triangle_ptr = ll_triangle;
            new_l_triangle->r_triangle_ptr = new_r_triangle;

            new_r_triangle->intersect_point = *r_triangle_intersect_point;
            new_r_triangle->r_pol_vertex = r_triangle->r_pol_vertex;
            new_r_triangle->l_pol_vertex = intersect_vertex;
            new_r_triangle->r_triangle_ptr = rr_triangle;
            new_r_triangle->l_triangle_ptr = new_l_triangle;

            ll_triangle->r_triangle_ptr = new_l_triangle;
            rr_triangle->l_triangle_ptr = new_r_triangle;

            minHeapTriangleRay.insert(new_l_triangle, l_dist);

            std::cout << "size1: " << minHeapTriangleRay.get_size() << std::endl;
            minHeapTriangleRay.insert(new_r_triangle, r_dist);

            std::cout << "size2: " << minHeapTriangleRay.get_size() << std::endl;

            minHeapTriangleRay.remove(l_triangle);
            minHeapTriangleRay.remove(r_triangle);

            // destruct triangles
            //l_triangle->~Triangle();
            //r_triangle->~Triangle();
            //ntersect_triangle->~Triangle();

            std::cout << "size3: " << minHeapTriangleRay.get_size() << std::endl;

            continue;
            return;

            /*
            if (l_edge == CGAL::SM_Halfedge_index()) {
                std::cerr << "Error: No halfedge for intersect_vertex" << std::endl;
                return;
            }

            //const CGAL::SM_Halfedge_index  r_edge = graph.next_around_target(l_edge);

            if (r_edge == CGAL::SM_Halfedge_index()) {
                std::cerr << "Error: Invalid r_edge (no next around target)" << std::endl;
                return;
            }

            // todo: check if not the edge_line
            const CGAL::SM_Halfedge_index ll_edge = graph.next_around_source(l_edge);
            const CGAL::SM_Halfedge_index rr_edge = graph.next_around_source(r_edge);

            const CGAL::SM_Vertex_index l_vertex = graph.source(graph.next_around_target(ll_edge));
            const CGAL::SM_Vertex_index r_vertex = graph.source(graph.next_around_target(rr_edge));

           // std::cout << graph.point(l_vertex) << " " << std::endl;


            //const auto& m_vertex = CGAL::midpoint(graph.point(l_vertex), graph.point(r_vertex));

            // create new edge of new vertex
            //const auto& m_edge = std::make_shared<Line>(m_vertex, graph.point(intersect_vertex));

            //graph.add_vertex(m_vertex);

            //graph.add_edge(test2, l_vertex);
            //graph.add_edge(test2, r_vertex);

            //minHeapTriangleRay.insert(graph.target(ll_edge), minHeapTriangleRay.top());

            return;

            // delete old triangles
            //minHeapTriangleRay.remove(ll_edge));
            //minHeapTriangleRay.remove(graph.target(rr_edge));

            graph.remove_edge(graph.edge(ll_edge));
            graph.remove_edge(graph.edge(rr_edge));
            */
        }
    }
}
