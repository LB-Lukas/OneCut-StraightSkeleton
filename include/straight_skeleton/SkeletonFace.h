#pragma once

#include "StraightSkeletonTypes.h"
#include "GraphClassification.h"
#include "GraphUtils.h"
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

    friend std::ostream& operator<<(std::ostream& os, const SkeletonFace& face);


private:
    std::vector<Point> vertices;
    std::vector<int> adjacentFaces;
};


}
