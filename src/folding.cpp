//
// Created by user on 11/11/24.
//

#include "../include/StraightSkeleton/folding.h"

//              // use default "FoldType::Convex"
//            auto fold_type_map = graph.add_property_map<CGAL::SM_Halfedge_index, FoldType>("e:fold_type", FoldType::Convex).first;
//

namespace Geometry {
    Folding::Folding() : graph(PlanarGraph()) {
        this->foldtype_map = graph.add_property_map<CGAL::SM_Edge_index, FoldType>("e:fold-type").first;
    }


    std::vector<Point> Folding::getVertices() {
        std::vector<Point> vertices;

        for(const auto& vertex : graph.vertices()) {
            const auto point = graph.point(vertex);
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

    Folding Folding::getFolding(const std::vector<Point> &polygon_points) {
        Folding folding;

        auto& graph = folding.graph;

        auto vertex_0 = graph.add_vertex(polygon_points[0]);
        auto vertex = vertex_0;

        for(int i = 1; i < polygon_points.size(); i++) {
            auto new_vertex = graph.add_vertex(polygon_points[i]);
            auto edge = graph.add_edge(vertex, new_vertex);
            folding.foldtype_map[graph.edge(edge)] = FoldType::Reflex;
            vertex = new_vertex;
        }

        auto edge = graph.edge(graph.add_edge(vertex, vertex_0));
        folding.foldtype_map[edge] = FoldType::Convex;

        return folding;
    }

    std::vector<std::pair<Point, Point>> Folding::getEdges(FoldType foldType) {
        std::vector<std::pair<Point, Point>> edges;

        for(const auto& edge : graph.edges()) {
            const auto fold_map = graph.property_map<CGAL::SM_Edge_index, FoldType>("e:fold_type").value();
            // ToDo: check if inside cut graph
            if(fold_map[edge] != foldType) continue;

            // Get the corresponding half-edge for this edge
            const auto halfedge = graph.halfedge(edge);

            // Get the target vertices of the half-edge and its twin (forms a complete edge)
            auto& point_a = graph.point(graph.target(halfedge));
            auto& point_b = graph.point(graph.target(graph.opposite(halfedge)));


            edges.push_back(std::make_pair<Point, Point>(std::move(point_a), std::move(point_b)));
        }

        return edges;
    }


}