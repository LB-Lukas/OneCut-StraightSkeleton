#pragma once

#include <utility>

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Point_2.h>
#include <CGAL/Line_2.h>
#include <CGAL/Surface_mesh.h>

#include "StraightSkeleton/graphUtils.h" 


namespace Geometry {

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Point_2 Point;
typedef K::Line_2 Line;
typedef CGAL::Surface_mesh<Point> PlanarGraph;

class PerpendicularGraph {
public:
    /**
     * @brief Construct a new Perpendicular Graph object
     * 
     */
    PerpendicularGraph();

    /**
     * @brief Construct a new Perpendicular Graph object
     * 
     * @param skeletonGraph the straight skeleton graph
     * @param cutGraph the cut graph
     */
    PerpendicularGraph(const PlanarGraph& skeletonGraph, const PlanarGraph& cutGraph);

    /**
     * @brief This method adds perpendiculars to the perpendicularGraph by calculating the perpendicular bisectors for each skeleton vertex
     * 
     */
    void addPerpendiculars();

    /**
     * @brief Get the Perpendicular Graph object: the graph containing the perpendiculars
     * 
     * @return PlanarGraph (CGAL::Surface_mesh<K::Point_2>) 
     */
    PlanarGraph getPerpendicularGraph() const;

    /**
     * @brief Get the Cut Graph object: the graph containing the cut edges
     * 
     * @return PlanarGraph (CGAL::Surface_mesh<K::Point_2>)
     */
    PlanarGraph getCutGraph() const;

    /**
     * @brief Get the Straight Skeleton Graph object: the graph containing the straight skeleton
     * 
     * @return PlanarGraph (CGAL::Surface_mesh<K::Point_2>)
     */
    PlanarGraph getStraightSkeletonGraph() const;

    /**
     * @brief Get the Complete Graph object consisting of the straight skeleton, the cut edges and the perpendiculars
     * @todo Not yet implemented
     * @throws std::runtime_error as it is not yet implemented
     * 
     * @return PlanarGraph (CGAL::Surface_mesh<K::Point_2>)
     */
    PlanarGraph getCompleteGraph() const;


private:
    PlanarGraph cutGraph;
    PlanarGraph skeletonGraph;
    PlanarGraph perpendicularGraph;

    std::pair<Point, Point> calculatePerpendicularBisector(const Point& point, const std::pair<Point, Point>& edge);
};




}