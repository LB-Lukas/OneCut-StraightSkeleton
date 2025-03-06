#include "straight_skeleton/utils/SkeletonGeometryUtil.h"

namespace straight_skeleton {

// Create a triangle from the edge and the polygon
Triangle SkeletonGeometryUtil::makeTriangle(const size_t& edgeIndex, const Polygon2D& polygon) {
    size_t polygonSize = polygon.size();
    size_t i = edgeIndex % polygonSize;
    size_t iNext = (edgeIndex + 1) % polygonSize;
    size_t iPrev = (i + polygonSize - 1) % polygonSize;
    size_t iNext2 = (edgeIndex + 2) % polygonSize;

    // base
    Point2D baseP = polygon[i];
    Point2D baseQ = polygon[iNext];

    // get the bisector to the left and right edge
    Vector2D leftBisector = computeBisector(polygon[iPrev], baseP, baseQ);
    Vector2D rightBisector = computeBisector(baseP, baseQ, polygon[iNext2]);

    // get the intersection of the bisectors
    Point2D intersection = intersectLines(baseP, leftBisector, baseQ, rightBisector);

    // get the distance from the edge to the intersection
    double t = perpendicularDistance(intersection, Line2D(baseP, baseQ));

    // create the triangle
    Point3D baseA = Point3D(baseP.x(), baseP.y(), 0);
    Point3D baseB = Point3D(baseQ.x(), baseQ.y(), 0);
    Point3D apex = Point3D(intersection.x(), intersection.y(), t);

    return Triangle(baseA, baseB, apex, true);
}

Triangle SkeletonGeometryUtil::makeNewClockwiseTriangle(const TrianglePtr& collapsedEdge, const TrianglePtr& oldCW) {
    Point3D collapsePoint = collapsedEdge->getApex();
    double distance = CGAL::to_double(CGAL::squared_distance(collapsePoint, collapsedEdge->getBaseB()));
    Vector3D direction = normalize(oldCW->getApex()-oldCW->getBaseB());
    Point3D newPoint = oldCW->getBaseB() + distance * direction;
    return Triangle(collapsePoint, newPoint, oldCW->getApex(), true);
}

Triangle SkeletonGeometryUtil::makeNewCounterClockwiseTriangle(const TrianglePtr& collapsedEdge,
                                                                  const TrianglePtr& oldCCW) {
    Point3D collapsePoint = collapsedEdge->getApex();
    double distance = CGAL::to_double(CGAL::squared_distance(collapsePoint, collapsedEdge->getBaseA()));
    Vector3D direction = normalize(oldCCW->getApex()-oldCCW->getBaseA());
    Point3D newPoint = oldCCW->getBaseA() + distance * direction;
    return Triangle(newPoint, collapsePoint, oldCCW->getApex(), true);
}

Vector2D SkeletonGeometryUtil::computeBisector(const Point2D& prev, const Point2D& point, const Point2D& next) {
    // calculate the bisector
    Vector2D vecA = point - prev;
    Vector2D vecB = next - point;

    Vector2D normalA = normalize(rotate90(vecA));
    Vector2D normalB = normalize(rotate90(vecB));
    Vector2D bisector = normalA + normalB;

    return normalize(bisector);
}

Point2D SkeletonGeometryUtil::intersectLines(const Point2D& originA, const Vector2D& dirA, const Point2D& originB,
                                             const Vector2D& dirB) {
    double denominator = CGAL::to_double(dirA.x() * dirB.y() - dirA.y() * dirB.x());
    if (std::fabs(denominator) < 1e-9) {
        throw std::runtime_error("Lines are parallel");
    }

    Vector2D diff = originB - originA;
    double t = CGAL::to_double((diff.x() * dirB.y() - diff.y() * dirB.x()) / denominator);
    return originA + t * dirA;
}

Vector2D SkeletonGeometryUtil::rotate90(const Vector2D& vec) {
    return Vector2D(-vec.y(), vec.x());
}

Vector2D SkeletonGeometryUtil::normalize(const Vector2D& vec) {
    return vec / std::sqrt(CGAL::to_double(vec.squared_length()));
}

Vector3D SkeletonGeometryUtil::normalize(const Vector3D& vec) {
    return vec / std::sqrt(CGAL::to_double(vec.squared_length()));
}

double SkeletonGeometryUtil::perpendicularDistance(const Point2D& point, const Line2D& line) {
    return std::sqrt(CGAL::to_double(CGAL::squared_distance(point, line)));
}

double SkeletonGeometryUtil::computeEventTime(const Triangle& triangle) {
    return CGAL::to_double(triangle.getApex().z());
}

bool SkeletonGeometryUtil::areAllPointsUnique(const std::vector<Point2D>& points, double epsilon) {
    for (size_t i = 0; i < points.size(); i++) {
        for (size_t j = i + 1; j < points.size(); j++) {
            // Compute absolute differences FIRST, then compare to epsilon
            const double dx = CGAL::to_double(points[i].x() - points[j].x());
            const double dy = CGAL::to_double(points[i].y() - points[j].y());

            if (std::abs(dx) <= epsilon && std::abs(dy) <= epsilon) {
                return false;  // Found a near-duplicate
            }
        }
    }
    return true;  // All points are unique
}

}  // namespace straight_skeleton