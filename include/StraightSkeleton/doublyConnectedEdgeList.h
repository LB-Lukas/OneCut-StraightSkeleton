// Created by user on 11/3/24.

#ifndef DCEL_H
#define DCEL_H

#include <memory>
#include <vector>

namespace Geometry {

    struct Vertex;
    struct HalfEdge;
    struct Face;

    struct Vertex {
        int x, y; // Coordinates for the vertex
        std::shared_ptr<HalfEdge> incidentEdge; // Any half-edge originating from this vertex

        Vertex(int xCoord, int yCoord);
    };

    struct HalfEdge {
        std::shared_ptr<Vertex> origin; // Origin vertex of the half-edge
        std::shared_ptr<HalfEdge> twin; // Twin half-edge
        std::shared_ptr<HalfEdge> next; // Next half-edge around the face
        std::shared_ptr<HalfEdge> prev; // Previous half-edge around the face
        std::shared_ptr<Face> face;     // Face this half-edge borders

        HalfEdge();
    };

    struct Face {
        std::shared_ptr<HalfEdge> edge; // A half-edge bordering this face

        Face();
    };

    class DCEL {
    public:
        std::vector<std::shared_ptr<Vertex>> vertices;
        std::vector<std::shared_ptr<HalfEdge>> halfEdges;
        std::vector<std::shared_ptr<Face>> faces;

        std::shared_ptr<Vertex> createVertex(int x, int y);

        std::pair<std::shared_ptr<HalfEdge>, std::shared_ptr<HalfEdge>> createEdge(const std::shared_ptr<Vertex>& v1,
            const std::shared_ptr<Vertex>& v2);

        std::shared_ptr<Face> createFace();

        void makeCycle(const std::vector<std::shared_ptr<HalfEdge>>& edges, const std::shared_ptr<Face>& face);
    };

}

#endif // DCEL_H
