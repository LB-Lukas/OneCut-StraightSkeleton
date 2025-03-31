#include "OneCut/FoldManager.h"

namespace OneCut {

FoldManager::FoldManager(const std::vector<SkeletonConstruction::Point>& polygon)
    : skeletonBuilder(SkeletonConstruction::SkeletonBuilder(polygon)),
      skeleton(skeletonBuilder.buildSkeleton()),
      perpendicularFinder(skeleton) {}

std::vector<Crease> FoldManager::getCreases() {
    std::vector<Crease> creases;
    for (int f = 0; f < skeleton.faceCount(); f++) {
        for (int v = 1; v < skeleton.face(f).vertexCount(); v++) {
            auto adj = skeleton.face(f).adjacentFaces[v];
            auto fold = std::make_pair(skeleton.face(f).vertices[v],
                                       skeleton.face(f).vertices[(v + 1) % skeleton.face(f).vertexCount()]);
            if (adj > f) {
                Crease crease;
                auto adjEdge = std::make_pair(skeleton.face(adj).vertices[0], skeleton.face(adj).vertices[1]);
                auto edge = std::make_pair(skeleton.face(f).vertices[0], skeleton.face(f).vertices[1]);

                auto adjVec = skeleton.face(adj).vertices[1] - skeleton.face(adj).vertices[0];
                auto foldVec = fold.second - fold.first;

                auto sidedness = GeometryUtil::scalarProjection(adjVec, foldVec);

                if (skeleton.face(f).isOuter) {
                    if (sidedness > -0.0001) {
                        crease.foldType = FoldType::VALLEY;
                    } else {
                        crease.foldType = FoldType::MOUNTAIN;
                    }
                } else {
                    if (sidedness > -0.0001) {
                        crease.foldType = FoldType::MOUNTAIN;
                    } else {
                        crease.foldType = FoldType::VALLEY;
                    }
                }

                crease.edge = fold;
                crease.origin = Origin::SKELETON;
                creases.push_back(crease);
            }
        }
    }


    // Add perpendicular creases; all valleys
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

}  // namespace OneCut
