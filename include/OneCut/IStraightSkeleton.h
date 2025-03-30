#pragma once

#include "StraightSkeletonTypes.h"
#include "ISkeletonFace.h"

namespace OneCut {

/**
 * @class IStraightSkeleton
 * @brief Abstract interface representing a straight skeleton structure.
 *
 * Provides access to the faces that compose the straight skeleton of a polygon.
 * The straight skeleton is a topological structure representing the inward 
 * offsetting of a polygon's edges.
 */
class IStraightSkeleton {
public:
    /**
     * @brief Virtual destructor for proper polymorphic destruction.
     */
    virtual ~IStraightSkeleton() = default;

    /**
     * @brief Gets the number of faces in the straight skeleton.
     * @return The total count of faces in the skeleton structure.
     * 
     * @note The face count includes both the interior faces and the outer face(s)
     *       representing the original polygon boundary.
     */
    virtual size_t faceCount() const = 0;

    /**
     * @brief Gets a specific face from the straight skeleton.
     * @param i Index of the face to retrieve (0 ≤ i < faceCount())
     * @return Const reference to the requested face object.
     * @pre The index must be valid (0 ≤ i < faceCount())
     * 
     * @warning The returned reference remains valid only while the skeleton exists.
     *          Callers should not store this reference long-term.
     */
    virtual const ISkeletonFace& face(size_t i) const = 0;
};

} // namespace OneCut