#pragma once

#include "straight_skeleton/StraightSkeletonTypes.h"

namespace straight_skeleton {

/**
 * @brief Represents a ray in 3D space.
 *
 * The Ray class defines a geometric ray with an origin, a direction,
 * and an optional starting parameter (tStart). It provides methods to access
 * the ray's properties and compute positions along the ray.
 */
class Ray {
   public:
    /**
     * @brief Default constructor.
     *
     * Constructs a ray with default values for origin, direction, and tStart.
     */
    Ray();

    /**
     * @brief Constructs a ray with the specified origin and direction.
     *
     * @param origin The starting point of the ray.
     * @param direction The direction of the ray.
     */
    Ray(const Point3D& origin, const Direction3D& direction);

    /**
     * @brief Constructs a ray with the specified origin, direction, and starting parameter.
     *
     * @param origin The starting point of the ray.
     * @param direction The direction of the ray.
     * @param tStart The starting parameter along the ray.
     */
    Ray(const Point3D& origin, const Direction3D& direction, double tStart);

    /**
     * @brief Retrieves the origin of the ray.
     *
     * @return The origin point as a Point3D object.
     */
    Point3D getOrigin() const;

    /**
     * @brief Retrieves the direction of the ray.
     *
     * @return The direction as a Direction3D object.
     */
    Direction3D getDirection() const;

    /**
     * @brief Retrieves the starting parameter of the ray.
     *
     * @return The tStart value.
     */
    double getTStart() const;

    /**
     * @brief Computes the position along the ray at a given parameter.
     *
     * This method calculates the point along the ray corresponding to the
     * parameter t, using the ray's origin, direction, and tStart offset.
     *
     * @param t The parameter along the ray.
     * @return The computed position as a Point3D.
     */
    Point3D positionAt(double t) const;

    /**
     * @brief Compares this ray with another for equality.
     *
     * Two rays are considered equal if they have the same origin, direction,
     * and tStart value.
     *
     * @param other The other Ray to compare with.
     * @return true if the rays are equal, false otherwise.
     */
    bool operator==(const Ray& other) const;

   private:
    Point3D origin;        
    Direction3D direction;  
    double tStart;          
};

}  // namespace straight_skeleton
