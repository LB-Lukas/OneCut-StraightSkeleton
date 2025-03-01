#pragma once

#include "StraightSkeletonTypes.h"
#include "IStraightSkeleton.h"

namespace straight_skeleton {

struct PerpSegment {
    Point2D start;
    Point2D end;
    int faceIndex;
};

struct PerpHelperResult {
    bool isValid;
    Point2D intersection;
    int edgeIndex;
};

struct PerpIntersectionResult {
    bool valid;
    double t; // distance along ray
    double u; // distance along segment
};

typedef std::vector<PerpSegment> PerpChain;

class PerpendicularFinder {
public:
    static const int MAX_PERP_PROGRESSION = 20;
    PerpendicularFinder(const IStraightSkeleton& skeleton);

    std::vector<PerpChain> findPerpendiculars();

private:
    const IStraightSkeleton& skeleton; // NOT ALLOWED?

    PerpHelperResult perpHelper(const Point2D& vertex, const ISkeletonFace& face, int edgeIndex);

    int findEdgeIndex(const ISkeletonFace& face, const Point2D& startPoint) const;

    Vector2D rotate90(const Vector2D& v) const;

    PerpIntersectionResult intersectRaySegment(const Point2D& origin, const Vector2D& direction, const Point2D& segmentStart, const Point2D& segmentEnd) const;

    double cross(const Vector2D& a, const Vector2D& b) const;

    Vector2D normalize(const Vector2D& v) const;

};

}