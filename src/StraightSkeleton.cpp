#include "straight_skeleton/StraightSkeleton.h"

namespace straight_skeleton {

StraightSkeleton::StraightSkeleton(const std::vector<Point2D>& polygonPoints) : originalPolygonPoints(polygonPoints) {
    faces.reserve(polygonPoints.size());
    initialize();
    computeSkeleton();
}

void StraightSkeleton::computeSkeleton() {
    while (!eventQueue.empty()) {
        TrianglePtr eventTriangle = eventQueue.top();
        eventQueue.pop();
        if (eventTriangle->isValid()) {
            handleEdgeEvent(eventTriangle);
        }

        // update Data structure
    }
}

size_t StraightSkeleton::faceCount() const {
    return faces.size();
}

const ISkeletonFace& StraightSkeleton::face(size_t i) const {
    return faces[i];
}

void StraightSkeleton::initialize() {
    // create polygon from points
    polygon = Polygon2D(originalPolygonPoints.begin(), originalPolygonPoints.end());

    // create Triangle for each edge
    std::vector<TrianglePtr> triangles;
    triangles.reserve(polygon.edges().size());
    for (size_t i = 0; i < polygon.edges().size(); ++i) {
        triangles.push_back(std::make_shared<Triangle>(SkeletonGeometryUtil::makeTriangle(i, polygon)));
    }

    // set pointers on neighbouring triangles
    for (size_t i = 0; i < triangles.size(); ++i) {
        triangles[i]->setCounterclockwiseNeighbour(triangles[(i - 1 + triangles.size()) % triangles.size()]);
        triangles[i]->setClockwiseNeighbour(triangles[(i + 1) % triangles.size()]);
    }

    for (const auto& triangle : triangles) {
        //auto e =std::make_shared<Event>(EventType::EdgeEvent, CGAL::to_double(triangle->getApex().z()), triangle, true);
        eventQueue.push(triangle);
    }

    // project all edges to the plane and edd 2d edges to the edges vector
    // for (const auto& triangle : triangles) {
    //     edges.push_back(std::make_pair(RayTriangleUtil::getProjection(triangle->getBaseA()),
    //     RayTriangleUtil::getProjection(triangle->getApex())));
    //     edges.push_back(std::make_pair(RayTriangleUtil::getProjection(triangle->getBaseB()),
    //     RayTriangleUtil::getProjection(triangle->getApex())));
    // }
}

std::vector<std::pair<Point2D, Point2D>> StraightSkeleton::getEdges() const {
    return edges;
}

void StraightSkeleton::handleEdgeEvent(const TrianglePtr& event) {
    TrianglePtr triangle = event;
    if (!triangle) {
        std::cout << "Triangle is null" << std::endl;
        return;
    }

    TrianglePtr cw = triangle->getClockwiseNeighbour();
    TrianglePtr ccw = triangle->getCounterclockwiseNeighbour();
    invalidateTriangleEvent(cw);
    invalidateTriangleEvent(ccw);
    invalidateTriangleEvent(triangle);

    edges.push_back(std::make_pair(RayTriangleUtil::getProjection(triangle->getBaseA()),
                                   RayTriangleUtil::getProjection(triangle->getApex())));
    edges.push_back(std::make_pair(RayTriangleUtil::getProjection(triangle->getBaseB()),
                                   RayTriangleUtil::getProjection(triangle->getApex())));
    TrianglePtr newCW = std::make_shared<Triangle>(SkeletonGeometryUtil::makeNewClockwiseTriangle(triangle, cw));
    TrianglePtr newCCW = std::make_shared<Triangle>(SkeletonGeometryUtil::makeNewCounterClockwiseTriangle(triangle, ccw));
    std::cout << "newCW: " << newCW->getBaseA() << ", " << newCW->getBaseB() << ", " << newCW->getApex() << std::endl;
    std::cout << "newCCW: " << newCCW->getBaseA() << ", " << newCCW->getBaseB() << ", " << newCCW->getApex() << std::endl;
    // update neighbours
    // newCW->setClockwiseNeighbour(cw->getClockwiseNeighbour());
    // newCW->setCounterclockwiseNeighbour(newCW); // temporary, later new ccw Neighbour will be set
}

void StraightSkeleton::invalidateTriangleEvent(const std::shared_ptr<Triangle>& triangle) {
    if (!triangle) return;
    triangle->setValid(false);
}

void StraightSkeleton::handleSplitEvent(const std::shared_ptr<Event>& event) {
    // Handle split event logic
}

void StraightSkeleton::handleVertexEvent(const std::shared_ptr<Event>& event) {
    // Handle vertex event logic
}

}  // namespace straight_skeleton