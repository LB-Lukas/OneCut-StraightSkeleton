#include <vector>

#include "../include/straight_skeleton/PerpendicularFinder.h"
#include "../include/straight_skeleton/PlaceholderSkeleton.h"
#include "../include/straight_skeleton/StraightSkeletonTypes.h"

namespace straight_skeleton {

std::vector<Point> getPerpendiculars() {
    Point p1(0, 0);
    Point p2(2, 0);
    Point p3(2, 2);
    Point p4(0, 2);
    Point center(1, 1);

    std::vector<Point> f0 = {p1, p2, center};
    std::vector<Point> f1 = {p2, p3, center};
    std::vector<Point> f2 = {p3, p4, center};
    std::vector<Point> f3 = {p4, p1, center};

    SkeletonFace face0(f0, {-1, 1, 3});
    SkeletonFace face1(f1, {-1, 2, 0});
    SkeletonFace face2(f2, {-1, 1, 1});
    SkeletonFace face3(f3, {-1, 0, 2});

    std::vector<SkeletonFace> faces = {face0, face1, face2, face3};
    PlaceholderSkeleton skeleton(faces);

    PerpendicularFinder finder(skeleton);
    std::vector<PerpChain> chains = finder.findPerpendiculars();

    // Collect the segment endpoints
    std::vector<Point> points;
    for (const auto& chain : chains) {
        for (const auto& seg : chain) {
            points.push_back(seg.start);
            points.push_back(seg.end);
        }
    }

    return points;
}

}  // namespace straight_skeleton

int main() {
    straight_skeleton::getPerpendiculars();

    return 0;
}
