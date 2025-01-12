#pragma once

#include <memory>
#include <stdexcept>
#include <functional>
#include <CGAL/Surface_mesh.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

//#include "SkeletonEvent.h"
#include "../StraightSkeletonTypes.h"

// forward decleration to of circular dependency with SkeletonEdge.h
namespace straight_skeleton {
    class EdgeEvent;
    using EdgeEventPtr = std::shared_ptr<EdgeEvent>;
}

namespace straight_skeleton {

    /**
     * @brief A simple Triangle structure, used in the straight-skeleton logic.
     */
    struct SkeletonEventTriangle;   // forward decleration

    // alias for ptr of SkeletonEventTriangle
    using TrianglePtr = std::shared_ptr<SkeletonEventTriangle>;

    struct SkeletonEventTriangle : public std::enable_shared_from_this<SkeletonEventTriangle> {
        // Neighbor triangles
        TrianglePtr l_triangle_ptr;
        TrianglePtr r_triangle_ptr;

        EdgeEventPtr skeleton_event_ptr;

        // Intersection point in 2D
        Point intersect_point;

        // Polygon vertices associated with this triangle
        VertexIndex l_pol_vertex;
        VertexIndex r_pol_vertex;

        /**
         * @brief Equality operator compares only the intersect_point.
         *
         * Declared here; implemented in the .cpp file.
         */
        bool operator==(const SkeletonEventTriangle &t) const;

        /**
         * @brief Ensures that the Triangle pointers form a closed doubly-linked loop.
         * @throws std::runtime_error if the loop is invalid or broken.
         *
         * Declared here; implemented in the .cpp file.
         */
        void assertValide() const;
    };
}

//
// Specialize std::hash for Geometry::Triangle (needed for hashmap in heap-structure)
//
namespace std {
    template<>
    struct hash<straight_skeleton::SkeletonEventTriangle> {
        std::size_t operator()(const straight_skeleton::SkeletonEventTriangle &t) const noexcept {
            // Combine hashes of the coordinates of the intersect point
            std::size_t h1 = std::hash<double>()(CGAL::to_double(t.intersect_point.x()));
            std::size_t h2 = std::hash<double>()(CGAL::to_double(t.intersect_point.y()));

            // A simple combination (e.g. boost::hash_combine pattern)
            // h1 ^ (h2 << 1) is common, but here's a slightly more robust approach:
            return h1 ^ (h2 + 0x9e3779b97f4a7c15ULL + (h1 << 6) + (h1 >> 2));
        }
    };
} // end namespace std
