#pragma once

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Line_2.h>
#include <CGAL/Point_2.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/intersections.h>
#include <CGAL/squared_distance_2.h>

/**
 * @brief: type-aliases for frequently used types
 */
namespace OneCut {
using K = CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt;
using Point = K::Point_2;
using Line = K::Line_2;
using Vector = K::Vector_2;
using Direction = K::Direction_2;
using PlanarGraph = CGAL::Surface_mesh<Point>;
using VertexIndex = PlanarGraph::Vertex_index;
using FaceIndex = PlanarGraph::Face_index;
using EdgeIndex = PlanarGraph::Edge_index;
using HalfedgeIndex = PlanarGraph::Halfedge_index;
}  // namespace OneCut


