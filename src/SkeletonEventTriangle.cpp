#include "straight_skeleton/event_management/SkeletonEventTriangle.h"

#include "straight_skeleton/event_management/SkeletonEvent.h"

namespace straight_skeleton {
    bool SkeletonEventTriangle::operator==(const SkeletonEventTriangle &t) const {
        // Compare only the intersection point
        return (this->intersect_point == t.intersect_point);
    }

    void SkeletonEventTriangle::assertValide() const {
        // Start from the left and right neighbors
        TrianglePtr left_triangle = this->l_triangle_ptr;
        TrianglePtr right_triangle = this->r_triangle_ptr;

        // Basic null check
        if (!left_triangle || !right_triangle) {
            throw std::runtime_error("Triangle is not part of a closed loop (missing neighbors).");
        }

        // We'll climb left and right until we come back to 'this'
        while (left_triangle != shared_from_this() && right_triangle != shared_from_this()) {
            left_triangle = left_triangle->l_triangle_ptr;
            right_triangle = right_triangle->r_triangle_ptr;

            if (!left_triangle || !right_triangle) {
                throw std::runtime_error("Triangle is not part of a closed loop (null neighbor encountered).");
            }
        }

        // Finally, ensure both arrived back at 'this'
        if (left_triangle != shared_from_this() || right_triangle != shared_from_this()) {
            throw std::runtime_error("Left-linked and right-linked loops differ in size; not a closed loop.");
        }
    }
} // end namespace Geometry
