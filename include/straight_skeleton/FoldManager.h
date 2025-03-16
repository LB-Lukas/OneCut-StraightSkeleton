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

namespace straight_skeleton {

class FoldManager {
   public:
    FoldManager(const std::vector<TestSkeleton::Point>& polygon);

    std::vector<Crease> getCreases();

   private:
    TestSkeleton::SkeletonBuilder skeletonBuilder;
    StraightSkeleton skeleton;
    PerpendicularFinder perpendicularFinder;
};

}  // namespace straight_skeleton
