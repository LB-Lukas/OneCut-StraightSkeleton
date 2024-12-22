#pragma once

#include "doublyConnectedEdgeList.h"

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Line_2.h>
#include <CGAL/Point_2.h>

#include <CGAL/intersections.h>
#include <CGAL/squared_distance_2.h>
#include <CGAL/Surface_mesh.h>


typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Point_2 Point;
typedef K::Line_2 Line;
typedef K::Vector_2 Vector;
typedef K::Direction_2 Direction;

typedef CGAL::Surface_mesh<Point> PlanarGraph;

namespace Geometry {
    class StraightSkeleton {
        std::shared_ptr<DCEL> skeletonGraph;

    public:
        StraightSkeleton(const std::vector<Point>& polygon_points);

        // helper methods
        static Line computeRay(const Point& l_point, const Point& ll_point, const Point& r_point, const Point& rr_point);

        PlanarGraph graph;
    };
}

