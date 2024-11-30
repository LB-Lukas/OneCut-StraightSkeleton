#include "../include/StraightSkeleton/straightSkeleton.h"
#include "../include/StraightSkeleton/folding.h"
#include <map>
#include <vector>
#include <queue>
#include <functional> // For std::greater

//todo: make header file for minheaptriangle
#include "MinHeapTriangleRay.cpp"


namespace Geometry {
    /**
     * 
     */

    //class MinHeapTriangleRay {
    //
    //};

    typedef CGAL::Surface_mesh<Point>::Property_map<CGAL::SM_Vertex_index, double> DistMap;

    // Custom comparator for a min-heap based on triangle area

    StraightSkeleton::StraightSkeleton(const std::vector<Point>& polygon_vertices) {
        DistMap distMap = graph.add_property_map<CGAL::SM_Vertex_index, double>("e:dist-map").first;
        const size_t num_vertices = polygon_vertices.size();

        /// comperator for Intersect heap
        struct IntersectMinHeapComparator {
            // stores the distance of the intersectvertices to corresponding edge
            //std::shared_ptr<DistMap> _distMap = distMap;

            bool operator()(const CGAL::SM_Vertex_index& t1,
                const CGAL::SM_Vertex_index& t2) const {
                return true;// distMap[t1] < distMap[t2];
            }
        };

        // Vector to store triangles
        //auto triangles = std::vector<Triangle>();

        // Vector to store rays with the corresponding vertex_index
        std::vector<std::pair<Line, CGAL::SM_Vertex_index>> rays;

        // Create rays
        for(int i = 0; i < num_vertices; i++) {
            const auto& lvertex = polygon_vertices[(i + num_vertices - 1) % num_vertices]; // Handle wrap-around with modulo
            const auto& rvertex = polygon_vertices[(i + 1) % num_vertices];

            // needed to compute the line of the wege
            Point mvertex = CGAL::midpoint(lvertex, rvertex); //(0.5 * (lvertex.x() + rvertex.x()), 0.5 * (lvertex.y() + rvertex.y()));

            const auto& vertex = graph.add_vertex(polygon_vertices[i]);    // vertex of polygon in graph
            // for testing purposes: graph.add_vertex(mvertex);

            // ray of bisecting angle
            Line ray = Line(polygon_vertices[i], mvertex);

            rays.emplace_back(ray, vertex);
        }

        // Create triangles and add to min-heap
        //std::priority_queue<CGAL::SM_Vertex_index, std::vector<CGAL::SM_Vertex_index>, IntersectMinHeapComparator> minHeap;
        MinHeapTriangleRay<CGAL::SM_Vertex_index> minHeapTriangleRay;

        for(int i = 0; i < num_vertices; i++) {
            const auto& lvertex = polygon_vertices[i];
            const auto& rvertex = polygon_vertices[(i + 1) % num_vertices];

            const auto& lray = rays[i];
            const auto& rray = rays[(i + 1) % num_vertices];

            // Compute intersection of rays
            const auto intersection = CGAL::intersection(lray.first, rray.first);
            const auto intersect_point = std::get_if<Point>(&*intersection);

            if(intersect_point == nullptr) {
                std::cerr << "No intersection found" << std::endl;
                continue;
            }

            const auto& intersect_vertex = graph.add_vertex(*intersect_point);

            graph.add_edge(lray.second, intersect_vertex);
            graph.add_edge(rray.second, intersect_vertex);


            // Create a triangle
            //Triangle triangle(*lvertex, *rvertex, *intersect_point);

            // Compute distance of intersect point to edge
            Line edge_line(lvertex, rvertex);
            double squared_distance = CGAL::to_double(CGAL::squared_distance(edge_line, graph.point(intersect_vertex)));
            double distance = std::sqrt(squared_distance);

            distMap[intersect_vertex] = distance;

            // Push triangle with distance to the min-heap
            minHeapTriangleRay.insert(intersect_vertex, distance);
        }

        return;

        // Process triangles in min-heap (example usage)
        while (!minHeapTriangleRay.is_empty()) {
            const CGAL::SM_Vertex_index& intersect_vertex = minHeapTriangleRay.extract_min();
            //const auto& intersection = minHeapBegin.;

            //minHeap.po(minHeap.begin());

            const auto& l_edge = graph.halfedge(intersect_vertex);
            const auto& r_edge = graph.next_around_target(l_edge);

            // todo: check if not the edge_line
            const auto& ll_edge = graph.next_around_source(l_edge);
            const auto& rr_edge = graph.next_around_source(r_edge);

            const auto& l_vertex = graph.source(graph.next_around_target(ll_edge));
            const auto& r_vertex = graph.source(graph.next_around_target(rr_edge));

            const auto& m_vertex = std::make_shared<Point>(graph.point(l_vertex).x() + graph.point(r_vertex).x(),
                graph.point(l_vertex).y() + graph.point(r_vertex).y());

            // create new edge of new vertex
            const auto& m_edge = std::make_shared<Line>(*m_vertex, graph.point(intersect_vertex));

            //minHeapTriangleRay(graph.target(ll_edge), minHeapTriangleRay.top());

            // delete old triangles
            //minHeapTriangleRay.(graph.target(ll_edge));
            //minHeapTriangleRay.delete(graph.target(rr_edge));

            //graph.remove_edge(ll_edge);
            //graph.remove_edge(rr_edge);
        }
    }
}
