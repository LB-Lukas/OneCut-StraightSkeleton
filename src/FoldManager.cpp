#include "straight_skeleton/FoldManager.h"

namespace straight_skeleton {

FoldManager::FoldManager(const std::vector<TestSkeleton::Point>& polygon)
    : skeletonBuilder(TestSkeleton::TestStraightSkeleton(polygon)), perpendicularFinder(PerpendicularFinder(skeletonBuilder)) {}

std::vector<Crease> FoldManager::getCreases() {
    std::vector<Crease> creases;
    for (const auto& edge : skeletonBuilder.getEdges()) {
        Crease crease;
        crease.edge = std::make_pair(Point(edge.first.x(), edge.first.y()), Point(edge.second.x(), edge.second.y()));
        crease.foldType = FoldType::MOUNTAIN;
        crease.origin = Origin::SKELETON;
        creases.push_back(crease);
    }

    // Add perpendicular creases
    std::vector<PerpChain> chains = perpendicularFinder.findPerpendiculars();
    for (const auto& chain : chains) {
        for (const auto& seg : chain) {
            Crease crease;
            crease.edge = std::make_pair(Point(seg.start.x(), seg.start.y()), Point(seg.end.x(), seg.end.y()));
            crease.foldType = FoldType::VALLEY;
            crease.origin = Origin::PERPENDICULAR;
            creases.push_back(crease);
        }
    }

    return creases;
}

}  // namespace straight_skeleton
