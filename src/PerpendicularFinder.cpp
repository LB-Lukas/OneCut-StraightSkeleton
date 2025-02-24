#include "straight_skeleton/PerpendicularFinder.h"

#include <CGAL/number_utils.h>  // for CGAL::to_double
#include <iostream>  // for std::cout

namespace straight_skeleton {

PerpendicularFinder::PerpendicularFinder(const IStraightSkeleton& skeleton) : skeleton(skeleton) {}

std::vector<PerpChain> PerpendicularFinder::findPerpendiculars() {
    std::vector<PerpChain> result;  // result is a vector of a vector of PerpSegment objects

    int faceCount = skeleton.faceCount();
    std::cout << "Face count: " << faceCount << std::endl;
    for (int f = 0; f < faceCount; f++) {
        const ISkeletonFace& face = skeleton.face(f);
        std::cout << "Processing face: " << f << std::endl;
        std::cout << "FACE: " << face << std::endl;

        // Vertex 0 and 1 form the cut Edge
        for (int v = 2; v < face.vertexCount(); v++) {
            std::cout << "Processing vertex: " << v << std::endl;
            if (face.adjacentFaceIndex(v) == -1) {
                std::cout << "Skipping vertex[case 1]: " << v << " due to missing adjacent face at index " << v << std::endl;
                continue;
            }
            if (face.adjacentFaceIndex(v - 1) == -1) {
                std::cout << "Skipping vertex[case 2]: " << v << " due to missing adjacent face at index " << (v - 1) << std::endl;
                continue;
            }

            PerpChain chain;
            Point2D currentVertex = face.vertex(v);
            int currentFace = f;
            int currentEdge = v;

            std::cout << "currentVertex = " << currentVertex << std::endl;
            std::cout << "currentFace = " << currentFace << std::endl;
            std::cout << "currentEdge = " << currentEdge << std::endl;

            std::cout << "Starting new chain from vertex: " << v << std::endl;

            for (int step = 0; step < MAX_PERP_PROGRESSION; step++) {
                std::cout << "Step: " << step << std::endl;
                PerpHelperResult phrResult = perpHelper(currentVertex, skeleton.face(currentFace), currentEdge);

                if (!phrResult.isValid) {
                    std::cout << "Invalid PerpHelperResult at step: " << step << std::endl;
                    break;
                }

                chain.push_back({currentVertex, phrResult.intersection, currentFace});
                std::cout << "Added to chain: " << currentVertex << " -> " << phrResult.intersection << std::endl;

                int nextFaceIndex = skeleton.face(currentFace).adjacentFaceIndex(phrResult.edgeIndex);
                if (nextFaceIndex == -1) {
                    // infinity
                    std::cout << "Reached infinity at step: " << step << std::endl;
                    break;
                }

                Point2D nextEdgeStart = skeleton.face(currentFace).vertex((phrResult.edgeIndex + 1) % skeleton.face(currentFace).vertexCount());
                int nextEdge = findEdgeIndex(skeleton.face(nextFaceIndex), nextEdgeStart);
                if (nextEdge < 0) {
                    std::cerr << "Error: Adjacent face mismatch" << std::endl;
                    break;
                }

                currentVertex = phrResult.intersection;
                currentFace = nextFaceIndex;
                currentEdge = nextEdge;
            }

            if (!chain.empty()) {
                result.push_back(chain);
                std::cout << "Chain added to result" << std::endl;
            }
        }
    }
    return result;
}

PerpHelperResult PerpendicularFinder::perpHelper(const Point2D& vertex, const ISkeletonFace& face, int edgeIndex) {
    if (face.vertexCount() < 2) {
        return {false, Point2D(0, 0), -1};
    }

    Vector2D baseVector = face.vertex(1) - face.vertex(0);
    Vector2D directionRay = rotate90(baseVector);
    directionRay = normalize(directionRay);

    Point2D vE0 = face.vertex(edgeIndex);
    Point2D vE1 = face.vertex((edgeIndex + 1) % face.vertexCount());
    Vector2D edgeVector = vE1 - vE0;

    if (edgeVector * directionRay < 0) {
        // flip direction
        directionRay = -directionRay;
    }

    double bestT = std::numeric_limits<double>::infinity();
    int bestEdge = -1;
    Point2D bestPoint;

    for (int e = 0; e < face.vertexCount(); e++) {
        if (e == edgeIndex) {
            continue;
        }

        Point2D p1 = face.vertex(e);
        Point2D p2 = face.vertex((e + 1) % face.vertexCount());

        auto intersection = intersectRaySegment(vertex, directionRay, p1, p2);
        if (intersection.valid && intersection.t > 0 && intersection.t < bestT && intersection.u >= 0 && intersection.u <= 1) {
            bestT = intersection.t;
            bestEdge = e;
            bestPoint = Point2D(vertex.x() + directionRay.x() * bestT, vertex.y() + directionRay.y() * bestT);
        }
    }

    if (bestEdge < 0) {
        return {false, Point2D(0, 0), -1};
    }

    std::cout << "PerpHelperResult: " << bestPoint << " on edge " << bestEdge << std::endl;
    return {true, bestPoint, bestEdge};
}

int PerpendicularFinder::findEdgeIndex(const ISkeletonFace& face, const Point2D& startPoint) const {
    for (int i = 0; i < face.vertexCount(); i++) {
        if (face.vertex(i) == startPoint) {
            return i;
        }
    }
    return -1;
}

Vector2D PerpendicularFinder::rotate90(const Vector2D& v) const { return Vector2D(-v.y(), v.x()); }

IntersectionResult PerpendicularFinder::intersectRaySegment(const Point2D& origin, const Vector2D& direction, const Point2D& segmentStart,
                                                            const Point2D& segmentEnd) const {
    // standard 2D line intersection
    Vector2D s = segmentEnd - segmentStart;
    double rayCrossS = cross(direction, s);
    Vector2D qMinusP = Vector2D(segmentStart.x() - origin.x(), segmentStart.y() - origin.y());
    double qMinusPCrossS = cross(qMinusP, direction);

    if (std::fabs(rayCrossS) < 1e-13) {
        // ray and segment are nearly parallel
        return {false, 0, 0};
    }

    double t = cross(qMinusP, s) / rayCrossS;
    double u = qMinusPCrossS / rayCrossS;

    return {true, t, u};
}

double PerpendicularFinder::cross(const Vector2D& a, const Vector2D& b) const {
    // Convert x() and y() of CGAL's number types to double
    double ax = CGAL::to_double(a.x());
    double ay = CGAL::to_double(a.y());
    double bx = CGAL::to_double(b.x());
    double by = CGAL::to_double(b.y());

    return ax * by - ay * bx;
}

Vector2D PerpendicularFinder::normalize(const Vector2D& v) const {
    // For length, we can do sqrt( to_double( squared_length() ) )
    double length = std::sqrt(CGAL::to_double(v.squared_length()));
    if (length == 0) {
        // handle zero-length vector, returning an unchanged or zero vector
        return v;
    }
    // Now we divide each coordinate by length, which is a double
    double vx = CGAL::to_double(v.x());
    double vy = CGAL::to_double(v.y());
    return Vector2D(vx / length, vy / length);
}

}  // namespace straight_skeleton
