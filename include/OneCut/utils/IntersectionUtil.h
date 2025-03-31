#pragma once

#include "OneCut/StraightSkeletonTypes.h"
#include "OneCut/utils/GeometryUtil.h"

namespace OneCut {

/**
 * @struct IntersectionResult
 * @brief Represents the result of a ray-segment intersection test.
 * 
 * Contains both a validity flag and parametric coordinates of the intersection.
 */
struct IntersectionResult {
    bool valid;         ///< True if a valid intersection exists
    double rayParam;    ///< Distance along ray (t-value) from origin to intersection point
    double segmentParam; ///< Distance along segment (u-value) from start to end point (0-1)
};

/**
 * @class IntersectionUtil
 * @brief Provides exact geometric intersection operations.
 *
 * This class contains static methods for computing intersections between
 * geometric primitives using exact arithmetic.
 */
class IntersectionUtil {
public:
    /**
     * @brief Computes the intersection between a ray and a line segment.
     * @param origin Starting point of the ray
     * @param direction Direction vector of the ray (does not need to be normalized)
     * @param segmentStart Starting point of the line segment
     * @param segmentEnd Ending point of the line segment
     * @return IntersectionResult containing:
     *         - valid: true if intersection exists within segment bounds
     *         - rayParam: parametric distance along ray (t > 0)
     *         - segmentParam: parametric distance along segment (0 ≤ u ≤ 1)
     *
     * @note Returns valid=false for:
     *       - Parallel or collinear non-intersecting geometries
     *       - Intersections behind ray origin (t ≤ 0)
     *       - Intersections outside segment bounds (u < 0 or u > 1)
     */
    static IntersectionResult intersectRaySegment(
        const Point& origin,
        const Vector& direction,
        const Point& segmentStart,
        const Point& segmentEnd);
};

}  // namespace OneCut