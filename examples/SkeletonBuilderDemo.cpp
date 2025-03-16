#include <vector>

#include "../include/straight_skeleton/SkeletonBuilder.h"
#include "../include/straight_skeleton/StraightSkeleton.h"
#include "../include/straight_skeleton/StraightSkeletonTypes.h"

namespace straight_skeleton {

StraightSkeleton getSkeleton() {
    // simple polygon
    TestSkeleton::Point p1(0, 0);
    TestSkeleton::Point p2(2, 0);
    TestSkeleton::Point p3(2, 2);
    TestSkeleton::Point p4(0, 2);
    std::vector<TestSkeleton::Point> polygon = {p1, p2, p3, p4};

    TestSkeleton::SkeletonBuilder builder(polygon);
    StraightSkeleton skeleton = builder.buildSkeleton();

    return skeleton;
}

}  // namespace straight_skeleton

int main() {
    straight_skeleton::StraightSkeleton skeleton = straight_skeleton::getSkeleton();

    std::cout << "Straight Skeleton:" << std::endl;
    for (const auto& face : skeleton.getFaces()) {
        std::cout << face << std::endl;
    }


    return 0;
}
