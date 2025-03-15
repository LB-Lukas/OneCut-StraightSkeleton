#pragma once

#include "StraightSkeletonTypes.h"
#include "IStraightSkeleton.h"
#include "utils/GeometryUtil.h"

namespace straight_skeleton {

struct PerpSegment {
    Point start;
    Point end;
    int faceIndex;
};

struct PerpHelperResult {
    bool isValid;
    Point intersection;
    int edgeIndex;
};

struct IntersectionResult {
    bool valid;
    double t; // distance along ray
    double u; // distance along segment
};

typedef std::vector<PerpSegment> PerpChain;

class PerpendicularFinder {
public:
    static const int MAX_PERP_PROGRESSION = 5;
    PerpendicularFinder(const IStraightSkeleton& skeleton);

    std::vector<PerpChain> findPerpendiculars();

private:
    const IStraightSkeleton& skeleton; // NOT ALLOWED?

    PerpHelperResult perpHelper(const Point& vertex, const ISkeletonFace& face, int edgeIndex);

    int findEdgeIndex(const ISkeletonFace& face, const Point& startPoint) const;

   // Vector rotate90(const Vector& v) const;

    IntersectionResult intersectRaySegment(const Point& origin, const Vector& direction, const Point& segmentStart, const Point& segmentEnd) const;

    // double cross(const Vector& a, const Vector& b) const;

    // Vector normalize(const Vector& v) const;

};

}