#pragma once

#include "OneCut/StraightSkeletonTypes.h"

namespace OneCut {

/**
 * @class GeometryUtil
 * @brief Provides utility functions for 2D geometric operations.
 *
 * This class contains static methods for common vector operations used
 * throughout the OneCut project. All operations use exact arithmetic
 * through CGAL's exact computation kernel.
 */
class GeometryUtil {
public:
    /**
     * @brief Rotates a vector 90 degrees counterclockwise.
     * @param v The input vector to rotate
     * @return A new vector perpendicular to the input
     */
    static Vector rotate90(const Vector& v);

    /**
     * @brief Normalizes a vector to unit length.
     * @param v The vector to normalize
     * @return A vector in the same direction with length 1
     * @warning Input vector must not have zero length
     */
    static Vector normalize(const Vector& v);

    /**
     * @brief Computes the 2D cross product of two vectors.
     * @param a First input vector
     * @param b Second input vector
     * @return The signed magnitude of the cross product (a.x*b.y - a.y*b.x)
     * @note This returns a scalar value representing the "z-component" in 3D
     */
    static K::FT cross(const Vector& a, const Vector& b);

    /**
     * @brief Computes the scalar projection of a onto b.
     * @param a The vector to project
     * @param b The vector to project onto
     * @return The signed length of a's projection onto b
     * @note Equivalent to |a|*cos(θ) where θ is the angle between vectors
     */
    static K::FT scalarProjection(const Vector& a, const Vector& b);
};

}  // namespace OneCut