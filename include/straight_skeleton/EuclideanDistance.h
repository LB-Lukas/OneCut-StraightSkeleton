#pragma once
#include "straight_skeleton/StraightSkeletonTypes.h"
#include <cmath>


namespace straight_skeleton {

class EuclideanDistance {
   public:
   /**
    * @brief Get the Euclidean distance between two points.
    * 
    * @param a the first point
    * @param b the second point
    * @return double the Euclidean distance between the two points.
    */
    double operator()(const Point2D& a, const Point2D& b) const {
        double dx = CGAL::to_double(a.x() - b.x());
        double dy = CGAL::to_double(a.y() - b.y());
        return std::sqrt(dx * dx + dy * dy);
    }
};

}  // namespace straight_skeleton