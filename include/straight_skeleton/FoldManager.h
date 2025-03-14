#pragma once

#include <utility>
#include <vector>

#include "PerpendicularFinder.h"
#include "TestStraightSkeleton.h"
#include "Crease.h"

namespace straight_skeleton {

class FoldManager {
   public:

    FoldManager(const std::vector<TestSkeleton::Point>& polygon);

    std::vector<Crease> getCreases();

   private:
    TestSkeleton::TestStraightSkeleton skeletonBuilder;
    PerpendicularFinder perpendicularFinder;
};

}  // namespace straight_skeleton
