#pragma once

#include "StraightSkeletonTypes.h"
#include "ISkeletonFace.h"
#include <iostream>

namespace OneCut {

/**
 * @class SkeletonFace
 * @brief Concrete implementation of ISkeletonFace representing a face in a straight skeleton.
 *
 * This class stores the geometric and topological information of a single face
 * in the straight skeleton structure, including its vertices and adjacent faces.
 */
class SkeletonFace : public ISkeletonFace {
public:
    /**
     * @brief Constructs a SkeletonFace with given vertices and adjacency information.
     * @param vertices The ordered vertices of the face (clockwise or counter-clockwise)
     * @param adjacentFaces Indices of adjacent faces corresponding to each edge
     * @pre vertices.size() == adjacentFaces.size()
     * @pre vertices.size() >= 3 (must form a valid polygon)
     */
    SkeletonFace(const std::vector<Point>& vertices, const std::vector<int>& adjacentFaces);

    /// @name ISkeletonFace Interface Implementation
    /// @{
    size_t vertexCount() const override;
    Point vertex(size_t i) const override;
    std::vector<Point> getVertices() const override;
    std::vector<int> getAdjacentFaces() const override;
    int adjacentFaceIndex(int i) const override;
    std::ostream& print(std::ostream& os) const override;
    /// @}

    std::vector<Point> vertices;      ///< Ordered vertices of the face
    std::vector<int> adjacentFaces;   ///< Indices of adjacent faces for each edge
    bool isOuter = false;            ///< Flag indicating if this is the outer face (boundary)

private:
    // Note: Implementation details would go here if there were any private members
};

} // namespace OneCut