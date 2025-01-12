#include "../include/StraightSkeleton/perpendicularGraph.h"
#include "StraightSkeleton/perpendicularGraph.h"
#include <iostream>

namespace Geometry {

    PerpendicularGraph::PerpendicularGraph() {}

    PerpendicularGraph::PerpendicularGraph(const PlanarGraph &skeletonGraph, const PlanarGraph &cutGraph) : skeletonGraph(skeletonGraph),
                                                                                                            cutGraph(cutGraph) {}

    void PerpendicularGraph::addPerpendiculars() {
        std::cout << "Starting to add perpendiculars..." << std::endl;

        //for (const auto &skeletonVertex : skeletonGraph.vertices()) {
        // TODO: Check if skeleton Vertex degree is odd
        auto iterator = cutGraph.vertices().begin();
        auto skeletonVertex = *iterator;
        std::cout << "S_V: " << skeletonGraph.point(skeletonVertex) <<":" << std::endl << std::endl;
        for (const auto &cutEdge : cutGraph.edges()) {
            auto halfedge = cutGraph.halfedge(cutEdge);
            std::cout << "for C_E " << cutGraph.point(cutGraph.source(halfedge)) << " to " << cutGraph.point(cutGraph.target(halfedge)) << " ";

            std::pair<Point, Point> perpendicular = calculatePerpendicularBisector(skeletonGraph.point(skeletonVertex), std::make_pair(cutGraph.point(cutGraph.source(halfedge)), cutGraph.point(cutGraph.target(halfedge))));
            std::cout << "=> Bisector from " << perpendicular.first << " to " << perpendicular.second << std::endl;

            auto v1 = perpendicularGraph.add_vertex(perpendicular.first);
            auto v2 = perpendicularGraph.add_vertex(perpendicular.second);
            perpendicularGraph.add_edge(v1, v2);
            std::cout << "Added perpendicular edge from vertex " << v1 << " to vertex " << v2 << std::endl;
        }
        std::cout << "-----------------------------------" << std::endl;
        //}
        std::cout << "Finished adding perpendiculars." << std::endl;
    }

    PlanarGraph PerpendicularGraph::getPerpendicularGraph() const {
        return this->perpendicularGraph;
    }

    PlanarGraph PerpendicularGraph::getCutGraph() const {
        return this->cutGraph;
    }

    PlanarGraph PerpendicularGraph::getStraightSkeletonGraph() const {
        return this->skeletonGraph;
    }

    PlanarGraph PerpendicularGraph::getCompleteGraph() const {
        // TODO: Implement this method
        // nicht so trivial wie angenommen
        // aktuell wird deswegen als Platzhalter nur der Skeleton Graph zurückgegeben
        // wahrschinlich ist es auch nicht so wichtig diese Feature zu implementieren, daher *OPTIONAL?*
        throw std::runtime_error("Method not yet implemented.");
        return this->skeletonGraph;
    }

    std::pair<Point, Point> PerpendicularGraph::calculatePerpendicularBisector(const Point &point, const std::pair<Point, Point> &edge) {
        // The perpendicular must be perpendicular to the edge of the cut graph and its origin must be the vertex of the skeleton graph
        Point A = edge.first;
        Point B = edge.second;
        Line AB(A, B);

        // Degenerate case: edge reduced to a single point
        if (A == B) {
            return std::pair<Point, Point>(point, A); // Projection is the degenerate edge point
        }

        // calculate perpendicular point on edge AB
        Point L = AB.projection(point);

        return std::pair<Point, Point>(point, L);
    }

}