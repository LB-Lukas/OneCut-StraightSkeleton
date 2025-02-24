#pragma once

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Line_2.h>
#include <CGAL/Point_2.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/intersections.h>
#include <CGAL/squared_distance_2.h>

/**
 * @brief: type-aliases for frequently used types
 */
namespace straight_skeleton {
using K = CGAL::Exact_predicates_exact_constructions_kernel;
using Point2D = K::Point_2;
using Line2D = K::Line_2;
using Vector2D = K::Vector_2;
using Direction2D = K::Direction_2;
using Point3D = K::Point_3;
using Vector3D = K::Vector_3;
using Direction3D = K::Direction_3;
using PlanarGraph = CGAL::Surface_mesh<Point2D>;
using VertexIndex = PlanarGraph::Vertex_index;
using FaceIndex = PlanarGraph::Face_index;
using EdgeIndex = PlanarGraph::Edge_index;
using HalfedgeIndex = PlanarGraph::Halfedge_index;
}  // namespace straight_skeleton

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

}  // namespace std
