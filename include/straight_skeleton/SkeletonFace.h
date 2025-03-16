#pragma once

#include "StraightSkeletonTypes.h"
#include "ISkeletonFace.h"
#include <iostream>

namespace straight_skeleton {

class SkeletonFace : public ISkeletonFace{
public:
    SkeletonFace(const std::vector<Point>& vertices, const std::vector<int>& adjacentFaces);

    size_t vertexCount() const override;
    Point vertex(size_t i) const override;
    std::vector<Point> getVertices() const override;
    std::vector<int> getAdjacentFaces() const override;
    int adjacentFaceIndex(int i) const override;

    std::ostream& print(std::ostream& os) const override;

    std::vector<Point> vertices;
    std::vector<int> adjacentFaces;


private:

};


}
