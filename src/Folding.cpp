//
// Created by user on 11/11/24.
//

#include "straight_skeleton/Folding.h"
#include "straight_skeleton/StraightSkeleton.h"
#include "straight_skeleton/event_management/SkeletonEvent.h"

//              // use default "FoldType::Convex"
//            auto fold_type_map = graph.add_property_map<CGAL::SM_Halfedge_index, FoldType>("e:fold_type", FoldType::Convex).first;
//

namespace straight_skeleton {
    Folding::Folding() : graph(PlanarGraph()) {
        this->foldtype_map = graph.add_property_map<CGAL::SM_Halfedge_index, FoldType>("e:fold-type").first;
    }

    Folding::Folding(const PlanarGraph& graph) : graph(graph) {
        this->foldtype_map = this->graph.add_property_map<CGAL::SM_Halfedge_index, FoldType>("e:fold-type").first;
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
        auto skeleton = StraightSkeleton(polygon_points);
        // TODO: Add perpendiculars to the skeleton
        // Planarer Graph mit den Skeleton Kanten und den Skeleton Punkten
        // typedef CGAL::Surface_mesh<Point> PlanarGraph;   das ist der Datentyp
        // Hier alle Knoten durchgehen und Kanten senkrecht zu den Polygon Kanten einfügen
        // Surface Mesh für Straight Skeleton und ursprüngliches Polygon muss separat genommen werden
        // Beachte: "Each perpendicular edge is contained in one skeleton face, and is perpendicular to the graph edge contained in that skeleton face"


        // TODO: Edge Assignment
        Folding folding(skeleton.graph);
        bool reflex = false;

        std::cout << skeleton.graph.halfedges().size() << std::endl;

        for(const auto half_edge : folding.graph.halfedges()) {
            folding.foldtype_map[half_edge] =  FoldType::Reflex;
        }

        auto vertex_0 = folding.graph.add_vertex(polygon_points[0]);

        auto vertex = vertex_0;

        for (int i = 1; i < polygon_points.size(); ++i) {
            auto new_vertex = folding.graph.add_vertex(polygon_points[i]);

            auto edge = folding.graph.add_edge(vertex, new_vertex);
            auto twin = folding.graph.opposite(edge);

            folding.foldtype_map[edge] = FoldType::Convex;
            folding.foldtype_map[twin] = FoldType::Convex;
            vertex = new_vertex;
        }

        // Add the closing edge from last vertex to the first vertex
        auto edge = folding.graph.add_edge(vertex, vertex_0);
        auto twin = folding.graph.opposite(edge);

        folding.foldtype_map[edge] = FoldType::Convex;
        folding.foldtype_map[twin] = FoldType::Convex;

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