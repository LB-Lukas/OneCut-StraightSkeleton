#include <gtest/gtest.h>

#include "OneCut/utils/GeometryUtil.h"

namespace OneCut {

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

TEST(GeometryUtilTest, Rotate90_NonAxisAligned) {
    Vector v(1, 1);
    Vector rotated = GeometryUtil::rotate90(v);
    EXPECT_EQ(rotated.x(), -1);
    EXPECT_EQ(rotated.y(), 1);
}

TEST(GeometryUtilTest, Rotate90_NegativeComponents) {
    Vector v(0, -2);
    Vector rotated = GeometryUtil::rotate90(v);
    EXPECT_EQ(rotated.x(), 2);
    EXPECT_EQ(rotated.y(), 0);
}

TEST(GeometryUtilTest, NormalizeNegativeVector) {
    Vector v(-3, 4);
    Vector normalized = GeometryUtil::normalize(v);
    EXPECT_NEAR(CGAL::to_double(normalized.x()), -0.6, 1e-6);
    EXPECT_NEAR(CGAL::to_double(normalized.y()), 0.8, 1e-6);
}

TEST(GeometryUtilTest, NormalizeUnitVector) {
    Vector v(0, 1);
    Vector normalized = GeometryUtil::normalize(v);
    EXPECT_EQ(normalized.x(), 0);
    EXPECT_EQ(normalized.y(), 1);
}

TEST(GeometryUtilTest, CrossProductNegativeResult) {
    Vector a(2, 3);
    Vector b(4, 5);
    K::FT result = GeometryUtil::cross(a, b);
    EXPECT_EQ(result, -2);
}

TEST(GeometryUtilTest, CrossProductZeroResult) {
    Vector a(3, 4);
    K::FT result = GeometryUtil::cross(a, a);  // Cross with self
    EXPECT_EQ(result, 0);
}

TEST(GeometryUtilTest, CrossProductWithZeroVector) {
    Vector a(0, 0);
    Vector b(1, 2);
    K::FT result = GeometryUtil::cross(a, b);
    EXPECT_EQ(result, 0);
}

TEST(GeometryUtilTest, ScalarProjectionColinearVectors) {
    Vector a(2, 0);
    Vector b(3, 0);
    K::FT result = GeometryUtil::scalarProjection(a, b);
    EXPECT_NEAR(CGAL::to_double(result), 2.0, 1e-6);
}

TEST(GeometryUtilTest, ScalarProjectionPerpendicular) {
    Vector a(0, 5);
    Vector b(1, 0);
    K::FT result = GeometryUtil::scalarProjection(a, b);
    EXPECT_NEAR(CGAL::to_double(result), 0.0, 1e-6);
}

TEST(GeometryUtilTest, ScalarProjectionObtuseAngle) {
    Vector a(1, 1);
    Vector b(-1, 0);
    K::FT result = GeometryUtil::scalarProjection(a, b);
    EXPECT_NEAR(CGAL::to_double(result), -1.0, 1e-6);
}

TEST(GeometryUtilTest, ScalarProjectionGeneralCase) {
    Vector a(3, 4);
    Vector b(0, 2);
    K::FT result = GeometryUtil::scalarProjection(a, b);
    EXPECT_NEAR(CGAL::to_double(result), 4.0, 1e-6);  // (3*0 + 4*2)/2 = 8/2 = 4
}

TEST(GeometryUtilTest, ScalarProjectionLargeValues) {
    Vector a(5, 5);
    Vector b(3, 4);
    K::FT result = GeometryUtil::scalarProjection(a, b);
    EXPECT_NEAR(CGAL::to_double(result), 7.0, 1e-6);  // (15+20)/5 = 35/5 = 7
}

TEST(GeometryUtilTest, ScalarProjectionPrecision) {
    Vector a(2, 3);
    Vector b(1, 1);
    K::FT result = GeometryUtil::scalarProjection(a, b);
    double expected = 5.0 / std::sqrt(2.0);  // ~3.5355339
    EXPECT_NEAR(CGAL::to_double(result), expected, 1e-6);
}


}