#include "straight_skeleton/NaiveLowestIntersectionQuery.h"

namespace straight_skeleton {

void NaiveLowestIntersectionQuery::addRay(const Ray& ray) {
    rays.push_back(ray);
}

void NaiveLowestIntersectionQuery::removeRay(const Ray& ray) {
    // Naive approach: find array and remove it
    auto it = std::find(rays.begin(), rays.end(), ray);
    if (it != rays.end()) {
        rays.erase(it);
    }
}

IntersectionResult NaiveLowestIntersectionQuery::findLowestIntersection(const Triangle& triangle) const {
    IntersectionResult result;
    result.hasIntersection = false;
    double bestValue = std::numeric_limits<double>::infinity();
    for (const auto& ray : rays) {
        auto intersection =
            RayTriangleUtil::intersectRayTriangle(RayTriangleUtil::getRay(ray), RayTriangleUtil::getTriangle(triangle));
        if (intersection.has_value()) {
            const auto& [t, point] = intersection.value();
            if (t < bestValue) {
                bestValue = t;
                result.intersectedRay = ray;
                result.intersectionPoint = point;
                result.parameterT = t;
                result.hasIntersection = true;
            }
        }
    }

    return result;
}

}  // namespace straight_skeleton