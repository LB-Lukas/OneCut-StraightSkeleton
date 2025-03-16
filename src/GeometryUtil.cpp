#include "straight_skeleton/utils/GeometryUtil.h"

namespace straight_skeleton {

Vector GeometryUtil::rotate90(const Vector& v) {
    return Vector(-v.y(), v.x());
}

Vector GeometryUtil::normalize(const Vector& v) {
    auto sqLength = v.squared_length();
    if (sqLength == 0) {
        return v;  // Return the original vector if it's zero-length
    }
    auto d = CGAL::sqrt(sqLength);
    return v / d;
}

K::FT GeometryUtil::cross(const Vector& a, const Vector& b) {
    return a.x() * b.y() - a.y() * b.x();
}

}  // namespace straight_skeleton
