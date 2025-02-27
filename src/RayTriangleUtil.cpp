#include "straight_skeleton/utils/RayTriangleUtil.h"

#include <CGAL/Kernel/global_functions_3.h>
#include <CGAL/Vector_3.h>

#include <cmath>

namespace straight_skeleton {

Triangle3D RayTriangleUtil::getTriangle(const Triangle& triangle) {
    return Triangle3D(triangle.getBaseA(), triangle.getBaseB(), triangle.getApex());
}

Ray3D RayTriangleUtil::getRay(const Ray& ray) {
    return Ray3D(ray.getOrigin(), ray.getDirection().vector());
}

std::optional<std::pair<double, Point3D>> RayTriangleUtil::intersectRayTriangle(const Ray3D& ray,
                                                                                const Triangle3D& triangle) {
    // Retrieve the triangle's vertices.
    const Point3D& p0 = triangle.vertex(0);
    const Point3D& p1 = triangle.vertex(1);
    const Point3D& p2 = triangle.vertex(2);

    // Compute edges of the triangle.
    Vector3D edge1 = p1 - p0;
    Vector3D edge2 = p2 - p0;

    // Get ray origin and convert its direction to a vector.
    const Point3D& origin = ray.source();
    Vector3D d = ray.direction().vector();  // Assumes Direction3D provides a .vector() method

    // Begin Möller–Trumbore algorithm.
    Vector3D h = CGAL::cross_product(d, edge2);
    auto a = edge1 * h;  // dot product

    constexpr double EPSILON = 1e-6;
    if (std::abs(CGAL::to_double(a)) < EPSILON) {
        // Ray is parallel to the triangle.
        return std::nullopt;
    }

    auto f = 1 / a;
    Vector3D s = origin - p0;
    auto u = f * (s * h);
    if (CGAL::to_double(u) < 0.0 || CGAL::to_double(u) > 1.0) {
        return std::nullopt;
    }

    Vector3D q = CGAL::cross_product(s, edge1);
    auto v = f * (d * q);
    if (CGAL::to_double(v) < 0.0 || (CGAL::to_double(u) + CGAL::to_double(v)) > 1.0) {
        return std::nullopt;
    }

    auto t = f * (edge2 * q);
    if (CGAL::to_double(t) < 0.0) {
        // Intersection point lies behind the ray origin.
        return std::nullopt;
    }

    // Compute the intersection point along the ray.
    Point3D intersectionPoint = origin + CGAL::to_double(t) * d;
    return std::make_optional(std::make_pair(CGAL::to_double(t), intersectionPoint));
}

}  // namespace straight_skeleton
