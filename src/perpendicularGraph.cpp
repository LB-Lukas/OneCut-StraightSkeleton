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
            // -> also für jede Skeleton Face berechenen!
            for (const auto halfedge : CGAL::halfedges_around_target(vertex, this->skeletonGraph)) {
                PlanarGraph::Face_index face = this->skeletonGraph.face(halfedge);

                // Only handle faces that are not the outer face
                if (face == PlanarGraph::null_face()) {
                    continue;
                }

                // Get one halfedge of the current face
                auto h = this->skeletonGraph.halfedge(face);
                for (auto edge : this->skeletonGraph.halfedges_around_face(h)) {
                    // Check if the edge is a cut edge
                    if (GraphUtils::isEdgeInGraph(this->cutGraph, std::make_pair(this->skeletonGraph.point(this->skeletonGraph.source(edge)), this->skeletonGraph.point(this->skeletonGraph.target(edge))))) {
                        // Calculate perpendicular bisector
                        Point p = this->skeletonGraph.point(vertex);
                        Point first = this->skeletonGraph.point(this->skeletonGraph.source(edge));
                        Point second = this->skeletonGraph.point(this->skeletonGraph.target(edge));
                        auto perpendicular = calculatePerpendicularBisector(p, std::pair<Point, Point>(first, second));
                        // Add perpendicular to the perpendicular graph
                        auto v1 = this->perpendicularGraph.add_vertex(perpendicular.first);
                        auto v2 = this->perpendicularGraph.add_vertex(perpendicular.second);
                        this->perpendicularGraph.add_edge(v1, v2);
                    }
                }

            }
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

    std::pair<Point, Point> PerpendicularGraph::calculatePerpendicularBisector(const Point& point,const std::pair<Point, Point> &edge) {
        // The perpendicular must be perpendicular to the edge of the cut graph and its origin must be the vertex of the skeleton graph
        Point A = edge.first;
        Point B = edge.second;
        Line AB(A, B);

        // calculate perpendicular point on edge AB
        Point L = AB.projection(point);

        return std::pair<Point, Point>(point, L);


        /*
        throw std::runtime_error("Method not yet implemented.");
        return std::pair<Point, Point>(Point(0, 0), Point(0, 0));
        */
    }

}