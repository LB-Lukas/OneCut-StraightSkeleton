#pragma once

#include <memory>
#include <utility>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

#include "../StraightSkeletonTypes.h"
//#include "SkeletonEventTriangle.h"
#include "SkeletonEventRay.h"

namespace straight_skeleton {
    class SkeletonEventTriangle;
    using TrianglePtr = std::shared_ptr<SkeletonEventTriangle>;
}

namespace straight_skeleton {

    // forward decleration
    class SkeletonEvent;
    class EdgeEvent;
    class SplitEvent;
    class VertexEvent;

    using SkeletonEventPtr = std::shared_ptr<SkeletonEvent>;
    using EdgeEventPtr = std::shared_ptr<EdgeEvent>;
    using VertexEventPtr = std::shared_ptr<VertexEvent>;
    using SplitEventPtr = std::shared_ptr<SplitEvent>;

    class SkeletonEvent {
    public:
        // make Skeleton Event polymorphic
        virtual ~SkeletonEvent() = default;

        double time; // The "inward offset" time at which this event occurs (third upward dimension)
        Point location; // Intersection point in the plane

        SkeletonEvent(double tm, Point loc)
            : time(tm), location(std::move(loc)) {
        }
    };

    /**
     * to wedges collide with each other
     */
    class EdgeEvent : public SkeletonEvent {
    public:
        TrianglePtr edge_triangle;  // The triangle associated with the edge where collision occurs
        RayPtr ray;                 // Optional ray associated with the event

        /**
         * Constructor for EdgeEvent.
         *
         * @param tm            The time at which the event occurs.
         * @param loc           The intersection point in the plane.
         * @param edge_tri      The triangle associated with the edge.
         * @param optional_ray  (Optional) The ray associated with the event. Defaults to nullptr if not provided.
         */
        EdgeEvent(double tm, const Point &loc,
                  TrianglePtr edge_tri, RayPtr optional_ray = nullptr)
            : SkeletonEvent(tm, loc),                  // Initialize base class
              edge_triangle(std::move(edge_tri)),   // Initialize triangle
              ray(std::move(optional_ray)) {        // Initialize ray
        }
    };

    class VertexEvent : public SkeletonEvent {
        using TriangleVector = std::vector<TrianglePtr>;

    public:
        TriangleVector triangles;

        VertexEvent(double tm, const Point &loc,
          TrianglePtr edge_tri, RayPtr optional_ray = nullptr)
    : SkeletonEvent(tm, loc){        }
    };

    class SplitEvent : public SkeletonEvent {
    public:
        RayPtr ray;
        CGAL::SM_Edge_index intersect_edge;

        SplitEvent(double tm, const Point &loc,
          TrianglePtr edge_tri, RayPtr optional_ray = nullptr)
    : SkeletonEvent(tm, loc),                  // Initialize base class
      ray(std::move(optional_ray)) {        // Initialize ray
        }
    };
}

namespace std {
    /**
     * @brief Hash functor for Geometry::Ray, combining the geometry fields.
     *        This needs to remain in the header so any compilation unit
     *        that uses std::unordered_map<Ray, ...> sees it.
     */
    template<>
    struct hash<straight_skeleton::SkeletonEvent> {
        std::size_t operator()(const straight_skeleton::SkeletonEvent &skeleton_event) const noexcept {
            // Hash the location point
            std::size_t hash_x = std::hash<double>()(CGAL::to_double(skeleton_event.location.x()));
            std::size_t hash_y = std::hash<double>()(CGAL::to_double(skeleton_event.location.y()));

            // Hash the time
            std::size_t hash_time = std::hash<double>()(skeleton_event.time);

            // Combine them in a typical way:
            std::size_t h = hash_x ^ (hash_y + 0x9e3779b97f4a7c15ULL + (hash_x << 6) + (hash_x >> 2));
            h ^= (hash_time + 0x9e3779b97f4a7c15ULL + (h << 6) + (h >> 2));

            return h;
        }
    };
}