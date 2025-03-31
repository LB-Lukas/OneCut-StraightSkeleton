#include <gtest/gtest.h>

#include <vector>

#include "OneCut/PerpendicularFinder.h"
#include "OneCut/SkeletonBuilder.h"
#include "OneCut/SkeletonFace.h"
#include "OneCut/StraightSkeleton.h"

namespace OneCut {

TEST(PerpendicularFinderTest, BasicTest) {
    std::vector<SkeletonConstruction::Point> square = {
        SkeletonConstruction::Point(100, 100), SkeletonConstruction::Point(500, 100),
        SkeletonConstruction::Point(500, 500), SkeletonConstruction::Point(100, 500)};

    SkeletonConstruction::SkeletonBuilder builder(square);
    auto skeleton = builder.buildSkeleton();

    PerpendicularFinder finder(skeleton);
    std::vector<PerpChain> chains = finder.findPerpendiculars();
    EXPECT_GE(chains.size(), 1);
}

}  // namespace OneCut
