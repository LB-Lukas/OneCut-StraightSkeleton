#pragma once

#include "straight_skeleton/StraightSkeletonTypes.h"

namespace straight_skeleton {

class GeometryUtil {
   public:
    static Vector rotate90(const Vector& v);
    static Vector normalize(const Vector& v);
    static K::FT cross(const Vector& a, const Vector& b);
};

}  // namespace straight_skeleton
