#pragma once

#include "../StraightSkeletonTypes.h"
#include "../Triangle.h"
#include "RayTriangleUtil.h"

namespace straight_skeleton {

class SkeletonGeometryUtil {
   public:
    /**
     * @brief Construct a triangle for the given edge index and polygon
     *
     * @param edgeIndex
     * @param polygon
     * @return Triangle
     */
    static Triangle makeTriangle(const size_t& edgeIndex, const Polygon2D& polygon);

    static Triangle makeNewClockwiseTriangle(const TrianglePtr& collapsedEdge, const TrianglePtr& oldCW);

    static Triangle makeNewCounterClockwiseTriangle(const TrianglePtr& collapsedEdge, const TrianglePtr& oldCCW);

    static Vector2D computeBisector(const Point2D& prev, const Point2D& point, const Point2D& next);

    // calculate intersection for two lines A and B
    static Point2D intersectLines(const Point2D& originA, const Vector2D& dirA, const Point2D& originB,
                                  const Vector2D& dirB);

    // Counter clockwise
    static Vector2D rotate90(const Vector2D& vec);

    static Vector2D normalize(const Vector2D& vec);

    static Vector3D normalize(const Vector3D& vec);

    static double perpendicularDistance(const Point2D& point, const Line2D& line);

    static double computeEventTime(const Triangle& triangle);

    static bool areAllPointsUnique(const std::vector<Point2D>& points, double epsilon);
};

}  // namespace straight_skeleton