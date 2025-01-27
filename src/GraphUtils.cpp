#include "../include/straight_skeleton/GraphUtils.h"

namespace straight_skeleton {

bool GraphUtils::areGraphsEqual(const PlanarGraph& graph1, const PlanarGraph& graph2) {
    if (graph1.number_of_vertices() != graph2.number_of_vertices()) {
        return false;
    }

    if (graph1.number_of_faces() != graph2.number_of_faces()) {
        return false;
    }

    if (graph1.number_of_edges() != graph2.number_of_edges()) {
        return false;
    }

    // TODO: CHECK if there are more attributes that can be compared

    return true;
}

bool GraphUtils::isEdgeInGraph(const PlanarGraph& graph, const std::pair<Point, Point>& edge) {
    const Point& p1 = edge.first;
    const Point& p2 = edge.second;

    // Iterate through all the halfedges in the graph
    for (const auto& graph_edge : graph.edges()) {
        // Get the halfedge corresponding to the edge
        auto halfedge = graph.halfedge(graph_edge);

        // Get the two points that form the current edge from the halfedges
        Point edge_point1 = graph.point(graph.target(halfedge));
        Point edge_point2 = graph.point(graph.source(halfedge));

        // Check if the edge is either (p1, p2) or (p2, p1)
        if ((edge_point1 == p1 && edge_point2 == p2) || (edge_point1 == p2 && edge_point2 == p1)) {
            return true;
        }
    }
    return false;
}

std::vector<FaceIndex> GraphUtils::getFacesContainingVertex(const PlanarGraph& graph, const VertexIndex& vertex) {
    std::vector<FaceIndex> faces;
    if (graph.is_isolated(vertex)) {
        std::cout << "Vertex is isolated" << std::endl;
        throw std::invalid_argument("Vertex is isolated");
        return faces;
    }
    HalfedgeIndex halfedge = graph.halfedge(vertex);
    HalfedgeIndex start = halfedge;
    do {
        FaceIndex face = graph.face(halfedge);
        if (face != PlanarGraph::null_face()) {
            faces.push_back(face);
        }
        halfedge = graph.next_around_target(halfedge);
    } while (halfedge != start);

    return faces;
}

std::vector<EdgeIndex> GraphUtils::getEdgesOfFace(const PlanarGraph& graph, const FaceIndex& face) {
    std::vector<EdgeIndex> edges;
    for (auto halfedge : CGAL::halfedges_around_face(graph.halfedge(face), graph)) {
        edges.push_back(graph.edge(halfedge));
    }
    return edges;
}

std::vector<HalfedgeIndex> GraphUtils::getHalfedgesOfFace(const PlanarGraph& graph, const FaceIndex& face) {
    std::vector<HalfedgeIndex> halfedges;
    for (auto halfedge : CGAL::halfedges_around_face(graph.halfedge(face), graph)) {
        halfedges.push_back(halfedge);
    }
    return halfedges;
}

size_t GraphUtils::getDegree(const PlanarGraph& graph, const VertexIndex& vertex) {
    size_t deg = 0;
    for (auto e : graph.edges()) {
        auto h = graph.halfedge(e);
        auto s = graph.source(h);
        auto t = graph.target(h);
        // Count if vertex is either endpoint
        if (s == vertex || t == vertex) {
            deg++;
        }
    }
    return deg;
}


}  // namespace straight_skeleton