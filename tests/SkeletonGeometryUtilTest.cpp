#include <gtest/gtest.h>

#include "straight_skeleton/StraightSkeleton.h"
#include "straight_skeleton/utils/RayTriangleUtil.h"

namespace straight_skeleton {
// TODO: Add more test for some corner cases


TEST(SkeletonGeometryUtilTest, MakeTriangle) {
    // Test for a simple square
    std::vector<Point2D> polygonPoints = {Point2D(0, 0), Point2D(1, 0), Point2D(1, 1), Point2D(0, 1)};
    Polygon2D polygon(polygonPoints.begin(), polygonPoints.end());
    Triangle triangle = SkeletonGeometryUtil::makeTriangle(0, polygon);
    EXPECT_EQ(triangle.getBaseA(), Point3D(0, 0, 0));
    EXPECT_EQ(triangle.getBaseB(), Point3D(1, 0, 0));
    EXPECT_NEAR(CGAL::to_double(triangle.getApex().x()), 0.5, 1e-6);
    EXPECT_NEAR(CGAL::to_double(triangle.getApex().y()), 0.5, 1e-6);
    EXPECT_NEAR(CGAL::to_double(triangle.getApex().z()), 0.5, 1e-6);
}

TEST(SkeletonGeometryUtilTest, IntersectLines) {
    Point2D originA(0, 0);
    Vector2D dirA(1, 1);
    Point2D originB(0, 1);
    Vector2D dirB(1, -1);

    Point2D intersection = SkeletonGeometryUtil::intersectLines(originA, dirA, originB, dirB);
    EXPECT_EQ(intersection, Point2D(0.5, 0.5));
}

TEST(SkeletonGeometryUtilTest, Rotate90) {
    Vector2D vec(1, 0);
    Vector2D rotatedVec = SkeletonGeometryUtil::rotate90(vec);
    EXPECT_EQ(rotatedVec, Vector2D(0, 1));
}

TEST(SkeletonGeometryUtilTest, Normalize) {
    Vector2D vec(0, 9);
    Vector2D normalizedVec = SkeletonGeometryUtil::normalize(vec);
    EXPECT_EQ(CGAL::to_double(normalizedVec.y()), 1.0);
}

}  // namespace straight_skeleton