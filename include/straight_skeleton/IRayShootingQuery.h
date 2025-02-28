#pragma once

#include <optional>

#include "Ray.h"
#include "StraightSkeletonTypes.h"
#include "Triangle.h"
#include "IDataStructure.h"

namespace straight_skeleton {

struct IntersectionResult;

class IRayShootingQuery : public IDataStructure {
   public:
    virtual ~IRayShootingQuery() = default;

    virtual void addTriangle(const Triangle& triangle) = 0;

    virtual void removeTriangle(const Triangle& triangle) = 0;

    virtual IntersectionResult findRayIntersection(const Ray& ray) const = 0;
};

}  // namespace straight_skeleton