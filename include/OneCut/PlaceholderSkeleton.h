#pragma once

#include "StraightSkeletonTypes.h"
#include "SkeletonFace.h"
#include "IStraightSkeleton.h"
#include <vector>
#include <stddef.h>

namespace OneCut {

/**
 * @class PlaceholderSkeleton
 * @brief Temporary implementation of IStraightSkeleton used for testing.
 * @deprecated This class was used for Testing; might be removed.
 *             Use the proper Skeleton implementation instead.
 *
 * This class provides a simple wrapper around a vector of faces for testing
 * purposes, but doesn't implement actual straight skeleton functionality.
 */
class PlaceholderSkeleton : public IStraightSkeleton {
public:
    /**
     * @brief Constructs a placeholder skeleton from existing faces.
     * @deprecated Temporary constructor for testing purposes.
     * @param faces Vector of pre-computed skeleton faces.
     */
    PlaceholderSkeleton(const std::vector<SkeletonFace>& faces);

    /**
     * @brief Gets the number of faces in the skeleton.
     * @deprecated Temporary implementation - use real skeleton instead.
     * @return The number of faces in the placeholder skeleton.
     */
    size_t faceCount() const override;

    /**
     * @brief Gets a specific face from the skeleton.
     * @deprecated Temporary implementation - use real skeleton instead.
     * @param i Index of the face to retrieve (0 ≤ i < faceCount())
     * @return Const reference to the requested face.
     * @pre The index must be valid (0 ≤ i < faceCount())
     */
    const SkeletonFace& face(size_t i) const override;

private:
    const std::vector<SkeletonFace>& faces; ///< @deprecated Temporary face storage
};

} // namespace OneCut