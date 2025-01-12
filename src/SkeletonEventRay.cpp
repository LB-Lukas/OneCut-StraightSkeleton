#include "straight_skeleton/event_management/SkeletonEventRay.h"  // Ensure this path is correct based on your project structure
#include <stdexcept>           // For std::runtime_error

namespace straight_skeleton {

    // Definition of the equality operator
    bool SkeletonEventRay::operator==(const SkeletonEventRay& other) const {
        return (start_point == other.start_point &&
                direction   == other.direction);
    }

    // Definition of the assertValide method
    void SkeletonEventRay::assertValide() const {
        // Obtain shared_ptr references to left_ray and right_ray
        std::shared_ptr<const SkeletonEventRay> l = left_ray;
        std::shared_ptr<const SkeletonEventRay> r = right_ray;

        // Check if either neighbor is missing
        if (!l || !r) {
            throw std::runtime_error("SkeletonEventRay loop is not closed (missing neighbors).");
        }

        // Traverse the left_ray chain to ensure it loops back to this ray
        std::shared_ptr<const SkeletonEventRay> current_left = l;
        while (current_left != shared_from_this()) {
            if (!current_left) {
                throw std::runtime_error("SkeletonEventRay loop is not closed (null encountered in left_ray).");
            }
            current_left = current_left->left_ray;
        }

        // Traverse the right_ray chain to ensure it loops back to this ray
        std::shared_ptr<const SkeletonEventRay> current_right = r;
        while (current_right != shared_from_this()) {
            if (!current_right) {
                throw std::runtime_error("SkeletonEventRay loop is not closed (null encountered in right_ray).");
            }
            current_right = current_right->right_ray;
        }

        // If both traversals successfully loop back, the structure is valid
    }

}
