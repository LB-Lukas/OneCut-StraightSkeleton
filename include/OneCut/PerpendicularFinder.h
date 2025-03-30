#pragma once

#include "IStraightSkeleton.h"
#include "StraightSkeletonTypes.h"
#include "utils/GeometryUtil.h"
#include "utils/IntersectionUtil.h"

namespace OneCut {

struct PerpSegment {
    Point start;
    Point end;
    int faceIndex;
};

struct PerpendicularHit {
    bool isValid;
    Point intersection;
    int edgeIndex;
};

typedef std::vector<PerpSegment> PerpChain;

class PerpendicularFinder {
   public:
    static const int MAX_ITERATIONS = 30;
    static const int PAPER_BORDER_X = 600;
    static const int PAPER_BORDER_Y = 600;
    PerpendicularFinder(const IStraightSkeleton& skeleton);

    std::vector<PerpChain> findPerpendiculars();

   private:
    const IStraightSkeleton& skeleton;

    PerpendicularHit computePerpendicularIntersection(const Point& vertex, const ISkeletonFace& face, int edgeIndex);

    int findEdgeIndex(const ISkeletonFace& face, const Point& startPoint) const;
};

}  // namespace OneCut