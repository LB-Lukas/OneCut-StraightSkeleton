#include <gtest/gtest.h>
#include "StraightSkeleton/perpendicularGraph.h"
#include "StraightSkeleton/graphUtils.h"
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Surface_mesh.h>
#include <iostream>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Point_2 Point;
typedef K::Vector_2 Vector;
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

        auto center = skeletonGraph.add_vertex(Point(0.5, 0.5));
        auto v5 = skeletonGraph.add_vertex(Point(-1, -1));
        auto v6 = skeletonGraph.add_vertex(Point(-1, 2));
        auto v7 = skeletonGraph.add_vertex(Point(2, 2));
        auto v8 = skeletonGraph.add_vertex(Point(-1, 2));
        skeletonGraph.add_edge(center, v5);
        skeletonGraph.add_edge(center, v6);
        skeletonGraph.add_edge(center, v7);
        skeletonGraph.add_edge(center, v8);
    }
};

// Test to check if edges are created in cutGraph
TEST_F(PerpendicularGraphTest, TestCutGraphEdges) {
    auto iterator = cutGraph.vertices().begin();
    auto v0 = cutGraph.point(*iterator);
    iterator++;
    auto v1 = cutGraph.point(*iterator);
    iterator++;
    auto v2 = cutGraph.point(*iterator);
    iterator++;
    auto v3 = cutGraph.point(*iterator);

    // Check if edges exist for the face (v0->v1, v1->v2, v2->v3, v3->v0)
    EXPECT_TRUE(Geometry::GraphUtils::isEdgeInGraph(cutGraph, std::make_pair(v0, v1)));
    EXPECT_TRUE(Geometry::GraphUtils::isEdgeInGraph(cutGraph, std::make_pair(v1, v2)));
    EXPECT_TRUE(Geometry::GraphUtils::isEdgeInGraph(cutGraph, std::make_pair(v2, v3)));
    EXPECT_TRUE(Geometry::GraphUtils::isEdgeInGraph(cutGraph, std::make_pair(v3, v0)));
}

TEST_F(PerpendicularGraphTest, Constructor) {
    perpendicularGraph = Geometry::PerpendicularGraph(skeletonGraph, cutGraph);
    EXPECT_TRUE(Geometry::GraphUtils::areGraphsEqual(perpendicularGraph.getCutGraph(), cutGraph));
    EXPECT_TRUE(Geometry::GraphUtils::areGraphsEqual(perpendicularGraph.getStraightSkeletonGraph(), skeletonGraph));
}

