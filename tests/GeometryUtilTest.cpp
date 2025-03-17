#include <gtest/gtest.h>

#include "straight_skeleton/utils/GeometryUtil.h"

namespace straight_skeleton {

// counterclockwise
TEST(GeometryUtilTest, Rotate90) {
    Vector v(1, 0);
    Vector rotated = GeometryUtil::rotate90(v);
    EXPECT_EQ(rotated.x(), 0);
    EXPECT_EQ(rotated.y(), 1);
}

TEST(GeometryUtilTest, Normalize) {
    Vector v(3, 4);
    Vector normalized = GeometryUtil::normalize(v);
    EXPECT_NEAR(CGAL::to_double(normalized.x()), 0.6, 1e-6);
    EXPECT_NEAR(CGAL::to_double(normalized.y()), 0.8, 1e-6);

    Vector v2(42, 1);
    Vector normalized2 = GeometryUtil::normalize(v2);
    EXPECT_NEAR(CGAL::to_double(CGAL::squared_length(normalized2)), 1.0, 1e-6);
}

TEST(GeometryUtilTest, Cross) {
    Vector a(1, 0);
    Vector b(0, 1);
    K::FT cross_product = GeometryUtil::cross(a, b);
    EXPECT_EQ(cross_product, 1);
}

}