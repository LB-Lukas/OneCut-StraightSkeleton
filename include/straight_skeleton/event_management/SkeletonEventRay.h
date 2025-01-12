#pragma once

#include <memory>
#include <stdexcept>
#include <functional>
#include <utility>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Surface_mesh.h>
#include "../StraightSkeletonTypes.h"

/**
 * @file Ray.h
 * @brief Declares a Ray struct for the straight-skeleton algorithm.
 */

namespace straight_skeleton {
    /**
     * @brief Represents an "inward" or "boundary" ray in a straight-skeleton context.
     * This struct is similar to your Triangle struct, with pointers to neighboring rays.
     */
    struct SkeletonEventRay : public std::enable_shared_from_this<SkeletonEventRay> {
        /// Neighbor pointers, if you maintain a doubly-linked or circular list of rays
        std::shared_ptr<SkeletonEventRay> left_ray;
        std::shared_ptr<SkeletonEventRay> right_ray;

        /// Geometry: start point of the ray, plus its direction
        Point start_point;
        Direction direction;

        using RayPtr = std::shared_ptr<SkeletonEventRay>;

        /// basic constructor initializing left_ray and right_ray
        SkeletonEventRay(Point start_point, Direction direction)
            : start_point(std::move(start_point)), direction(std::move(direction)) {}

        /**
         * @brief Compares geometry (start_point, direction).
         *        Implemented in Ray.cpp.
         */
        bool operator==(const SkeletonEventRay &other) const;

        /**
         * @brief Checks if left_ray/right_ray form a closed loop back to this ray.
         *        Implemented in Ray.cpp.
         * @throws std::runtime_error if the loop is broken.
         */
        void assertValide() const;
    };

    // alias for shared_ptr of SkeletonEventRay
    using RayPtr = std::shared_ptr<SkeletonEventRay>;
} // end namespace Geometry


//
// Specialize std::hash for Geometry::Ray
//
namespace std {
    /**
     * @brief Hash functor for Geometry::Ray, combining the geometry fields.
     *        This needs to remain in the header so any compilation unit
     *        that uses std::unordered_map<Ray, ...> sees it.
     */
    template<>
    struct hash<straight_skeleton::SkeletonEventRay> {
        std::size_t operator()(const straight_skeleton::SkeletonEventRay &ray) const noexcept {
            // Hash the start point
            std::size_t hx = std::hash<double>()(CGAL::to_double(ray.start_point.x()));
            std::size_t hy = std::hash<double>()(CGAL::to_double(ray.start_point.y()));

            // Hash the direction (dx, dy)
            double dx = CGAL::to_double(ray.direction.dx());
            double dy = CGAL::to_double(ray.direction.dy());
            std::size_t hd1 = std::hash<double>()(dx);
            std::size_t hd2 = std::hash<double>()(dy);

            // Combine them in a typical way:
            std::size_t h = hx ^ (hy + 0x9e3779b97f4a7c15ULL + (hx << 6) + (hx >> 2));
            h ^= (hd1 + 0x9e3779b97f4a7c15ULL + (h << 6) + (h >> 2));
            h ^= (hd2 + 0x9e3779b97f4a7c15ULL + (h << 6) + (h >> 2));

            return h;
        }
    };
} // end namespace std
