#pragma once

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

class GraphUtils {
public:
    /** 
     * @brief Compares two graphs for equality: Two graphs are equal if they have the same number of vertices, edges and faces
     * @todo Check if there are any other properties that need to be compared
     * @warning This function is not a complete graph isomorphism check, it only checks for the number of vertices, edges and faces
     * 
     * @param graph1 the first graph
     * @param graph2 the second graph
     * 
     * @return true if the graphs are equal, false otherwise
     */
    static bool areGraphsEqual(const PlanarGraph& graph1, const PlanarGraph& graph2); 
};


}