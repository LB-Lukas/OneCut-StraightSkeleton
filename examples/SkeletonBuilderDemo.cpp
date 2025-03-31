#include <vector>

#include "../include/OneCut/SkeletonBuilder.h"
#include "../include/OneCut/StraightSkeleton.h"
#include "../include/OneCut/StraightSkeletonTypes.h"

namespace OneCut {

StraightSkeleton getSkeleton() {
    // simple polygon
    SkeletonConstruction::Point p1(0, 0);
    SkeletonConstruction::Point p2(2, 0);
    SkeletonConstruction::Point p3(2, 2);
    SkeletonConstruction::Point p4(0, 2);
    std::vector<SkeletonConstruction::Point> polygon = {p1, p2, p3, p4};

    SkeletonConstruction::SkeletonBuilder builder(polygon);
    StraightSkeleton skeleton = builder.buildSkeleton();

    return skeleton;
}

}  // namespace OneCut

int main() {
    OneCut::StraightSkeleton skeleton = OneCut::getSkeleton();

    std::cout << "Straight Skeleton:" << std::endl;
    for (const auto& face : skeleton.getFaces()) {
        std::cout << face << std::endl;
    }


    return 0;
}
