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

}