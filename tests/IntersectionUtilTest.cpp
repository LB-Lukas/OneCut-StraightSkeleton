#include <gtest/gtest.h>
#include "OneCut/utils/IntersectionUtil.h"

namespace OneCut {

TEST(IntersectionUtilTest, BasicIntersectionMiddle) {
    Point origin(0, 0);
    Vector direction(1, 0);
    Point segStart(1, -1);
    Point segEnd(1, 1);
    
    auto result = IntersectionUtil::intersectRaySegment(origin, direction, segStart, segEnd);
    
    EXPECT_TRUE(result.valid);
    EXPECT_DOUBLE_EQ(result.rayParam, 1.0);
    EXPECT_DOUBLE_EQ(result.segmentParam, 0.5);
}

TEST(IntersectionUtilTest, NoIntersection) {
    Point origin(2, 2);
    Vector direction(0, 1);
    Point segStart(0, 0);
    Point segEnd(0, 5);
    
    auto result = IntersectionUtil::intersectRaySegment(origin, direction, segStart, segEnd);
    
    EXPECT_FALSE(result.valid);
}

TEST(IntersectionUtilTest, RayStartsOnSegment) {
    Point origin(1, 1);
    Vector direction(1, 0);
    Point segStart(0, 0);
    Point segEnd(2, 2);
    
    auto result = IntersectionUtil::intersectRaySegment(origin, direction, segStart, segEnd);
    
    EXPECT_TRUE(result.valid);
    EXPECT_DOUBLE_EQ(result.rayParam, 0.0);
    EXPECT_DOUBLE_EQ(result.segmentParam, 0.5);
}

TEST(IntersectionUtilTest, ParallelNoIntersection) {
    Point origin(0, 0);
    Vector direction(1, 1);
    Point segStart(1, 2);
    Point segEnd(3, 4);
    
    auto result = IntersectionUtil::intersectRaySegment(origin, direction, segStart, segEnd);
    
    EXPECT_FALSE(result.valid);
}

TEST(IntersectionUtilTest, ZeroLengthSegment) {
    Point origin(0, 0);
    Vector direction(1, 0);
    Point segStart(2, 2);
    Point segEnd(2, 2);
    
    auto result = IntersectionUtil::intersectRaySegment(origin, direction, segStart, segEnd);
    
    EXPECT_FALSE(result.valid);
}

TEST(IntersectionUtilTest, ZeroDirectionVector) {
    Point origin(0, 0);
    Vector direction(0, 0);
    Point segStart(1, 1);
    Point segEnd(2, 2);
    
    auto result = IntersectionUtil::intersectRaySegment(origin, direction, segStart, segEnd);
    
    EXPECT_FALSE(result.valid);
}

TEST(IntersectionUtilTest, VerticalRayHorizontalSegment) {
    Point origin(2, 2);
    Vector direction(0, 1);
    Point segStart(1, 3);
    Point segEnd(3, 3);
    
    auto result = IntersectionUtil::intersectRaySegment(origin, direction, segStart, segEnd);
    
    EXPECT_TRUE(result.valid);
    EXPECT_DOUBLE_EQ(result.rayParam, 1.0);
    EXPECT_DOUBLE_EQ(result.segmentParam, 0.5);
}

}  // namespace OneCut