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

std::ostream& operator<<(std::ostream& os, const SkeletonFace& face) {
    os << "{ \"vertices\": [";
    {
        const auto& verts = face.getVertices();
        for (size_t i = 0; i < verts.size(); ++i) {
            os << verts[i];
            if (i != verts.size() - 1)
                os << ", ";
        }
    }
    os << "], \"adjacentFaces\": [";
    {
        const auto& adjFaces = face.getAdjacentFaces();
        for (size_t i = 0; i < adjFaces.size(); ++i) {
            os << adjFaces[i];
            if (i != adjFaces.size() - 1)
                os << ", ";
        }
    }
    os << "] }";
    return os;
}

}  // namespace straight_skeleton