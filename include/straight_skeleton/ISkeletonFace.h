#pragma once

#include "StraightSkeletonTypes.h"
#include <vector>
#include <iostream>

namespace straight_skeleton {

class ISkeletonFace {
public:

    virtual ~ISkeletonFace() = default;

    virtual size_t vertexCount() const = 0;

    virtual Point2D vertex(size_t i) const = 0;

    virtual std::vector<Point2D> getVertices() const = 0;

    virtual std::vector <int> getAdjacentFaces() const = 0;

    virtual int adjacentFaceIndex(int i) const = 0;

    virtual std::ostream& print(std::ostream& os) const = 0;
    
};

inline std::ostream& operator<<(std::ostream& os, const ISkeletonFace& face) {
    return face.print(os);
}


}