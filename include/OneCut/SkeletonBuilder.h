#pragma once

#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <utility>
#include <vector>

// CGAL headers for kernel and surface mesh
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/Straight_skeleton_2/IO/print.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/create_straight_skeleton_2.h>
#include <CGAL/draw_straight_skeleton_2.h>

#include "Crease.h"
#include "SkeletonFace.h"
#include "StraightSkeleton.h"

namespace SkeletonConstruction {

/// @name CGAL Type Definitions
/// @{
typedef CGAL::Exact_predicates_inexact_constructions_kernel K; ///< CGAL kernel with exact predicates
typedef K::Point_2 Point;                                      ///< 2D point type for geometric calculations
typedef CGAL::Polygon_2<K> Polygon_2;                          ///< Polygon container type
typedef CGAL::Straight_skeleton_2<K> Ss;                       ///< Straight skeleton data structure
/// @}

/// @name Smart Pointers and Surface Mesh
/// @{
typedef std::shared_ptr<Ss> SsPtr;                ///< Shared pointer to straight skeleton
typedef CGAL::Surface_mesh<Point> SurfaceMesh;    ///< Surface mesh representation
/// @}

/**
 * @class SkeletonBuilder
 * @brief Builds a StraightSkeleton object using CGAL's straight skeleton algorithms
 * 
 * Constructs both inner and outer straight skeletons from input polygons and converts them
 * into a unified data structure with proper face adjacencies for perpendicular fold computation.
 */
class SkeletonBuilder {
   public:
    /**
     * @brief Construct a new Skeleton Builder from polygon vertices
     * @param polygon_points Input polygon vertices in counter-clockwise order
     * @note The polygon must be simple (non-intersecting) and non-degenerate
     */
    explicit SkeletonBuilder(const std::vector<Point>& polygon_points);

    /**
     * @brief Build the complete straight skeleton structure
     * @return OneCut::StraightSkeleton Combined skeleton containing both inner and outer faces
     * @details Merges inner and outer skeletons while maintaining proper face adjacencies
     */
    OneCut::StraightSkeleton buildSkeleton();

   private:
    /// @name CGAL Skeleton Structures
    /// @{
    SsPtr iss_; ///< Inner straight skeleton (contour offsetting)
    SsPtr oss_; ///< Outer straight skeleton (polygon boundary)
    /// @}

    /// @name Face Tracking Structures
    /// @{
    std::map<std::pair<Point, Point>, int> polyEdgeToFaceIndexMap; ///< Maps polygon edges to face indices
    std::vector<OneCut::SkeletonFace> faces;       ///< All generated faces
    std::vector<OneCut::SkeletonFace> facesOuter;  ///< Faces from outer skeleton
    std::vector<OneCut::SkeletonFace> facesInner;  ///< Faces from inner skeleton
    std::vector<Point> originalPolygonPoints;      ///< Original input vertices
    /// @}

    /// @name Skeleton Conversion Utilities
    /// @{
    /**
     * @brief Convert inner skeleton to face structures
     * @param skeleton CGAL straight skeleton pointer
     * @param offset Index offset for face numbering
     * @return Vector of generated SkeletonFace objects
     */
    std::vector<OneCut::SkeletonFace> innerSkeletonToFaces(SsPtr skeleton, int offset);

    /**
     * @brief Convert outer skeleton to face structures
     * @param skeleton CGAL straight skeleton pointer
     * @param offset Index offset for face numbering
     * @return Vector of generated SkeletonFace objects
     */
    std::vector<OneCut::SkeletonFace> outerSkeletonToFaces(SsPtr skeleton, int offset);

    /**
     * @brief Convert CGAL point to application-specific point type
     * @param point Input CGAL point
     * @return Converted OneCut::Point
     */
    OneCut::Point convertPoint(const Point& point) const;
    /// @}
};

}  // namespace SkeletonConstruction