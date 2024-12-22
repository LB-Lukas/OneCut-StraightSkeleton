#pragma once
#include "../include/StraightSkeleton/straightSkeleton.h"

namespace Geometry {
    struct Triangle {
        std::shared_ptr<Triangle> l_triangle_ptr, r_triangle_ptr;

        Point intersect_point;

        CGAL::SM_Vertex_index l_pol_vertex, r_pol_vertex;

        bool operator==(const Triangle& t) const {
            return this->intersect_point == t.intersect_point;
        }

    };
}

namespace std {
    template <>
    struct hash<Geometry::Triangle> {
        std::size_t operator()(const Geometry::Triangle& t) const {
            // Combine hashes of the coordinates of the intersect point
            std::size_t h1 = std::hash<double>()(CGAL::to_double(t.intersect_point.x()));
            std::size_t h2 = std::hash<double>()(CGAL::to_double(t.intersect_point.y()));

            // Combine the hashes
            return h1 ^ (h2 << 1);
        }
    };
}
