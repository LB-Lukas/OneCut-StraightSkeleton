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

   private:
    SsPtr iss_;
    SsPtr oss_;

    std::map<std::pair<Point, Point>, int> polyEdgeToFaceIndexMap;

    std::vector<straight_skeleton::SkeletonFace> faces;

    std::vector<straight_skeleton::SkeletonFace> facesOuter;

    std::vector<straight_skeleton::SkeletonFace> facesInner;
    std::vector<Point> originalPolygonPoints;

    std::vector<straight_skeleton::SkeletonFace> innerSkeletonToFaces(SsPtr skeleton, int offset);

    std::vector<straight_skeleton::SkeletonFace> outerSkeletonToFaces(SsPtr skeleton, int offset);

    straight_skeleton::Point convertPoint(const Point& point) const;
};

}  // namespace TestSkeleton