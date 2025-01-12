#include "gtest/gtest.h"
#include "straight_skeleton/StraightSkeleton.h"
#include "straight_skeleton/event_management/SkeletonEventTriangle.h"
#include <memory>
#include <stdexcept>
#include <unordered_set>

namespace straight_skeleton {
    class SkeletonEventTriangleTest : public ::testing::Test {
    protected:
        std::shared_ptr<SkeletonEventTriangle> triangle1;
        std::shared_ptr<SkeletonEventTriangle> triangle2;
        std::shared_ptr<SkeletonEventTriangle> triangle3;

        void SetUp() override {
            // Initialize test triangles
            triangle1 = std::make_shared<SkeletonEventTriangle>();
            triangle2 = std::make_shared<SkeletonEventTriangle>();
            triangle3 = std::make_shared<SkeletonEventTriangle>();

            // Form a loop
            triangle1->l_triangle_ptr = triangle3;
            triangle1->r_triangle_ptr = triangle2;

            triangle2->l_triangle_ptr = triangle1;
            triangle2->r_triangle_ptr = triangle3;

            triangle3->l_triangle_ptr = triangle2;
            triangle3->r_triangle_ptr = triangle1;
        }
    };

    TEST_F(SkeletonEventTriangleTest, TestLoopValidation_ValidLoop) {
        EXPECT_NO_THROW(triangle1->assertValide());
        EXPECT_NO_THROW(triangle2->assertValide());
        EXPECT_NO_THROW(triangle3->assertValide());
    }

    TEST_F(SkeletonEventTriangleTest, TestLoopValidation_InvalidLoop) {
        // Break the loop
        triangle1->l_triangle_ptr = nullptr;

        EXPECT_THROW(triangle1->assertValide(), std::runtime_error);
        EXPECT_THROW(triangle2->assertValide(), std::runtime_error);
        EXPECT_THROW(triangle3->assertValide(), std::runtime_error);
    }

    TEST_F(SkeletonEventTriangleTest, TestEqualityOperator) {
        SkeletonEventTriangle t1, t2;
        t1.intersect_point = Point(1, 2);
        t2.intersect_point = Point(1, 2);

        EXPECT_TRUE(t1 == t2);

        t2.intersect_point = Point(3, 4);
        EXPECT_FALSE(t1 == t2);
    }

    TEST_F(SkeletonEventTriangleTest, TestHashFunction) {
        SkeletonEventTriangle t1, t2, t3;
        t1.intersect_point = Point(1, 2);
        t2.intersect_point = Point(1, 2);
        t3.intersect_point = Point(3, 4);

        std::unordered_set<SkeletonEventTriangle, std::hash<SkeletonEventTriangle> > triangle_set;
        triangle_set.insert(t1);

        EXPECT_TRUE(triangle_set.find(t2) != triangle_set.end());
        EXPECT_TRUE(triangle_set.find(t3) == triangle_set.end());
    }
}
