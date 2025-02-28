#include <gtest/gtest.h>

#include "straight_skeleton/NaiveRayShootingQuery.h"
#include "straight_skeleton/Ray.h"
#include "straight_skeleton/StraightSkeletonTypes.h"
#include "straight_skeleton/Triangle.h"

namespace straight_skeleton {

class NaiveRayShootingQueryTest : public ::testing::Test {
   protected:
    NaiveRayShootingQuery ds;
    Triangle t1;
    Triangle t2;
    Triangle t3;
    Ray queryRay;
    void SetUp() override {
        t1 = Triangle(Point3D(0, 0, 1), Point3D(1, 1, 1), Point3D(-2, 1, 1));
        t2 = Triangle(Point3D(0, 0, 0.5), Point3D(1, 1, 0.5), Point3D(-2, 1, 0.5));
        t3 = Triangle(Point3D(0, 0, 2), Point3D(1, 1, 2), Point3D(-2, 1, 2));
        queryRay = Ray(Point3D(0, 0, 0), Direction3D(0, 0, 1));
    }
};

TEST_F(NaiveRayShootingQueryTest, TestAddTriangle) {
    ds.addTriangle(t1);
    ds.addTriangle(t2);
    EXPECT_EQ(ds.findRayIntersection(queryRay).intersectedTriangle, t2);
    ds.removeTriangle(t2);
    EXPECT_EQ(ds.findRayIntersection(queryRay).intersectedTriangle, t1);
}

TEST_F(NaiveRayShootingQueryTest, TestRemoveTriangle) {
    ds.addTriangle(t1);
    ds.addTriangle(t2);
    ds.removeTriangle(t1);
    EXPECT_EQ(ds.findRayIntersection(queryRay).intersectedTriangle, t2);
}

TEST_F(NaiveRayShootingQueryTest, NoTriangleAdded) {
    NaiveRayShootingQuery rsq;
    Ray queryRay(Point3D(0, 0, 0), Direction3D(0, 0, 1));
    EXPECT_FALSE(rsq.findRayIntersection(queryRay).hasIntersection);
}

TEST_F(NaiveRayShootingQueryTest, NoIntersection) {
    NaiveRayShootingQuery rsq;
    Triangle triangle(Point3D(-0.5, 0, 0), Point3D(1, 1, 0), Point3D(-2, 1, 0));
    rsq.addTriangle(triangle);
    EXPECT_FALSE(rsq.findRayIntersection(queryRay).hasIntersection);
}

}  // namespace straight_skeleton