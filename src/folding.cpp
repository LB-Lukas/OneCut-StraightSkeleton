//
// Created by user on 11/11/24.
//

#include "../include/StraightSkeleton/folding.h"

//              // use default "FoldType::Convex"
//            auto fold_type_map = graph.add_property_map<CGAL::SM_Halfedge_index, FoldType>("e:fold_type", FoldType::Convex).first;
//

namespace Geometry {
    Folding::Folding() : graph(PlanarGraph()) {
        this->foldtype_map = graph.add_property_map<CGAL::SM_Halfedge_index, FoldType>("e:fold-type").first;
    }


    std::vector<Point> Folding::getVertices() {
        std::vector<Point> vertices;

        for(const CGAL::SM_Vertex_index& vertex : graph.vertices()) {
            const Point& point = graph.point(vertex);
            vertices.push_back(point);
        }

        return vertices;
    }

    std::vector<std::pair<Point, Point>> Folding::getMountains() {
        return this->getEdges(FoldType::Reflex);
    }

    std::vector<std::pair<Point, Point>> Folding::getValleys() {
        return this->getEdges(FoldType::Convex);
    }

    /**
     * todo: right now just demo, connect to real folding
     * @param polygon_points : vertices of polygon
     * @return
     */
    Folding Folding::getFolding(const std::vector<Point> &polygon_points) {
        Folding folding;

        // Assuming graph is initialized properly in Folding constructor
        // auto& graph = folding.graph;

        if (polygon_points.empty()) {
            std::cerr << "Error: polygon_points is empty!" << std::endl;
            return folding;  // Early exit if no points are provided
        }

        // Add the first vertex
        auto vertex_0 = folding.graph.add_vertex(polygon_points[0]);

        auto vertex = vertex_0;

        for (int i = 1; i < polygon_points.size(); ++i) {
            auto new_vertex = folding.graph.add_vertex(polygon_points[i]);

            auto edge = folding.graph.add_edge(vertex, new_vertex);
            auto twin = folding.graph.opposite(edge);

            folding.foldtype_map[edge] = (i % 2 == 0 ? FoldType::Reflex : FoldType::Convex);
            folding.foldtype_map[twin] = (i % 2 == 0 ? FoldType::Reflex : FoldType::Convex);
            vertex = new_vertex;
        }

        // Add the closing edge from last vertex to the first vertex
        auto edge = folding.graph.add_edge(vertex, vertex_0);
        auto twin = folding.graph.opposite(edge);

        folding.foldtype_map[edge] = FoldType::Reflex;
        folding.foldtype_map[twin] = FoldType::Reflex;

        return folding;
    }


    std::vector<std::pair<Point, Point>> Folding::getEdges(FoldType foldType) {
        std::vector<std::pair<Point, Point>> edges;

        for(const auto& halfedge : graph.halfedges()) {
            // ToDo: check if inside cut graph
            if(foldtype_map[halfedge] == foldType) {
                // Get the target vertices of the half-edge and its twin (forms a complete edge)
                auto point_a = graph.point(graph.target(halfedge));
                auto point_b = graph.point(graph.target(graph.opposite(halfedge)));

                edges.push_back(std::make_pair<Point, Point>(std::move(point_a), std::move(point_b)));
            }
        }

        return edges;
    }


}