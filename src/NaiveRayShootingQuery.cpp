#include "straight_skeleton/NaiveRayShootingQuery.h"

namespace straight_skeleton {

void NaiveRayShootingQuery::addTriangle(const Triangle& triangle) {
    triangles.push_back(triangle);
}

void NaiveRayShootingQuery::removeTriangle(const Triangle& triangle) {
    // Naive approach: find element in array and remove it
    auto it = std::find(triangles.begin(), triangles.end(), triangle);
    if (it != triangles.end()) {
        triangles.erase(it);
    }
}

IntersectionResult NaiveRayShootingQuery::findRayIntersection(const Ray& ray) const {
    IntersectionResult result;
    result.hasIntersection = false;
    double bestValue = std::numeric_limits<double>::infinity();
    for (const auto& triangle : triangles) {
        auto intersection =
            RayTriangleUtil::intersectRayTriangle(RayTriangleUtil::getRay(ray), RayTriangleUtil::getTriangle(triangle));
        if (intersection.has_value()) {
            const auto& [t, point] = intersection.value();
            if (t < bestValue) {
                bestValue = t;
                result.intersectedRay = ray;
                result.intersectedTriangle = triangle;
                result.intersectionPoint = point;
                result.parameterT = t;
                result.hasIntersection = true;
            }
        }
    }
    return result;
}

bool NaiveRayShootingQuery::isEmpty() const {
    return triangles.empty();
}

size_t NaiveRayShootingQuery::size() const {
    return triangles.size();
}
}  // namespace  straight_skeleton
