#include <gtest/gtest.h>
#include "StraightSkeleton/folding.h"

TEST (FoldingTest, Test1) {
    Geometry::Folding folding;
    ASSERT_EQ(folding.getVertices().size(), 0);
}