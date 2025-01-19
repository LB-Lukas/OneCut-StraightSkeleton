#pragma once

#include "StraightSkeletonTypes.h"
#include <vector>

namespace straight_skeleton {

class ISkeletonFace {
public:

    virtual ~ISkeletonFace() = default;

    virtual size_t vertexCount() const = 0;

    virtual Point vertex(size_t i) const = 0;

    virtual std::vector<Point> getVertices() const = 0;

    virtual std::vector <int> getAdjacentFaces() const = 0;

    virtual int adjacentFaceIndex(int i) const = 0;
    
};


}