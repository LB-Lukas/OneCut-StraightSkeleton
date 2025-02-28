#pragma once

#include <optional>

#include "Ray.h"
#include "StraightSkeletonTypes.h"
#include "Triangle.h"
#include "IDataStructure.h"

namespace straight_skeleton {

struct IntersectionResult;

// Interface for the lowest intersection query
class ILowestIntersectionQuery : public IDataStructure {
   public:
    virtual ~ILowestIntersectionQuery() = default;

    virtual void addRay(const Ray& ray) = 0;

    virtual void removeRay(const Ray& ray) = 0;

    virtual IntersectionResult findLowestIntersection(const Triangle& triangle) const = 0;
};
}  // namespace straight_skeleton
