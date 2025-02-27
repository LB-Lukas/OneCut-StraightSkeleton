#pragma once

#include <optional>
#include <vector>

#include "Ray.h"
#include "StraightSkeletonTypes.h"
#include "Triangle.h"

namespace straight_skeleton {

struct IntersectionResult {
    bool hasIntersection{false};
    Ray intersectedRay;
    Point3D intersectionPoint;
    double parameterT;

    IntersectionResult() = default;
};

// Interface for the lowest intersection query
class ILowestIntersectionQuery {
   public:
    virtual ~ILowestIntersectionQuery() = default;

    virtual void addRay(const Ray& ray) = 0;

    virtual void removeRay(const Ray& ray) = 0;

    virtual IntersectionResult findLowestIntersection(const Triangle& triangle) const = 0;
};
}  // namespace straight_skeleton
