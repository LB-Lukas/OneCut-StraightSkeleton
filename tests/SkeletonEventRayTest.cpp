#include "gtest/gtest.h"
#include "straight_skeleton/event_management/SkeletonEventRay.h"
#include "straight_skeleton/StraightSkeleton.h"
#include <memory>
#include <stdexcept>
#include <unordered_set>

namespace straight_skeleton {

class SkeletonEventRayTest : public ::testing::Test {
protected:
    std::shared_ptr<SkeletonEventRay> ray1;
    std::shared_ptr<SkeletonEventRay> ray2;
    std::shared_ptr<SkeletonEventRay> ray3;

    void SetUp() override {
        // Initialisiere Test-Rays
        ray1 = std::make_shared<SkeletonEventRay>(Point(0, 0), Direction(1, 0));
        ray2 = std::make_shared<SkeletonEventRay>(Point(1, 0), Direction(0, 1));
        ray3 = std::make_shared<SkeletonEventRay>(Point(1, 1), Direction(-1, 0));

        // Forme eine geschlossene Schleife
        ray1->left_ray = ray3;
        ray1->right_ray = ray2;

        ray2->left_ray = ray1;
        ray2->right_ray = ray3;

        ray3->left_ray = ray2;
        ray3->right_ray = ray1;
    }
};

TEST_F(SkeletonEventRayTest, TestLoopValidation_ValidLoop) {
    EXPECT_NO_THROW(ray1->assertValide());
    EXPECT_NO_THROW(ray2->assertValide());
    EXPECT_NO_THROW(ray3->assertValide());
}

TEST_F(SkeletonEventRayTest, TestLoopValidation_InvalidLoop) {
    // Breche die Schleife
    ray1->left_ray = nullptr;

    EXPECT_THROW(ray1->assertValide(), std::runtime_error);
    EXPECT_THROW(ray2->assertValide(), std::runtime_error);
    EXPECT_THROW(ray3->assertValide(), std::runtime_error);
}

TEST_F(SkeletonEventRayTest, TestEqualityOperator) {
    SkeletonEventRay r1(Point(1, 2), Direction(1, 0));
    SkeletonEventRay r2(Point(1, 2), Direction(1, 0));
    SkeletonEventRay r3(Point(3, 4), Direction(0, 1));

    EXPECT_TRUE(r1 == r2);
    EXPECT_FALSE(r1 == r3);
}

TEST_F(SkeletonEventRayTest, TestHashFunction) {
    SkeletonEventRay r1(Point(1, 2), Direction(1, 0));
    SkeletonEventRay r2(Point(1, 2), Direction(1, 0));
    SkeletonEventRay r3(Point(3, 4), Direction(0, 1));

    std::unordered_set<SkeletonEventRay, std::hash<SkeletonEventRay>> ray_set;
    ray_set.insert(r1);

    EXPECT_TRUE(ray_set.find(r2) != ray_set.end());
    EXPECT_TRUE(ray_set.find(r3) == ray_set.end());
}

} // namespace straight_skeleton
