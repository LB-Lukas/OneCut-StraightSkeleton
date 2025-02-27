#include <gtest/gtest.h>

#include "straight_skeleton/NaiveLowestIntersectionQuery.h"
#include "straight_skeleton/Ray.h"
#include "straight_skeleton/StraightSkeletonTypes.h"
#include "straight_skeleton/Triangle.h"

namespace straight_skeleton {

class NaiveLowestIntersectionQueryTest : public ::testing::Test {
   protected:
    NaiveLowestIntersectionQuery ds;
    Ray r1;
    Ray r2;
    Triangle queryTriangle;
    void SetUp() override {
        r1 = Ray(Point3D(0, 0, 1), Direction3D(0, 0, 1));
        r2 = Ray(Point3D(0, 0, 0), Direction3D(0, 0, 1));
        queryTriangle = Triangle(Point3D(-5, 5, 2), Point3D(5, 5, 2), Point3D(5, -5, 2));
    }
};

TEST_F(NaiveLowestIntersectionQueryTest, TestAddRay) {
    ds.addRay(r1);
    ds.addRay(r2);
    EXPECT_EQ(ds.findLowestIntersection(queryTriangle).intersectedRay, r1);
    ds.removeRay(r1);
    EXPECT_EQ(ds.findLowestIntersection(queryTriangle).intersectedRay, r2);
}

TEST_F(NaiveLowestIntersectionQueryTest, TestRemoveRay) {
    ds.addRay(r1);
    ds.addRay(r2);
    ds.removeRay(r1);
    EXPECT_EQ(ds.findLowestIntersection(queryTriangle).intersectedRay, r2);
}

TEST_F(NaiveLowestIntersectionQueryTest, NoRayAdded) {
    EXPECT_FALSE(ds.findLowestIntersection(queryTriangle).hasIntersection);
}

TEST_F(NaiveLowestIntersectionQueryTest, NoIntersection) {
    NaiveLowestIntersectionQuery lowestIntersection;
    Ray ray(Point3D(0, 0, 0), Direction3D(0, 0, -1));
    lowestIntersection.addRay(ray);
    EXPECT_FALSE(lowestIntersection.findLowestIntersection(queryTriangle).hasIntersection);
}

}  // namespace straight_skeleton