#include <gtest/gtest.h>
#include "StraightSkeleton/graphUtils.h"
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Surface_mesh.h>


typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Point_2 Point;
typedef CGAL::Surface_mesh<Point> PlanarGraph;

class GraphUtilsTest : public ::testing::Test {
protected:
    PlanarGraph graph1;
    PlanarGraph graph2;

    void SetUp() override {
        // Initialize graph1
        auto v0 = graph1.add_vertex(Point(0, 0));
        auto v1 = graph1.add_vertex(Point(1, 0));
        auto v2 = graph1.add_vertex(Point(0, 1));
        graph1.add_edge(v0, v1);
        graph1.add_edge(v1, v2);

        // Initialize graph2
        auto v3 = graph2.add_vertex(Point(0, 0));
        auto v4 = graph2.add_vertex(Point(1, 0));
        auto v5 = graph2.add_vertex(Point(0, 1));
        graph2.add_edge(v3, v4);
        graph2.add_edge(v4, v5);
    }
};

TEST_F(GraphUtilsTest, AreGraphsEqual) {
    EXPECT_TRUE(Geometry::GraphUtils::areGraphsEqual(graph1, graph2));
}

TEST_F(GraphUtilsTest, IsEdgeInGraph) {
    auto iterator = graph1.vertices().begin();
    auto v0 = graph1.point(*iterator);
    iterator++;
    auto v1 = graph1.point(*iterator);
    std::pair<Point, Point> edge(v0, v1);
    EXPECT_TRUE(Geometry::GraphUtils::isEdgeInGraph(graph1, edge));
}
