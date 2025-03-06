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

Triangle RayTriangleUtil::getTriangleProjected(const Triangle& triangle) {
    return Triangle(Point3D(triangle.getBaseA().x(), triangle.getBaseA().y(), 0),
                    Point3D(triangle.getBaseB().x(), triangle.getBaseB().y(), 0),
                    Point3D(triangle.getApex().x(), triangle.getApex().y(), 0));
}

Point2D RayTriangleUtil::getProjection(const Point3D& point) {
    return Point2D(point.x(), point.y());
}

std::optional<std::pair<double, Point3D>> RayTriangleUtil::intersectRayTriangle(const Ray3D& ray,
                                                                                const Triangle3D& triangle) {
    const auto intersection = CGAL::intersection(ray, triangle);
    if (!intersection) {
        return std::nullopt;
    } else if (const Point3D* intersectionPoint = std::get_if<Point3D>(&*intersection)) {
        // Compute t using the projection formula:
        // t = ( (intersectionPoint - ray.source()) • rayDir ) / (rayDir • rayDir)
        Vector3D rayDir = ray.direction().vector();
        Vector3D diff = *intersectionPoint - ray.source();
        double t = CGAL::to_double((diff * rayDir) / (rayDir * rayDir));
        if (t < 0) {
            return std::nullopt;
        }
        return std::make_optional(std::make_pair(t, *intersectionPoint));
    } else if (const Segment3D* intersectionSegment = std::get_if<Segment3D>(&*intersection)) {
        // Get the endpoints of the overlapping segment.
        const Point3D& pA = intersectionSegment->source();
        const Point3D& pB = intersectionSegment->target();

        // Get the ray's direction vector.
        Vector3D rayDir = ray.direction().vector();

        // Compute the vector from the ray's origin to each endpoint.
        Vector3D diffA = pA - ray.source();
        Vector3D diffB = pB - ray.source();

        // Calculate parameter t for each endpoint along the ray.
        double tA = CGAL::to_double((diffA * rayDir) / (rayDir * rayDir));
        double tB = CGAL::to_double((diffB * rayDir) / (rayDir * rayDir));

        // Choose the endpoint with the smallest non-negative t.
        double t;
        Point3D chosen;
        if (tA >= 0 && tB >= 0) {
            if (tA < tB) {
                t = tA;
                chosen = pA;
            } else {
                t = tB;
                chosen = pB;
            }
        } else if (tA >= 0) {
            t = tA;
            chosen = pA;
        } else if (tB >= 0) {
            t = tB;
            chosen = pB;
        } else {
            // Both intersections lie behind the ray's origin.
            return std::nullopt;
        }
        return std::make_optional(std::make_pair(t, chosen));
    }
    return std::nullopt;
}

}  // namespace straight_skeleton
