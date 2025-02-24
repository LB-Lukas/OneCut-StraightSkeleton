#pragma once

#include "StraightSkeletonTypes.h"
#include "ISkeletonFace.h"
#include <iostream>

namespace straight_skeleton {

class SkeletonFace : public ISkeletonFace{
public:
    SkeletonFace(const std::vector<Point2D>& vertices, const std::vector<int>& adjacentFaces);

    size_t vertexCount() const override;
    Point2D vertex(size_t i) const override;
    std::vector<Point2D> getVertices() const override;
    std::vector<int> getAdjacentFaces() const override;
    int adjacentFaceIndex(int i) const override;

    std::ostream& print(std::ostream& os) const override;


private:
    std::vector<Point2D> vertices;
    std::vector<int> adjacentFaces;
};


}
