#include "../include/StraightSkeleton/doublyConnectedEdgeList.h"
#include <vector>

namespace Geometry {
    struct Vertex;
    struct HalfEdge;
    struct Face;

    Vertex::Vertex(const int& xCoord, const int& yCoord) : x(xCoord), y(yCoord) {};
    Vertex::Vertex(const std::shared_ptr<Vertex>& vertex1, const std::shared_ptr<Vertex>& vertex2)
    : x((vertex1->x + vertex2->x) / 2), y((vertex1->y + vertex2->y) / 2) {};

    HalfEdge::HalfEdge() = default;

    Face::Face() = default;

    DCEL::DCEL(const std::vector<std::shared_ptr<Vertex>>& vertices) {
        this->vertices = vertices;

        auto inner_face = std::make_shared<Face>();
        auto outer_face = std::make_shared<Face>();

        auto n = vertices.size();

        for(int i = 0; i < n; i++) {
            this->createEdge(vertices[i], vertices[i + 1]);
        }
        this->createEdge(vertices[n], vertices[0]);

        auto cw_edges = std::vector<std::shared_ptr<HalfEdge>>();
        auto ccw_edges = std::vector<std::shared_ptr<HalfEdge>>();

        for(int i = 0; i < this->halfEdges.size(); i+=2) {
            cw_edges.push_back(this->halfEdges[i]);
            ccw_edges.push_back(this->halfEdges[i+1]);
        }

        this->makeCycle(cw_edges, inner_face);
        this->makeCycle(ccw_edges, outer_face);

        this->faces.push_back(inner_face);
        this->faces.push_back(outer_face);
    }

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

    std::shared_ptr<Face> DCEL::createFace() {
        auto face = std::make_shared<Face>();
        faces.push_back(face);
        return face;
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

    void DCEL::printDCEL() const {
    std::cout << "DCEL Structure:" << std::endl;

    // Print vertices and their incident edges
    std::cout << "\nVertices:" << std::endl;
    for (const auto& vertex : vertices) {
        std::cout << "Vertex (" << vertex->x << ", " << vertex->y << ")";
        if (vertex->incidentEdge) {
            std::cout << " -> Incident Edge Originating at this Vertex.";
        }
        std::cout << std::endl;
    }

    // Print half-edges and their properties
    std::cout << "\nHalfEdges:" << std::endl;
    std::unordered_map<std::shared_ptr<HalfEdge>, int> edgeIndices;
    for (size_t i = 0; i < halfEdges.size(); ++i) {
        edgeIndices[halfEdges[i]] = i;
    }

    for (const auto& edge : halfEdges) {
        std::cout << "HalfEdge " << edgeIndices[edge] << ":" << std::endl;
        if (edge->origin) {
            std::cout << "  Origin: (" << edge->origin->x << ", " << edge->origin->y << ")" << std::endl;
        }
        if (edge->twin) {
            std::cout << "  Twin Edge ID: " << edgeIndices[edge->twin] << std::endl;
        }
        if (edge->next) {
            std::cout << "  Next Edge ID: " << edgeIndices[edge->next] << std::endl;
        }
        if (edge->prev) {
            std::cout << "  Prev Edge ID: " << edgeIndices[edge->prev] << std::endl;
        }
        if (edge->face) {
            std::cout << "  Face ID: " << edgeIndices[edge->face->edge] << " (First half-edge of Face)" << std::endl;
        }
    }

    // Print faces and their boundary edges
    std::cout << "\nFaces:" << std::endl;
    for (size_t i = 0; i < faces.size(); ++i) {
        std::cout << "Face " << i << ":" << std::endl;
        if (faces[i]->edge) {
            std::cout << "  Boundary HalfEdge ID: " << edgeIndices[faces[i]->edge] << std::endl;

            // Loop through the half-edges around the face to show the cycle
            auto startEdge = faces[i]->edge;
            auto currentEdge = startEdge;
            do {
                std::cout << "    HalfEdge ID: " << edgeIndices[currentEdge] << " -> ";
                currentEdge = currentEdge->next;
            } while (currentEdge != startEdge && currentEdge != nullptr);
            std::cout << " (Back to start)" << std::endl;
        }
    }
    std::cout << std::endl;
}

}
