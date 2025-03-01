#include "straight_skeleton/StraightSkeleton.h"

namespace straight_skeleton {

StraightSkeleton::StraightSkeleton(const std::vector<Point2D>& polygonPoints) : originalPolygonPoints(polygonPoints) {
    faces.reserve(polygonPoints.size());
}

void StraightSkeleton::computeSkeleton() {
    while (!eventQueue.isEmpty()) {
        Event event = eventQueue.pop();
        switch (event.getType()) {
            case EventType::EdgeEvent:
                handleEdgeEvent(event);
                break;
            case EventType::SplitEvent:
                handleSplitEvent(event);
                break;
            case EventType::VertexEvent:
                handleVertexEvent(event);
                break;
            default:
                break;
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
    std::vector<Triangle> triangles;
    triangles.reserve(polygon.edges().size());
    for (size_t i = 0; i < polygon.edges().size(); ++i) {
        triangles.push_back(SkeletonGeometryUtil::makeTriangle(i, polygon));
    }
}

void StraightSkeleton::handleEdgeEvent(const Event& event) {
    // Handle edge event logic
}

void StraightSkeleton::handleSplitEvent(const Event& event) {
    // Handle split event logic
}

void StraightSkeleton::handleVertexEvent(const Event& event) {
    // Handle vertex event logic
}

}  // namespace straight_skeleton