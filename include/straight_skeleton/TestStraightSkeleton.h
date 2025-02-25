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

#include "IStraightSkeleton.h"
#include "SkeletonFace.h"

namespace TestSkeleton {

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

typedef K::Point_2 Point2D;
typedef CGAL::Polygon_2<K> Polygon_2;
typedef CGAL::Straight_skeleton_2<K> Ss;

typedef std::shared_ptr<Ss> SsPtr;
typedef CGAL::Surface_mesh<Point2D> SurfaceMesh;

class TestStraightSkeleton : public straight_skeleton::IStraightSkeleton {
   public:
    /// Construct the straight skeleton from the input polygon (vertices in counterclockwise order)
    explicit TestStraightSkeleton(const std::vector<Point2D>& polygon_points);

    /// Returns a vector of edges (each as a pair of Points) in the computed skeleton.
    std::vector<std::pair<Point2D, Point2D>> getEdges() const;

    size_t faceCount() const override;
    const straight_skeleton::SkeletonFace& face(size_t i) const override;

    SurfaceMesh graph;

   private:
    SsPtr iss_;
    std::vector<straight_skeleton::SkeletonFace> faces;
    std::vector<Point2D> originalPolygonPoints;

    std::vector<straight_skeleton::SkeletonFace> skeletonToFaces(SsPtr skeleton) const;

    straight_skeleton::Point2D convertPoint(const Point2D& point) const;

    bool isOuterFace(const Ss::Face_handle& face) const;
};

}  // namespace TestSkeleton
