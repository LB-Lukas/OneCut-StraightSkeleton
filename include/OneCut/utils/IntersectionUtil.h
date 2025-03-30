#pragma once

#include "OneCut/StraightSkeletonTypes.h"
#include "OneCut/utils/GeometryUtil.h"

namespace OneCut {

struct IntersectionResult {
    bool valid;
    double rayParam;  // distance along ray
    double segmentParam;  // distance along segment
};

class IntersectionUtil {
   public:
    static IntersectionResult intersectRaySegment(const Point& origin, const Vector& direction,
                                                  const Point& segmentStart, const Point& segmentEnd);
};

}  // namespace OneCut