//
// Created by user on 11/11/24.
//

#ifndef FOLDING_H
#define FOLDING_H
#include "straightSkeleton.h"

#endif //FOLDING_H

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Line_2.h>
#include <CGAL/Point_2.h>

#include <CGAL/intersections.h>
#include <CGAL/squared_distance_2.h>
#include <CGAL/Surface_mesh.h>

namespace Geometry {
    enum class FoldType {
        Convex, Reflex, None
    };

    class Folding {
    protected:
        PlanarGraph graph;

        CGAL::Surface_mesh<CGAL::Point_2<CGAL::Epeck>>::Property_map<CGAL::SM_Halfedge_index, FoldType> foldtype_map;

        std::vector<std::pair<Point, Point>> getEdges(FoldType foldType);
    public:
        Folding();

        // export
        std::vector<Point> getVertices();

        std::vector<std::pair<Point, Point>> getMountains();

        std::vector<std::pair<Point, Point>> getValleys();

        static Folding getFolding(const std::vector<Point> &polygon_points);
    };
}