#pragma once

#include <utility>
#include <vector>
#include <map>
#include <iostream>

// CGAL headers for kernel and surface mesh
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/Straight_skeleton_2/IO/print.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/create_straight_skeleton_2.h>
#include <CGAL/draw_straight_skeleton_2.h>

#include "SkeletonFace.h"
#include "IStraightSkeleton.h"

namespace TestSkeleton {

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

typedef K::Point_2 Point;
typedef CGAL::Polygon_2<K> Polygon_2;
typedef CGAL::Straight_skeleton_2<K> Ss;

typedef std::shared_ptr<Ss> SsPtr;
typedef CGAL::Surface_mesh<Point> SurfaceMesh;

class TestStraightSkeleton : public straight_skeleton::IStraightSkeleton {
   public:
    /// Construct the straight skeleton from the input polygon (vertices in counterclockwise order)
    explicit TestStraightSkeleton(const std::vector<Point>& polygon_points);

    /// Returns a vector of edges (each as a pair of Points) in the computed skeleton.
    std::vector<std::pair<Point, Point>> getEdges() const;

    size_t faceCount() const override;
    const straight_skeleton::SkeletonFace& face(size_t i) const override;

    SurfaceMesh graph;

    private:
    SsPtr iss_;
    std::vector<straight_skeleton::SkeletonFace> faces;
    std::vector<Point> originalPolygonPoints;

    std::vector<straight_skeleton::SkeletonFace> skeletonToFaces(SsPtr skeleton) const;

    straight_skeleton::Point convertPoint(const Point& point) const;

    bool isOuterFace(const Ss::Face_handle& face) const;
  
    };

}  // namespace TestSkeleton
