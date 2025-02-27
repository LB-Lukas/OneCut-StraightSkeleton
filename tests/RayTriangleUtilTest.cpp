#include <gtest/gtest.h>
#include "straight_skeleton/utils/RayTriangleUtil.h"

namespace straight_skeleton {

    TEST(RayTriangleUtilTest, IntersectRayTriangle) {
        // Define a ray and a triangle
        Ray3D ray(Point3D(0, 0, 0), Direction3D(0, 0, 1));
        Triangle3D triangle(Point3D(-5, -5, 2), Point3D(-5, 5, 2), Point3D(5, 5, 2));

        // Call the intersection function
        auto result = RayTriangleUtil::intersectRayTriangle(ray, triangle);

        // Check if the result is valid
        ASSERT_TRUE(result.has_value());

        // Check if the intersection point is correct
        auto [t, intersectionPoint] = result.value();
        EXPECT_NEAR(CGAL::to_double(t), 2.0, 1e-6);
        EXPECT_NEAR(CGAL::to_double(intersectionPoint.x()), 0.0, 1e-6);
    }
}