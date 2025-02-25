#include <gtest/gtest.h>

#include "straight_skeleton/Triangle.h"

namespace straight_skeleton {

class TriangleTest : public testing::Test {
   protected:
    Point3D a, b, c;
    Triangle triangle;
    TrianglePtr trianglePtr;

    void SetUp() override {
        a = Point3D(2, 0, 0);
        b = Point3D(0, 0, 0);
        c = Point3D(1, 1, 0);
        triangle = Triangle(a, b, c);
        trianglePtr = std::make_shared<Triangle>(triangle);
    }
};

TEST_F(TriangleTest, Constructor) {
    EXPECT_EQ(triangle.getBaseA(), a);
    EXPECT_EQ(triangle.getBaseB(), b);

    EXPECT_EQ(triangle.getBase(), std::make_pair(a, b));
    EXPECT_EQ(triangle.getApex(), c);

    EXPECT_EQ(triangle.getClockwiseNeighbour(), nullptr);
    EXPECT_EQ(triangle.getCounterclockwiseNeighbour(), nullptr);
}

TEST_F(TriangleTest, Neighbours) {
    TrianglePtr neighbourA = std::make_shared<Triangle>(Point3D(0, 0, 0), Point3D(0, 2, 0), Point3D(1, 1, 0));
    neighbourA->setCounterclockwiseNeighbour(trianglePtr);

    TrianglePtr neighbourB = std::make_shared<Triangle>(Point3D(2, 2, 0), Point3D(2, 0, 0), Point3D(1, 1, 0));
    neighbourB->setClockwiseNeighbour(trianglePtr);

    triangle.setClockwiseNeighbour(neighbourA);
    triangle.setCounterclockwiseNeighbour(neighbourB);

    EXPECT_EQ(triangle.getClockwiseNeighbour(), neighbourA);
    EXPECT_EQ(triangle.getCounterclockwiseNeighbour(), neighbourB);

    EXPECT_EQ(triangle.getClockwiseNeighbour()->getBase(), std::make_pair(Point3D(0, 0, 0), Point3D(0, 2, 0)));
    EXPECT_EQ(triangle.getCounterclockwiseNeighbour()->getBase(), std::make_pair(Point3D(2, 2, 0), Point3D(2, 0, 0)));

    EXPECT_EQ(triangle.getClockwiseNeighbour()->getClockwiseNeighbour(), nullptr);
    EXPECT_EQ(triangle.getClockwiseNeighbour()->getCounterclockwiseNeighbour(), trianglePtr);

    EXPECT_THROW(triangle.setClockwiseNeighbour(neighbourB), std::runtime_error);
}

TEST_F(TriangleTest, EqualityOperator) {
    Triangle sameTriangle(a, b, c);
    EXPECT_EQ(triangle, sameTriangle);

    Triangle differentApex(a, b, Point3D(1, 2, 0));
    EXPECT_NE(triangle, differentApex);

    Triangle swappedBase(b, a, c);
    EXPECT_NE(triangle, swappedBase);
}

TEST_F(TriangleTest, ResetNeighboursToNull) {
    TrianglePtr neighbour = std::make_shared<Triangle>(b, a, c); // swapped base
    triangle.setClockwiseNeighbour(neighbour);
    triangle.setClockwiseNeighbour(nullptr);
    EXPECT_EQ(triangle.getClockwiseNeighbour(), nullptr);
}

TEST_F(TriangleTest, NeighbourMismatch) {
    TrianglePtr neighbourA = std::make_shared<Triangle>(Point3D(1, 0, 0), Point3D(0, 2, 0), Point3D(1, 1, 0));
    EXPECT_THROW(triangle.setClockwiseNeighbour(neighbourA), std::runtime_error);
}

}  // namespace straight_skeleton