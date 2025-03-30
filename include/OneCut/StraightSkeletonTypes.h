#pragma once

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Line_2.h>
#include <CGAL/Point_2.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/intersections.h>
#include <CGAL/squared_distance_2.h>

/**
 * @namespace OneCut
 * @brief Contains type aliases for CGAL geometric primitives and data structures.
 * 
 * This namespace provides convenient type aliases for working with CGAL's exact
 * geometric computation kernel and related data structures used throughout the
 * OneCut project.
 */

namespace OneCut {
/**
 * @brief Exact computation kernel with square root support.
 * 
 * Uses CGAL's exact predicates and exact constructions kernel with sqrt operations.
 * Ensures robust geometric computations without floating-point errors.
 */
using K = CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt;

/// @name Geometric Primitives
/// @{
using Point = K::Point_2;    ///< 2D point with exact coordinates
using Line = K::Line_2;      ///< 2D line with exact representation
using Vector = K::Vector_2;  ///< 2D vector with exact representation
using Direction = K::Direction_2;  ///< 2D direction (normalized vector)
/// @}

/**
 * @brief Planar graph representation using CGAL's Surface_mesh.
 * 
 * The underlying data structure for representing straight skeletons and fold patterns.
 * Provides efficient topological operations and maintains geometric data.
 */
using PlanarGraph = CGAL::Surface_mesh<Point>;

/// @name Planar Graph Indices
/// @{
using VertexIndex = PlanarGraph::Vertex_index;    ///< Index type for vertices in the graph
using FaceIndex = PlanarGraph::Face_index;       ///< Index type for faces in the graph
using EdgeIndex = PlanarGraph::Edge_index;       ///< Index type for edges in the graph
using HalfedgeIndex = PlanarGraph::Halfedge_index;  ///< Index type for halfedges in the graph
/// @}

}  // namespace OneCut