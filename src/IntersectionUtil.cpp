#include "OneCut/utils/IntersectionUtil.h"

namespace OneCut {

IntersectionResult IntersectionUtil::intersectRaySegment(const Point& origin, const Vector& direction,
                                                         const Point& segmentStart, const Point& segmentEnd) {
    Vector segmentVector = segmentEnd - segmentStart;
    double directionCrossSegment = CGAL::to_double(GeometryUtil::cross(direction, segmentVector));
    Vector originToSegmentStart = Vector(segmentStart.x() - origin.x(), segmentStart.y() - origin.y());
    double originToStartCrossDirection = CGAL::to_double(GeometryUtil::cross(originToSegmentStart, direction));

    if (std::fabs(directionCrossSegment) < 1e-13) {  // Check for near-parallel lines
        return {false, 0, 0};
    }

    // Calculate ray intersection parameter (t) and segment parameter (u)
    double rayParameter =
        CGAL::to_double(GeometryUtil::cross(originToSegmentStart, segmentVector) / directionCrossSegment);
    double segmentParameter = originToStartCrossDirection / directionCrossSegment;

    return {true, rayParameter, segmentParameter};
}

}  // namespace OneCut