TEST_F(PerpendicularGraphTest, Setup) {
    ASSERT_EQ(cutGraph.number_of_vertices(), 4);
    ASSERT_EQ(cutGraph.number_of_faces(), 1);
    ASSERT_EQ(cutGraph.number_of_edges(), 4);
    ASSERT_EQ(cutGraph.number_of_halfedges(), 8);

    ASSERT_EQ(skeletonGraph.number_of_vertices(), 5);
    ASSERT_EQ(skeletonGraph.number_of_faces(), 0);
    ASSERT_EQ(skeletonGraph.number_of_edges(), 4);

    std::cout << "Skeleton Graph: " << skeletonGraph.vertices().size() << std::endl;
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

TEST_F(PerpendicularGraphTest, AddPerpendiculars) {
    perpendicularGraph = Geometry::PerpendicularGraph(skeletonGraph, cutGraph);
    perpendicularGraph.addPerpendiculars();

    auto pGResult = perpendicularGraph.getPerpendicularGraph();

    // Check the number of vertices and edges in the perpendicular graph
    EXPECT_GT(pGResult.number_of_vertices(), 0) << "Perpendicular graph should have vertices.";
    EXPECT_GT(pGResult.number_of_edges(), 0) << "Perpendicular graph should have edges.";


    // TODO: write better Test
    // Verify perpendicularity
    //for (auto edge : cutGraph.edges()) {
    auto edge = *cutGraph.edges().begin();
    auto halfedge = cutGraph.halfedge(edge);
    auto cutEdgeSource = cutGraph.point(cutGraph.source(halfedge));
    auto cutEdgeTarget = cutGraph.point(cutGraph.target(halfedge));

    // Vector representation of the cut edge
    auto cutVector = Vector(cutEdgeTarget.x() - cutEdgeSource.x(),
                                    cutEdgeTarget.y() - cutEdgeSource.y());

    //for (auto perpendicularEdge : pGResult.edges()) {
    auto perpendicularEdge = *pGResult.edges().begin();
    auto perpHalfedge = pGResult.halfedge(perpendicularEdge);
    auto perpSource = pGResult.point(pGResult.source(perpHalfedge));
    auto perpTarget = pGResult.point(pGResult.target(perpHalfedge));

    // Vector representation of the perpendicular edge
    auto perpVector = Vector(perpTarget.x() - perpSource.x(),
                                    perpTarget.y() - perpSource.y());

    // Check if the vectors are perpendicular (dot product = 0)
    double dotProduct = CGAL::to_double(cutVector.x() * perpVector.x() + cutVector.y() * perpVector.y());
    EXPECT_NEAR(dotProduct, 0.0, 1e-6) << "Edge is not perpendicular!";
        //}
    //}
}


TEST_F(PerpendicularGraphTest, CalculatePerpendicularBisector) {
    Point p(0, 0);
    Point first(1, 0);
    Point second(0, 1);
    auto perpendicular = perpendicularGraph.calculatePerpendicularBisector(p, std::make_pair(first, second));
    EXPECT_EQ(perpendicular.first, Point(0, 0));
    EXPECT_EQ(perpendicular.second, Point(0.5, 0.5));
}

TEST_F(PerpendicularGraphTest, CalculatePerpendicularBisectorHorizontalEdge) {
    Point p(0, 1);
    Point first(-1, 0);
    Point second(1, 0);
    auto perpendicular = perpendicularGraph.calculatePerpendicularBisector(p, std::make_pair(first, second));
    EXPECT_EQ(perpendicular.first, Point(0, 1)); // Origin remains the same
    EXPECT_EQ(perpendicular.second, Point(0, 0)); // Projection is on the line y = 0
}

TEST_F(PerpendicularGraphTest, CalculatePerpendicularBisectorVerticalEdge) {
    Point p(1, 0);
    Point first(0, -1);
    Point second(0, 1);
    auto perpendicular = perpendicularGraph.calculatePerpendicularBisector(p, std::make_pair(first, second));
    EXPECT_EQ(perpendicular.first, Point(1, 0)); // Origin remains the same
    EXPECT_EQ(perpendicular.second, Point(0, 0)); // Projection is on the line x = 0
}

TEST_F(PerpendicularGraphTest, CalculatePerpendicularBisectorDiagonalEdge) {
    Point p(1, 1);
    Point first(0, 0);
    Point second(2, 2);
    auto perpendicular = perpendicularGraph.calculatePerpendicularBisector(p, std::make_pair(first, second));
    EXPECT_EQ(perpendicular.first, Point(1, 1)); // Origin remains the same
    EXPECT_EQ(perpendicular.second, Point(1, 1)); // Projection lies on the line itself (already on the edge)
}

TEST_F(PerpendicularGraphTest, CalculatePerpendicularBisectorPointOnEdge) {
    Point p(0.5, 0.5); // Point lies on the edge
    Point first(0, 0);
    Point second(1, 1);
    auto perpendicular = perpendicularGraph.calculatePerpendicularBisector(p, std::make_pair(first, second));
    EXPECT_EQ(perpendicular.first, Point(0.5, 0.5)); // Origin remains the same
    EXPECT_EQ(perpendicular.second, Point(0.5, 0.5)); // Projection is the point itself
}

TEST_F(PerpendicularGraphTest, CalculatePerpendicularBisectorPointOutsideEdgeBounds) {
    Point p(3, 3);
    Point first(0, 0);
    Point second(1, 1);
    auto perpendicular = perpendicularGraph.calculatePerpendicularBisector(p, std::make_pair(first, second));
    EXPECT_EQ(perpendicular.first, Point(3, 3));
    EXPECT_EQ(perpendicular.second, Point(3, 3)); 
}

TEST_F(PerpendicularGraphTest, CalculatePerpendicularBisectorDegenerateEdge) {
    Point p(1, 1);
    Point first(0, 0);
    Point second(0, 0); // Degenerate case: edge reduced to a point
    auto perpendicular = perpendicularGraph.calculatePerpendicularBisector(p, std::make_pair(first, second));
    EXPECT_EQ(perpendicular.first, Point(1, 1)); // Origin remains the same
    EXPECT_EQ(perpendicular.second, Point(0, 0)); // Projection is the degenerate edge point
}

