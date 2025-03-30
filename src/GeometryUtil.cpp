#include "OneCut/utils/GeometryUtil.h"

namespace OneCut {

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

K::FT GeometryUtil::scalarProjection(const Vector& a, const Vector& b) {
    auto dot_product = CGAL::scalar_product(a, b);
    auto axis_squared_length = b.squared_length();

    if (axis_squared_length == 0) {
        return 0.0;  // Avoid division by zero
    }

    return CGAL::to_double(dot_product) / std::sqrt(CGAL::to_double(axis_squared_length));
}

}  // namespace OneCut
