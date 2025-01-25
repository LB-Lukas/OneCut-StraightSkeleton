#include "straight_skeleton/SkeletonFace.h"

namespace straight_skeleton {

SkeletonFace::SkeletonFace(const std::vector<Point>& vertices, const std::vector<int>& adjacentFaces) : vertices(vertices), adjacentFaces(adjacentFaces) {}

size_t SkeletonFace::vertexCount() const {
    return vertices.size();
}

Point SkeletonFace::vertex(size_t i) const {
    return vertices[i];
}

std::vector<Point> SkeletonFace::getVertices() const {
    return vertices;
}

std::vector<int> SkeletonFace::getAdjacentFaces() const {
    return adjacentFaces;
}

int SkeletonFace::adjacentFaceIndex(int i) const {
    return adjacentFaces[i];
}

}  // namespace straight_skeleton