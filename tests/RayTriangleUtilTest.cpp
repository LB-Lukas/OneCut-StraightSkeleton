#include <gtest/gtest.h>

#include "straight_skeleton/utils/RayTriangleUtil.h"

namespace straight_skeleton {

TEST(RayTriangleUtilTest, IntersectRayTriangle) {
    Ray3D ray(Point3D(0, 0, 0), Direction3D(0, 0, 1));
    Triangle3D triangle(Point3D(-5, -5, 2), Point3D(-5, 5, 2), Point3D(5, 5, 2));

    // Call the intersection function
    auto result = RayTriangleUtil::intersectRayTriangle(ray, triangle);

    // Check if the result is valid
    ASSERT_TRUE(result.has_value());

    // Check if the intersection point is correct
    auto [t, intersectionPoint] = result.value();
    EXPECT_NEAR(t, 2.0, 1e-6);
    EXPECT_NEAR(CGAL::to_double(intersectionPoint.x()), 0.0, 1e-6);
}

TEST(RayTriangleUtilTest, NoIntersection) {
    Ray3D ray(Point3D(0, 0, 0), Direction3D(0, 0, -1));
    Triangle3D triangle(Point3D(-5, -5, 2), Point3D(-5, 5, 2), Point3D(5, 5, 2));

    // Call the intersection function
    auto result = RayTriangleUtil::intersectRayTriangle(ray, triangle);

    // Check if the result is invalid
    ASSERT_FALSE(result.has_value());
}

TEST(RayTriangleUtilTest, SamePlaneIntersection) {
    Ray3D ray(Point3D(0, 2, 0), Direction3D(0, -1, 0));
    Triangle3D triangle(Point3D(-1,0,0), Point3D(1,0,0), Point3D(0,-1,0));

    auto result = RayTriangleUtil::intersectRayTriangle(ray, triangle);

    ASSERT_TRUE(result.has_value());
    EXPECT_NEAR(result.value().first, 2.0, 1e-9);

}
}  // namespace straight_skeleton