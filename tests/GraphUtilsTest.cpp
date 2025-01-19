#include <gtest/gtest.h>
#include "straight_skeleton/GraphUtils.h"
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Surface_mesh.h>
#include <memory>
#include <vector>

using namespace straight_skeleton;

class GraphUtilsTest : public ::testing::Test {
protected:
    PlanarGraph graph1;
    PlanarGraph graph2;
    FaceIndex face1;

    void SetUp() override {
        // Initialize graph1
        auto v0 = graph1.add_vertex(Point(0, 0));
        auto v1 = graph1.add_vertex(Point(1, 0));
        auto v2 = graph1.add_vertex(Point(0, 1));
        face1 = graph1.add_face(v0, v1, v2);

        // Initialize graph2
        auto v3 = graph2.add_vertex(Point(0, 0));
        auto v4 = graph2.add_vertex(Point(1, 0));
        auto v5 = graph2.add_vertex(Point(0, 1));
        graph2.add_face(v3, v4, v5);
    }
};

TEST_F(GraphUtilsTest, AreGraphsEqual) {
    EXPECT_TRUE(GraphUtils::areGraphsEqual(graph1, graph2));
}

TEST_F(GraphUtilsTest, IsEdgeInGraph) {
    auto iterator = graph1.vertices().begin();
    auto v0 = graph1.point(*iterator);
    iterator++;
    auto v1 = graph1.point(*iterator);
    std::pair<Point, Point> edge(v0, v1);
    EXPECT_TRUE(GraphUtils::isEdgeInGraph(graph1, edge));
}

TEST_F(GraphUtilsTest, GetFacesContainingVertex) {
    auto iterator = graph1.vertices().begin();
    VertexIndex vertex = *iterator; // Get the first vertex in the graph
    std::vector<FaceIndex> faces = GraphUtils::getFacesContainingVertex(graph1, vertex);

    // Assert that the vertex has faces
    ASSERT_FALSE(faces.empty());
}

TEST_F(GraphUtilsTest, GetEdgesOfFace) {
    auto face = *graph1.faces().begin(); // Get the first face in the graph
    std::vector<EdgeIndex> edges = GraphUtils::getEdgesOfFace(graph1, face);

    // Assert that the face has edges
    ASSERT_FALSE(edges.empty());
    
    // // Verify all edges belong to the specified face
    // for (const auto& edge : edges) {
    //     bool belongsToFace = false;
    //     for (auto faceEdge : graph1.edges_around_face(graph1.halfedge(face))) {
    //         if (edge == faceEdge) {
    //             belongsToFace = true;
    //             break;
    //         }
    //     }
    //     EXPECT_TRUE(belongsToFace);
    // }
}

TEST_F(GraphUtilsTest, GetDegree) {
    // auto iterator = graph1.vertices().begin();
    // VertexIndex vertex = *iterator; // Get the first vertex in the graph
    // size_t degree = GraphUtils::getDegree(graph1, vertex);

    // // Verify the degree matches the number of incident edges
    // size_t incidentEdges = 0;
    // for (auto edge : graph1.edges_around_target(graph1.halfedge(vertex))) { // no member
    //     incidentEdges++;
    // }
    // EXPECT_EQ(degree, incidentEdges);
}

