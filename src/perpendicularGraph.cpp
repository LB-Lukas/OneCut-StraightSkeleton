#include "../include/StraightSkeleton/perpendicularGraph.h"
#include "StraightSkeleton/perpendicularGraph.h"

namespace Geometry {

    PerpendicularGraph::PerpendicularGraph() {}

    PerpendicularGraph::PerpendicularGraph(const PlanarGraph &skeletonGraph, const PlanarGraph &cutGraph) : 
        skeletonGraph(skeletonGraph),
        cutGraph(cutGraph) {}

    void PerpendicularGraph::addPerpendiculars() {
        /*
        perpendicular edges always meet graph edges
        at a right angle. In fact, each perpendicular edge is contained in one skeleton face, and is
        perpendicular to the graph edge contained in that skeleton face
        */
        // Für alle Knoten des Skeleton Graphen Perpendiculars berechnen
        for (const auto& vertex : this->skeletonGraph.vertices()) {
            // Für alle Kanten des Cut Graphen Perpendiculars berechnen

            // dem Perpendicular Graphen die Perpendiculars hinzufügen
        }
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
        // Ist wahrscheinlich schwieriger als gedacht
        // aktuell wird deswegen als Platzhalter nur der Skeleton Graph zurückgegeben
        // wahrschinlich ist es auch nicht so wichtig diese Feature zu implementieren, daher *OPTIONAL?*
        throw std::runtime_error("Method not yet implemented.");
        return this->skeletonGraph;
    }

    std::pair<Point, Point> PerpendicularGraph::calculatePerpendicularBisector(const std::pair<Point, Point> &edge) {
        // The perpendicular must be perpendicular to the edge of the cut graph and its origin must be the vertex of the skeleton graph
        throw std::runtime_error("Method not yet implemented.");
        return std::pair<Point, Point>(Point(0, 0), Point(0, 0));
    }

}