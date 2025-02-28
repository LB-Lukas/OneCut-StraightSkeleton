#pragma once

#include <vector>
#include <algorithm>
#include <limits>

#include "ILowestIntersectionQuery.h"
#include "utils/RayTriangleUtil.h"

namespace straight_skeleton {

// Time Complexity: O(n) ???
class NaiveLowestIntersectionQuery : public ILowestIntersectionQuery {
   public:
    NaiveLowestIntersectionQuery() = default;
    ~NaiveLowestIntersectionQuery() override = default;
    void addRay(const Ray& ray) override;
    void removeRay(const Ray& ray) override;
    IntersectionResult findLowestIntersection(const Triangle& triangle) const override;
    bool isEmpty() const override;
    size_t size() const override; 

   private:
    std::vector<Ray> rays;
};

}  // namespace straight_skeleton