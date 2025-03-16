#pragma once

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

#include "SkeletonFace.h"
#include "StraightSkeleton.h"
#include "Crease.h"

namespace TestSkeleton {

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

typedef K::Point_2 Point;
typedef CGAL::Polygon_2<K> Polygon_2;
typedef CGAL::Straight_skeleton_2<K> Ss;

typedef std::shared_ptr<Ss> SsPtr;
typedef CGAL::Surface_mesh<Point> SurfaceMesh;

class SkeletonBuilder {
   public:
    /// Construct the straight skeleton from the input polygon (vertices in counterclockwise order)
    explicit SkeletonBuilder(const std::vector<Point>& polygon_points);

    straight_skeleton::StraightSkeleton buildSkeleton();
    SurfaceMesh graph;

   private:
    SsPtr iss_;
    SsPtr oss_;
    std::vector<straight_skeleton::SkeletonFace> faces;
    std::vector<Point> originalPolygonPoints;

    std::vector<straight_skeleton::SkeletonFace> skeletonToFaces(
        SsPtr skeleton, const std::set<std::pair<Point, Point>, std::less<>>& polygonBoundaryEdges) const;

    straight_skeleton::Point convertPoint(const Point& point) const;

    std::set<std::pair<Point, Point>, std::less<>> computePolygonBoundaryEdges() const;

    void matchPolygonEdges(std::vector<straight_skeleton::SkeletonFace>& faces,
                           const std::set<std::pair<Point, Point>, std::less<>>& polygonBoundaryEdges) const;
};

}  // namespace TestSkeleton
