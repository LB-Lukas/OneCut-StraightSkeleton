#pragma once

#include <algorithm>
#include <limits>
#include <vector>

#include "IRayShootingQuery.h"
#include "utils/RayTriangleUtil.h"

namespace straight_skeleton {

class NaiveRayShootingQuery : public IRayShootingQuery {
   public:
    NaiveRayShootingQuery() = default;
    ~NaiveRayShootingQuery() override = default;
    void addTriangle(const Triangle& triangle) override;
    void removeTriangle(const Triangle& triangle) override;
    IntersectionResult findRayIntersection(const Ray& ray) const override;
    bool isEmpty() const override;
    size_t size() const override;

   private:
   std::vector<Triangle> triangles;
};
}  // namespace straight_skeleton