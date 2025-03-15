#pragma once

#include <utility>
#include <vector>
#include <cmath>
#include <utility>

#include "Crease.h"
#include "PerpendicularFinder.h"
#include "SkeletonBuilder.h"
#include "StraightSkeleton.h"
#include "StraightSkeletonTypes.h"

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
