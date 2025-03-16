#pragma once

#include <utility>

#include "StraightSkeletonTypes.h"

namespace straight_skeleton {

enum class Origin { POLYGON, SKELETON, PERPENDICULAR };

enum class FoldType { MOUNTAIN, VALLEY, UNFOLDED };

struct Crease {
    std::pair<Point, Point> edge;
    FoldType foldType;
    Origin origin;
    int faceIndex;        // Index of the face in the skeleton
    int edgeIndex;        // Index of the edge in the face
    bool isBoundaryEdge;  // True if this crease is a boundary edge of the polygon
};

}  // namespace straight_skeleton
