#include "../include/StraightSkeleton/straightSkeleton.h"
#include "../include/StraightSkeleton/folding.h"
#include <map>
#include <vector>
#include <functional> // For std::greater


namespace Geometry {

    // Custom comparator for a min-heap based on triangle area
    struct IntersectMinHeapComparator {
        bool operator()(const std::pair<CGAL::SM_Vertex_index, double>& t1,
            const std::pair<CGAL::SM_Vertex_index, double>& t2) const {
            return t1.second < t2.second;
        }
    };

    StraightSkeleton::StraightSkeleton(const std::vector<std::shared_ptr<Point>> &vertices) {
        PlanarGraph graph;

        // Vector to store triangles
        auto triangles = std::vector<Triangle>();

        // Vector to store rays
        std::vector<std::pair<std::shared_ptr<Line>, CGAL::SM_Vertex_index>> rays;

        // Create rays
        for(int i = 0; i < vertices.size(); i++) {
            const auto& lvertex = vertices[(i - 1 + vertices.size()) % vertices.size()]; // Handle wrap-around with modulo
            const auto& rvertex = vertices[(i + 1) % vertices.size()];

            Point mvertex(lvertex->x() + rvertex->x(), lvertex->y() + rvertex->y());

            const auto& vertex = graph.add_vertex(*vertices[i]);    // vertex of polygon in graph

            auto ray = std::make_shared<Line>(*vertices[i], mvertex);

            rays.push_back(std::make_pair(ray, vertex));
        }

        // Create triangles and add to min-heap
        std::map<CGAL::SM_Vertex_index,
                double,
                IntersectMinHeapComparator> minHeap;

        for(int i = 0; i < vertices.size(); i++) {
            const auto& lvertex = vertices[i];
            const auto& rvertex = vertices[(i + 1) % vertices.size()];

            const auto& ray1 = rays[i];
            const auto& ray2 = rays[(i + 1) % vertices.size()];
            
            // Compute intersection of rays
            auto intersection = CGAL::intersection(*ray1.first, *ray2.first);
            auto intersect_point = std::get_if<Point>(&*intersection);

            const auto& intersect_vertex = graph.add_vertex(*intersect_point);
            graph.add_edge(ray1.second, intersect_vertex);
            graph.add_edge(ray2.second, intersect_vertex);



            if(intersect_point == nullptr) continue; // Skip if no intersection

            // Create a triangle
            Triangle triangle(*lvertex, *rvertex, *intersect_point);

            // Compute distance of intersect point to edge
            Line edge_line(*lvertex, *rvertex);
            double squared_distance = CGAL::to_double(CGAL::squared_distance(Line(*lvertex, *rvertex), *rvertex));
            double distance = std::sqrt(squared_distance);

            // Push triangle with distance to the min-heap
            minHeap[intersect_vertex] = distance;
        }

        // Optional: Process triangles in min-heap (example usage)
        while (!minHeap.empty()) {
            const CGAL::SM_Vertex_index& intersect_vertex = minHeap.begin()->first;
            const auto& intersection = minHeap.begin()->second;

            minHeap.erase(minHeap.begin());

            const auto l_edge = graph.halfedge(intersect_vertex);
            const auto& r_edge = graph.next_around_target(l_edge);

            const auto ll_edge = graph.next_around_source(l_edge);
            const auto& rr_edge = graph.next_around_source(r_edge);

            const auto& l_vertex = graph.source(graph.next_around_target(ll_edge));
            const auto& r_vertex = graph.source(graph.next_around_target(rr_edge));

            const auto& m_vertex = std::make_shared<Point>(graph.point(l_vertex).x() + graph.point(r_vertex).x(),
                graph.point(l_vertex).y() + graph.point(r_vertex).y());

            // create new edge of new vertex
            const auto& m_edge = std::make_shared<Line>(*m_vertex, graph.point(intersect_vertex));

            // delete old triangles
            minHeap.erase(graph.target(ll_edge));
            minHeap.erase(graph.target(rr_edge));

            // Mark edges as mountain or valley folds
            //fold_type_map[l_edge] = FoldType::Convex;
            //fold_type_map[e2] = FoldType::Valley;
            //fold_type_map[e3] = FoldType::Mountain;

            //graph.remove_edge(ll_edge);
            //graph.remove_edge(rr_edge);
        }
    }
}
