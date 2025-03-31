#pragma once

#include "StraightSkeletonTypes.h"
#include <vector>
#include <iostream>

namespace OneCut {

/**
 * @class ISkeletonFace
 * @brief Abstract interface representing a face in the straight skeleton structure.
 * 
 * Provides access to geometric and topological properties of a skeleton face,
 * including vertex access and adjacency information.
 */
class ISkeletonFace {
public:
    /**
     * @brief Virtual destructor for proper polymorphic destruction.
     */
    virtual ~ISkeletonFace() = default;

    /**
     * @brief Gets the number of vertices in this face.
     * @return The vertex count.
     */
    virtual size_t vertexCount() const = 0;

    /**
     * @brief Gets a specific vertex of the face.
     * @param i Index of the vertex to retrieve (0 <= i < vertexCount())
     * @return The requested vertex point.
     * @pre The index must be valid (0 <= i < vertexCount())
     */
    virtual Point vertex(size_t i) const = 0;

    /**
     * @brief Gets all vertices of the face in order.
     * @return Vector containing all vertices in clockwise or counter-clockwise order.
     */
    virtual std::vector<Point> getVertices() const = 0;

    /**
     * @brief Gets indices of all adjacent faces.
     * @return Vector containing indices of neighboring faces.
     * 
     * The adjacency order corresponds to the edge order of this face.
     */
    virtual std::vector<int> getAdjacentFaces() const = 0;

    /**
     * @brief Gets the adjacent face index for a specific edge.
     * @param i Edge index (0 <= i < vertexCount())
     * @return Index of the face adjacent to the i-th edge.
     * @pre The index must be valid (0 <= i < vertexCount())
     */
    virtual int adjacentFaceIndex(int i) const = 0;

    /**
     * @brief Prints face information to an output stream.
     * @param os The output stream to write to.
     * @return Reference to the output stream.
     * 
     * Implementations should output a human-readable representation
     * of the face including its vertices and adjacency information.
     */
    virtual std::ostream& print(std::ostream& os) const = 0;
};

/**
 * @brief Stream insertion operator for ISkeletonFace.
 * @param os The output stream to write to.
 * @param face The face to output.
 * @return Reference to the output stream.
 * 
 * Delegates to the face's print() method to provide polymorphic output.
 */
inline std::ostream& operator<<(std::ostream& os, const ISkeletonFace& face) {
    return face.print(os);
}

} // namespace OneCut