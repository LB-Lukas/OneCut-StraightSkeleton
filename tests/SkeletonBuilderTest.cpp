#include <gtest/gtest.h>

#include <vector>

#include "straight_skeleton/SkeletonBuilder.h"

namespace TestSkeleton {

class SkeletonBuilderTest : public ::testing::Test {
   protected:
    std::vector<Point> square;
    std::vector<Point> triangle;
    std::vector<Point> rectangle;
    std::vector<Point> concave;

    void SetUp() override {
        square = {Point(100, 100), Point(500, 100), Point(500, 500), Point(100, 500)};
        triangle = {Point(100, 100), Point(500, 100), Point(300, 500)};
        rectangle = {Point(100, 100), Point(500, 100), Point(500, 400), Point(100, 400)};
        concave = {Point(100, 100), Point(500, 100), Point(500, 500), Point(300, 300), Point(100, 500)};
    }
};

TEST_F(SkeletonBuilderTest, ConstructorWithValidPolygon) {
    SkeletonBuilder builder(square);
    EXPECT_NO_THROW(builder.buildSkeleton());
}

}  // namespace TestSkeleton