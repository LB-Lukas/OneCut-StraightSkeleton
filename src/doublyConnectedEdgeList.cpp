#include "../include/StraightSkeleton/doublyConnectedEdgeList.h"
#include <vector>

namespace Geometry {
    struct Vertex;
    struct HalfEdge;
    struct Face;

    Vertex::Vertex(int xCoord, int yCoord) : x(xCoord), y(yCoord) {};

    HalfEdge::HalfEdge() = default;

    Face::Face() = default;

    std::shared_ptr<Vertex> DCEL::createVertex(int x, int y) {
        auto vertex = std::make_shared<Vertex>(x, y);
        vertices.push_back(vertex);
        return vertex;
    }

    std::pair<std::shared_ptr<HalfEdge>, std::shared_ptr<HalfEdge>> DCEL::createEdge(const std::shared_ptr<Vertex>& v1,
        const std::shared_ptr<Vertex>& v2) {
        auto e1 = std::make_shared<HalfEdge>();
        auto e2 = std::make_shared<HalfEdge>();

        halfEdges.push_back(e1);
        halfEdges.push_back(e2);

        e1->origin = v1;
        e2->origin = v2;

        e1->twin = e2;
        e2->twin = e1;

        if (!v1->incidentEdge) {
            v1->incidentEdge = e1;
        }
        if (!v2->incidentEdge) {
            v2->incidentEdge = e2;
        }

        return std::make_pair(e1, e2);
    }

    void DCEL::makeCycle(const std::vector<std::shared_ptr<HalfEdge>>& edges, const std::shared_ptr<Face>& face) {
        int n = edges.size();
        if (n < 2) return; // No cycle to make if fewer than 2 edges

        for (int i = 0; i < n; i++) {
            // Set the next pointer of the current edge to the next edge in the list
            edges[i]->next = edges[(i + 1) % n];

            // Set the prev pointer of the next edge to the current edge
            edges[(i + 1) % n]->prev = edges[i];

            // Assign the face to the current edge
            edges[i]->face = face;
        }

        // If a face is provided, set the face's edge to the first half-edge in the cycle
        if (face) {
            face->edge = edges[0];
        }
    }

    std::shared_ptr<Face> DCEL::createFace() {
        auto face = std::make_shared<Face>();
        faces.push_back(face);
        return face;
    }
}
