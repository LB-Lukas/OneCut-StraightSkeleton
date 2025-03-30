#pragma once

#include "StraightSkeletonTypes.h"
#include <vector>
#include <iostream>

namespace OneCut {

class ISkeletonFace {
public:

    virtual ~ISkeletonFace() = default;

    virtual size_t vertexCount() const = 0;

    virtual Point vertex(size_t i) const = 0;

    virtual std::vector<Point> getVertices() const = 0;

    virtual std::vector <int> getAdjacentFaces() const = 0;

    virtual int adjacentFaceIndex(int i) const = 0;

    virtual std::ostream& print(std::ostream& os) const = 0;
    
};

inline std::ostream& operator<<(std::ostream& os, const ISkeletonFace& face) {
    return face.print(os);
}


}