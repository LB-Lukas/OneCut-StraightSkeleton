#include "../include/StraightSkeleton/graphUtils.h"

namespace Geometry {

    bool GraphUtils::areGraphsEqual(const PlanarGraph& graph1, const PlanarGraph& graph2) {
        if (graph1.number_of_vertices() != graph2.number_of_vertices()) {
            return false;
        }

        if (graph1.number_of_faces() != graph2.number_of_faces()) {
            return false;
        }

        if (graph1.number_of_edges() != graph2.number_of_edges()) {
            return false;
        }

        // TODO: CHECK if there are more attributes that can be compared

        return true;
    }

    bool GraphUtils::isEdgeInGraph(const PlanarGraph& graph, const std::pair<Point, Point>& edge) {
        const Point& p1 = edge.first;
        const Point& p2 = edge.second;

        // Iterate through all the halfedges in the graph
        for (const auto& graph_edge : graph.edges()) {
            // Get the halfedge corresponding to the edge
            auto halfedge = graph.halfedge(graph_edge);

            // Get the two points that form the current edge from the halfedges
            Point edge_point1 = graph.point(graph.target(halfedge));
            Point edge_point2 = graph.point(graph.source(halfedge));

            // Check if the edge is either (p1, p2) or (p2, p1)
            if ((edge_point1 == p1 && edge_point2 == p2) || 
                (edge_point1 == p2 && edge_point2 == p1)) {
                return true;
            }
        }
        return false;
    }

}