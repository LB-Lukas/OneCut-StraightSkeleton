#pragma once

#include <unordered_map>
#include <vector>

#include "IStraightSkeleton.h"
#include "SkeletonFace.h"
#include "StraightSkeletonTypes.h"

namespace OneCut {

/**
 * @class StraightSkeleton
 * @brief Concrete implementation of IStraightSkeleton representing a complete straight skeleton structure.
 *
 * This class stores and provides access to all faces that compose the straight skeleton
 * of a polygon. The straight skeleton is a topological structure representing the inward
 * and outward offsetting of a polygon's edges.
 * This class contains all faces from the inside and the outside.
 */
class StraightSkeleton : public IStraightSkeleton {
   public:
    /**
     * @brief Constructs a StraightSkeleton from a set of faces.
     * @param faces Vector of SkeletonFace objects that compose the skeleton
     * @pre !faces.empty() (must contain at least one face)
     */
    StraightSkeleton(const std::vector<SkeletonFace>& faces);

    /**
     * @brief Gets the total number of faces in the skeleton.
     * @return The count of faces including both interior and boundary faces
     */
    size_t faceCount() const override;

    /**
     * @brief Gets a specific face from the skeleton by index.
     * @param i Index of the face to retrieve (0 ≤ i < faceCount())
     * @return Const reference to the requested SkeletonFace
     * @pre The index must be valid (0 ≤ i < faceCount())
     * @warning The returned reference is valid only while the skeleton exists
     */
    const SkeletonFace& face(size_t i) const override;

    /**
     * @brief Gets a copy of all faces in the skeleton.
     * @return Vector containing copies of all SkeletonFace objects
     * @note This returns copies rather than references for safe usage when
     *       the original skeleton might be destroyed
     */
    std::vector<SkeletonFace> getFaces() const;

   private:
    std::vector<SkeletonFace> faces; ///< Storage for all faces composing the skeleton
};

}  // namespace OneCut