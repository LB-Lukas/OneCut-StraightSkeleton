#include <gtest/gtest.h>

#include <vector>

#include "OneCut/SkeletonBuilder.h"

namespace SkeletonConstruction {

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

TEST_F(SkeletonBuilderTest, BuildSkeletonWithTriangle) {
    SkeletonBuilder builder(triangle);
    EXPECT_NO_THROW({
        auto skeleton = builder.buildSkeleton();
        EXPECT_GE(skeleton.faceCount(), 6);
    });
}

TEST_F(SkeletonBuilderTest, BuildSkeletonWithRectangle) {
    SkeletonBuilder builder(rectangle);
    EXPECT_NO_THROW({
        auto skeleton = builder.buildSkeleton();
        EXPECT_GE(skeleton.faceCount(), 8);
    });
}

TEST_F(SkeletonBuilderTest, BuildSkeletonWithConcavePolygon) {
    SkeletonBuilder builder(concave);
    EXPECT_NO_THROW({
        auto skeleton = builder.buildSkeleton();
        // Check that the skeleton is successfully created from a concave polygon.
    });
}

TEST_F(SkeletonBuilderTest, BuildSkeletonWithEmptyPolygon) {
    std::vector<Point> empty;
    EXPECT_ANY_THROW({
        SkeletonBuilder builder(empty);
        builder.buildSkeleton();
    });
}

TEST_F(SkeletonBuilderTest, BuildSkeletonWithDegeneratePolygon) {
    std::vector<Point> degenerate = {Point(100, 100), Point(200, 200), Point(300, 300)};
    EXPECT_ANY_THROW({
        SkeletonBuilder builder(degenerate);
        builder.buildSkeleton();
    });
}

}  // namespace SkeletonConstruction