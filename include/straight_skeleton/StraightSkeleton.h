#pragma once
#include <cmath>
#include <vector>
#include <queue>

#include "Event.h"
#include "IStraightSkeleton.h"
#include "Ray.h"
#include "SkeletonFace.h"
#include "StraightSkeletonTypes.h"
#include "Triangle.h"
#include "utils/SkeletonGeometryUtil.h"
#include "utils/RayTriangleUtil.h"

namespace straight_skeleton {

struct EventCompare {
    bool operator()(const TrianglePtr& a, const TrianglePtr& b) const {
        // Compare the underlying Event by time, type, etc.
        return (a->getApex().z() > b->getApex().z());
    }
};

class StraightSkeleton : public IStraightSkeleton {
   public:
    StraightSkeleton(const std::vector<Point2D>& polygonPoints);

    void computeSkeleton();

    size_t faceCount() const override;

    const ISkeletonFace& face(size_t i) const override;

    std::vector<std::pair<Point2D, Point2D>> getEdges() const;

   private:
    std::vector<straight_skeleton::SkeletonFace> faces;
    std::vector<Point2D> originalPolygonPoints;
    std::priority_queue<TrianglePtr, std::vector<TrianglePtr>, EventCompare> eventQueue;
    
    Polygon2D polygon;
    PlanarGraph graph;
    std::vector<std::pair<Point2D, Point2D>> edges;

    void initialize();

    void handleEdgeEvent(const TrianglePtr& event);

    void handleSplitEvent(const std::shared_ptr<Event>& event);

    void handleVertexEvent(const std::shared_ptr<Event>& event);

    void invalidateTriangleEvent(const std::shared_ptr<Triangle>& triangle);
};

}  // namespace straight_skeleton