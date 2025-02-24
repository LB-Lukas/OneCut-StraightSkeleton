#include <cmath>
#include <iostream>
#include <vector>

#include "../include/straight_skeleton/ClosestPair.h"
#include "../include/straight_skeleton/StraightSkeletonTypes.h"
#include "../include/straight_skeleton/EuclideanDistance.h"

namespace straight_skeleton {

void demo_close_pair() {
    ClosestPair<Point2D, Point2D, EuclideanDistance> cp;

    Point2D r0(0, 0);
    Point2D r1(0, 1);
    Point2D r2(0, -1);

    cp.insertFirst(r0);
    cp.insertFirst(r1);
    cp.insertFirst(r2);
    std::cout << "First set size: " << cp.firstSetSize() << std::endl;
    std::cout << "Second set size: " << cp.secondSetSize() << std::endl;

    Point2D b0(1, 500);
    Point2D b1(1, 1);

    cp.insertSecond(b0);
    cp.insertSecond(b1);
    std::cout << "First set size: " << cp.firstSetSize() << std::endl;
    std::cout << "Second set size: " << cp.secondSetSize() << std::endl;

    std::cout << "Minimum distance: " << cp.getMinDistance() << std::endl;
    std::cout << "Minimum pair: " << cp.getMinPair().first << ", " << cp.getMinPair().second << std::endl;
}

}  // namespace straight_skeleton

int main() {
    straight_skeleton::demo_close_pair();

    return 0;
}
