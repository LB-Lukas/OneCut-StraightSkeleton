#include <gtest/gtest.h>

#include <vector>

#include "straight_skeleton/PerpendicularFinder.h"
#include "straight_skeleton/SkeletonFace.h"
#include "straight_skeleton/StraightSkeleton.h"

namespace straight_skeleton {

class PerpendicularFinderTest : public ::testing::Test {
   protected:
    std::vector<SkeletonFace> faces;
    void SetUp() override {
        Point p0(0, 200);
        Point p1(400, 200);
        Point p2(400, 0);
        Point p3(0, 0);
        Point s0(100, 100);
        Point s1(300, 100);

        // Skeleton faces
        std::vector<Point> f0 = {p0, p1, s1, s0};
        std::vector<Point> f1 = {p1, p2, s1};
        std::vector<Point> f2 = {p2, p3, s0, s1};
        std::vector<Point> f3 = {p3, p0, s0};

        // Define Adjacent Faces
        SkeletonFace face0(f0, {-1, 1, 2, 3});
        SkeletonFace face1(f1, {-1, 2, 0});
        SkeletonFace face2(f2, {-1, 3, 0, 1});
        SkeletonFace face3(f3, {-1, 0, 2});

        faces = {face0, face1, face2, face3};
    }

    void TearDown() override {}
};

TEST_F(PerpendicularFinderTest, FindPerpendiculars) {
    StraightSkeleton skeleton(faces);
    PerpendicularFinder finder(skeleton);
    std::vector<PerpChain> result = finder.findPerpendiculars();

    // Check the size of the result
    EXPECT_EQ(result.size(), 2);
}

}  // namespace straight_skeleton
