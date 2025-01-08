#include <gtest/gtest.h>
#include "StraightSkeleton/perpendicularGraph.h"
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Surface_mesh.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 Point;
typedef CGAL::Surface_mesh<Point> PlanarGraph;


TEST(PerpendicularGraphTest, Constructor) {
    PlanarGraph skeletonGraph;
    PlanarGraph cutGraph;

    auto v0 = cutGraph.add_vertex(Point(0, 0));
    auto v1 = cutGraph.add_vertex(Point(1, 0));
    auto v2 = cutGraph.add_vertex(Point(1, 1));
    cutGraph.add_face(v0, v1, v2);

    EXPECT_EQ(cutGraph.number_of_vertices(), 3);
    EXPECT_EQ(cutGraph.number_of_faces(), 1);
    EXPECT_EQ(cutGraph.number_of_edges(), 3);
}

