#pragma once

#include "OneCut/StraightSkeletonTypes.h"

namespace OneCut {

class GeometryUtil {
   public:
    static Vector rotate90(const Vector& v);
    static Vector normalize(const Vector& v);
    static K::FT cross(const Vector& a, const Vector& b);
    static K::FT scalarProjection(const Vector& a, const Vector& b);
};

}  // namespace OneCut
