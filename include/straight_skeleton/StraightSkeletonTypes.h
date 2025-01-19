#pragma once

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Line_2.h>
#include <CGAL/Point_2.h>

#include <CGAL/intersections.h>
#include <CGAL/squared_distance_2.h>
#include <CGAL/Surface_mesh.h>

/**
* @brief: type-aliases for frequently used types
*/
namespace straight_skeleton {
    using K             = CGAL::Exact_predicates_exact_constructions_kernel;
    using Point         = K::Point_2;
    using Line          = K::Line_2;
    using Vector        = K::Vector_2;
    using Direction     = K::Direction_2;
    using PlanarGraph   = CGAL::Surface_mesh<Point>;
    using VertexIndex   = PlanarGraph::Vertex_index;
    using FaceIndex     = PlanarGraph::Face_index;
    using EdgeIndex     = PlanarGraph::Edge_index;  
    using HalfedgeIndex = PlanarGraph::Halfedge_index;
}


    namespace std {
        template <>
        struct hash<CGAL::Point_2<CGAL::Epeck>> {
            size_t operator()(const CGAL::Point_2<CGAL::Epeck>& p) const noexcept {
                // convertion of x- and y-coordinates to double values
                double x = CGAL::to_double(p.x());
                double y = CGAL::to_double(p.y());

                // hashing of the x- and y-coordinates
                size_t hx = std::hash<double>()(x);
                size_t hy = std::hash<double>()(y);

                // combine both hashes in a simple way
                return hx ^ (hy << 1);
            }
        };

    } // namespace std

