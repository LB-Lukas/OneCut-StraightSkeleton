#include "OneCut/FoldManager.h"

namespace OneCut {

FoldManager::FoldManager(const std::vector<SkeletonConstruction::Point>& polygon)
    : skeletonBuilder(SkeletonConstruction::SkeletonBuilder(polygon)),
      skeleton(skeletonBuilder.buildSkeleton()),
      perpendicularFinder(skeleton) {}

std::vector<Crease> FoldManager::getCreases() {
    std::vector<Crease> creases;
    for (int faceIndex = 0; faceIndex < skeleton.faceCount(); faceIndex++) {
        for (int vertexIndex = 1; vertexIndex < skeleton.face(faceIndex).vertexCount(); vertexIndex++) {
            auto adjacentFace = skeleton.face(faceIndex).adjacentFaces[vertexIndex];
            auto fold = std::make_pair(
                skeleton.face(faceIndex).vertices[vertexIndex],
                skeleton.face(faceIndex).vertices[(vertexIndex + 1) % skeleton.face(faceIndex).vertexCount()]);

            // Only process each face pair once
            if (adjacentFace > faceIndex) {
                Crease crease;
                std::pair<Point, Point> adjacentFaceEdge =
                    std::make_pair(skeleton.face(adjacentFace).vertices[0], skeleton.face(adjacentFace).vertices[1]);
                std::pair<Point, Point> edge =
                    std::make_pair(skeleton.face(faceIndex).vertices[0], skeleton.face(faceIndex).vertices[1]);

                Vector adjVec = skeleton.face(adjacentFace).vertices[1] - skeleton.face(adjacentFace).vertices[0];
                Vector foldVec = fold.second - fold.first;

                // Project adjacent edge vector onto current edge vector to determine fold direction
                auto projectionValue = GeometryUtil::scalarProjection(adjVec, foldVec);

                if (skeleton.face(faceIndex).isOuter) {
                    if (projectionValue > -0.0001) {
                        crease.foldType = FoldType::VALLEY;
                    } else {
                        crease.foldType = FoldType::MOUNTAIN;
                    }
                } else {
                    if (projectionValue > -0.0001) {
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

    // TODO: Tree structure? Dfs?
    // Add perpendicular creases; all valleys
    std::vector<PerpChain> chains = perpendicularFinder.findPerpendiculars();
    for (const auto& chain : chains) {
        for (const auto& segment : chain) {
            Crease crease;
            crease.edge =
                std::make_pair(Point(segment.start.x(), segment.start.y()), Point(segment.end.x(), segment.end.y()));
            crease.foldType = FoldType::VALLEY;
            crease.origin = Origin::PERPENDICULAR;
            creases.push_back(crease);
        }
    }

    return creases;
}

}  // namespace OneCut
