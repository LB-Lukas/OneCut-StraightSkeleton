#include <gtest/gtest.h>
#include "StraightSkeleton/perpendicularGraph.h"
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Surface_mesh.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Point_2 Point;
typedef CGAL::Surface_mesh<Point> PlanarGraph;

class PerpendicularGraphTest : public ::testing::Test {
protected:
    PlanarGraph cutGraph;
    PlanarGraph skeletonGraph;
    Geometry::PerpendicularGraph perpendicularGraph;

    void SetUp() override {
        auto v0 = cutGraph.add_vertex(Point(0, 0));
        auto v1 = cutGraph.add_vertex(Point(1, 0));
        auto v2 = cutGraph.add_vertex(Point(1, 1));
        auto v3 = cutGraph.add_vertex(Point(0, 1));
        cutGraph.add_face(v0, v1, v2, v3);

        auto v4 = skeletonGraph.add_vertex(Point(0.5, 0.5));
        auto v5 = skeletonGraph.add_vertex(Point(0.5, -1));
        auto v6 = skeletonGraph.add_vertex(Point(0.5, 1));
        auto v7 = skeletonGraph.add_vertex(Point(-1, 0.5));
        auto v8 = skeletonGraph.add_vertex(Point(1, 0.5));
        skeletonGraph.add_edge(v4, v5);
        skeletonGraph.add_edge(v4, v6);
        skeletonGraph.add_edge(v4, v7);
        skeletonGraph.add_edge(v4, v8);
    }

};


TEST_F(PerpendicularGraphTest, Constructor) {
    perpendicularGraph = Geometry::PerpendicularGraph(skeletonGraph, cutGraph);
}

TEST_F(PerpendicularGraphTest, Setup) {
    EXPECT_EQ(cutGraph.number_of_vertices(), 4);
    EXPECT_EQ(cutGraph.number_of_faces(), 1);
    EXPECT_EQ(cutGraph.number_of_edges(), 4);
    EXPECT_EQ(cutGraph.number_of_halfedges(), 8);

    EXPECT_EQ(skeletonGraph.number_of_vertices(), 5);
    EXPECT_EQ(skeletonGraph.number_of_faces(), 0);
    EXPECT_EQ(skeletonGraph.number_of_edges(), 4);
}

TEST_F(PerpendicularGraphTest, GetCutGraph) {
    // TODO
}

TEST_F(PerpendicularGraphTest, GetStraightSkeletonGraph) {
    // TODO
}

TEST_F(PerpendicularGraphTest, GetPerpendicularGraph) {
    // TODO
}

TEST_F(PerpendicularGraphTest, GetCompleteGraph) {
    // Expect Runtime error because the method is not yet implemented
    EXPECT_THROW(perpendicularGraph.getCompleteGraph(), std::runtime_error);
}

TEST_F(PerpendicularGraphTest, AddPerpenddiculars) {

}
