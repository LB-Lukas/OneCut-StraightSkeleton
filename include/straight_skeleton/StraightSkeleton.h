#pragma once
#include <vector>
#include <cmath>

#include "Event.h"
#include "EventQueue.h"
#include "IStraightSkeleton.h"
#include "SkeletonFace.h"
#include "StraightSkeletonTypes.h"
#include "Triangle.h"
#include "Ray.h"
#include "utils/SkeletonGeometryUtil.h"

namespace straight_skeleton {

class StraightSkeleton : public IStraightSkeleton {
   public:
    StraightSkeleton(const std::vector<Point2D>& polygonPoints);

    void computeSkeleton();

    size_t faceCount() const override;

    const ISkeletonFace& face(size_t i) const override;

   private:
    std::vector<straight_skeleton::SkeletonFace> faces;
    std::vector<Point2D> originalPolygonPoints;
    EventQueue eventQueue;
    Polygon2D polygon;
    PlanarGraph graph;

    void initialize();

    void handleEdgeEvent(const Event& event);

    void handleSplitEvent(const Event& event);

    void handleVertexEvent(const Event& event);
};

}  // namespace straight_skeleton