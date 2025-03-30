#include "OneCut/PerpendicularFinder.h"

#include <CGAL/number_utils.h>

#include <iostream>

namespace OneCut {

PerpendicularFinder::PerpendicularFinder(const IStraightSkeleton& skeleton) : skeleton(skeleton) {}

std::vector<PerpChain> PerpendicularFinder::findPerpendiculars() {
    std::vector<PerpChain> perpendicularChains;

    int faceCount = skeleton.faceCount();
    for (int faceIdx = 0; faceIdx < faceCount; faceIdx++) {
        const ISkeletonFace& face = skeleton.face(faceIdx);

        // Vertex 0 and 1 form the cut edge
        for (int vertexIdx = 2; vertexIdx < face.vertexCount(); vertexIdx++) {
            if (face.adjacentFaceIndex(vertexIdx) == -1 || face.adjacentFaceIndex(vertexIdx - 1) == -1) {
                continue;  // Skip if no adjacent faces
            }

            PerpChain chain;
            Point currentVertex = face.vertex(vertexIdx);

            // Skip vertices outside the paper bounds
            if (currentVertex.x() < 0 || currentVertex.y() < 0 || currentVertex.x() > PAPER_BORDER_X ||
                currentVertex.y() > PAPER_BORDER_Y) {
                continue;
            }

            int currentFaceIdx = faceIdx;
            int currentEdgeIdx = vertexIdx;

            for (int iteration = 0; iteration < MAX_ITERATIONS; iteration++) {
                PerpendicularHit perpHit =
                    computePerpendicularIntersection(currentVertex, skeleton.face(currentFaceIdx), currentEdgeIdx);

                if (!perpHit.isValid) {
                    break;
                }

                chain.push_back({currentVertex, perpHit.intersection, currentFaceIdx});

                int adjacentFaceIdx = skeleton.face(currentFaceIdx).adjacentFaceIndex(perpHit.edgeIndex);
                if (adjacentFaceIdx == -1) {
                    break;
                }

                Point adjacentEdgeStartVertex =
                    skeleton.face(currentFaceIdx)
                        .vertex((perpHit.edgeIndex + 1) % skeleton.face(currentFaceIdx).vertexCount());

                int adjacentEdgeIdx = findEdgeIndex(skeleton.face(adjacentFaceIdx), adjacentEdgeStartVertex);
                if (adjacentEdgeIdx < 0) {
                    break;
                }

                currentVertex = perpHit.intersection;
                currentFaceIdx = adjacentFaceIdx;
                currentEdgeIdx = adjacentEdgeIdx;
            }

            if (!chain.empty()) {
                perpendicularChains.push_back(chain);
            }
        }
    }
    return perpendicularChains;
}

PerpendicularHit PerpendicularFinder::computePerpendicularIntersection(const Point& vertex, const ISkeletonFace& face,
                                                                       int edgeIndex) {
    if (face.vertexCount() < 2) {
        return {false, Point(0, 0), -1};
    }

    Vector cutEdgeVector = face.vertex(1) - face.vertex(0);
    Vector perpendicularDir = GeometryUtil::rotate90(cutEdgeVector);
    perpendicularDir = GeometryUtil::normalize(perpendicularDir);

    Point edgeStart = face.vertex(edgeIndex);
    Point edgeEnd = face.vertex((edgeIndex + 1) % face.vertexCount());
    Vector edgeVector = edgeEnd - edgeStart;

    CGAL::Orientation orientation = CGAL::orientation(Point(0, 0), Point(edgeVector.x(), edgeVector.y()),
                                                      Point(perpendicularDir.x(), perpendicularDir.y()));
    if (orientation == CGAL::RIGHT_TURN) {
        perpendicularDir = -perpendicularDir;
    }

    double minRayParam = std::numeric_limits<double>::infinity();
    int closestEdgeIndex = -1;
    Point closestIntersection;

    for (int e = 0; e < face.vertexCount(); e++) {
        if (e == edgeIndex) {
            continue;
        }

        Point segmentStart = face.vertex(e);
        Point segmentEnd = face.vertex((e + 1) % face.vertexCount());

        auto intersection = IntersectionUtil::intersectRaySegment(vertex, perpendicularDir, segmentStart, segmentEnd);
        if (intersection.valid && intersection.rayParam > 0 && intersection.rayParam < minRayParam &&
            intersection.segmentParam >= 0 && intersection.segmentParam <= 1) {
            minRayParam = intersection.rayParam;
            closestEdgeIndex = e;
            closestIntersection =
                Point(vertex.x() + perpendicularDir.x() * minRayParam, vertex.y() + perpendicularDir.y() * minRayParam);
        }
    }

    if (closestEdgeIndex < 0) {
        return {false, Point(0, 0), -1};
    }

    return {true, closestIntersection, closestEdgeIndex};
}

int PerpendicularFinder::findEdgeIndex(const ISkeletonFace& face, const Point& startPoint) const {
    for (int i = 0; i < face.vertexCount(); i++) {
        if (face.vertex(i) == startPoint) {
            return i;
        }
    }
    return -1;
}

}  // namespace OneCut
