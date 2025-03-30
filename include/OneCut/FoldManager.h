#pragma once

#include <cmath>
#include <utility>
#include <vector>

#include "Crease.h"
#include "PerpendicularFinder.h"
#include "SkeletonBuilder.h"
#include "StraightSkeleton.h"
#include "StraightSkeletonTypes.h"
#include "utils/GeometryUtil.h"

namespace OneCut {

/**
 * @class FoldManager
 * @brief Manages the generation and processing of creases for one-cut origami folding.
 * 
 * This class coordinates the computation of the straight skeleton from an input polygon,
 * finds perpendicular folds, and generates the complete set of creases needed for folding.
 */
class FoldManager {
   public:
    /**
     * @brief Constructs a FoldManager for the given polygon.
     * @param polygon The input polygon represented as a vector of points.
     */
    FoldManager(const std::vector<TestSkeleton::Point>& polygon);

    /**
     * @brief Retrieves all creases computed by the FoldManager.
     * @return A vector containing all generated Crease objects, including:
     *         - Original polygon edges
     *         - Straight skeleton creases
     *         - Perpendicular folds
     */
    std::vector<Crease> getCreases();

   private:
    TestSkeleton::SkeletonBuilder skeletonBuilder; ///< Builder for computing the straight skeleton
    StraightSkeleton skeleton;                     ///< Computed straight skeleton structure
    PerpendicularFinder perpendicularFinder;       ///< Finds perpendicular folds in the skeleton
};

}  // namespace OneCut