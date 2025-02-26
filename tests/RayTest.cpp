#include <gtest/gtest.h>

#include "straight_skeleton/Ray.h"

namespace straight_skeleton {

class RayTest : public ::testing::Test {
   protected:
    Point3D a;
    Ray ray;

    void SetUp() override {
        a = Point3D(0, 0, 0);
        ray = Ray(a, Direction3D(1, 0, 0), 2.0);
    }
};

TEST_F(RayTest, DefaultConstructor) {
    Ray defaultRay;
    EXPECT_EQ(defaultRay.getOrigin(), Point3D(0,0,0));
    EXPECT_EQ(defaultRay.getDirection(), Direction3D(1,0,0));
    EXPECT_EQ(defaultRay.getTStart(), 0.0);
}

TEST_F(RayTest, ParameterizedConstructor) {
    Point3D origin(1, 2, 3);
    Direction3D direction(0, 1, 0);
    double tStart = 5.0;
    Ray pRay(origin, direction, tStart);
    EXPECT_EQ(pRay.getOrigin(), origin);
    EXPECT_EQ(pRay.getDirection(), direction);
    EXPECT_EQ(pRay.getTStart(), tStart);
}

TEST_F(RayTest, PositionAt) {
    EXPECT_EQ(ray.positionAt(2.0), Point3D(0, 0, 0));
    EXPECT_EQ(ray.positionAt(3.0), Point3D(1, 0, 0));
    EXPECT_EQ(ray.positionAt(0.0), Point3D(-2, 0, 0));
    EXPECT_EQ(ray.positionAt(-1.0), Point3D(-3, 0, 0));
}

TEST_F(RayTest, EqualityOperator) {
    Ray a(Point3D(0, 0, 0), Direction3D(1, 0, 0), 2.0);
    Ray b(Point3D(0, 0, 0), Direction3D(1, 0, 0), 2.0);
    Ray c(Point3D(0, 0, 0), Direction3D(-1, 0, 0), 2.0);
    Ray d(Point3D(0, 0, 0), Direction3D(1, 0, 0), 3.0);
    Ray e(Point3D(1, 0, 0), Direction3D(1, 0, 0), 2.0);

    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a == c);
    EXPECT_FALSE(a == d);
    EXPECT_FALSE(a == e);
    EXPECT_FALSE(b == c);
    EXPECT_FALSE(b == d);
}

}  // namespace straight_skeleton