#pragma once

#include <vector>
#include "StraightSkeletonTypes.h"

namespace straight_skeleton {

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

    /**
     * @brief Checks if an edge is part of a graph
     * 
     * @param graph the graph
     * @param edge the edge
     * 
     * @return true if the edge is part of the graph, false otherwise
     */
    static bool isEdgeInGraph(const PlanarGraph& graph, const std::pair<Point, Point>& edge);

    /** 
     * @brief Get all the faces that contain a certain vertex
     * 
     * @param graph the graph
     * @param vertex the vertex
     * @return vector<FaceIndex> the faces that contain the vertex
     */
    static std::vector<FaceIndex> getFacesContainingVertex(const PlanarGraph& graph, const VertexIndex& vertex);

    /**
     * @deprecated use getHalfedgesOfFace instead
     * @brief Get all the edges around a face
     * 
     * @param graph the graph
     * @param face the face
     * @return vector<EdgeIndex> the edges around the face
     */
    static std::vector<EdgeIndex> getEdgesOfFace(const PlanarGraph& graph, const FaceIndex& face);

    /**
     * @brief Get the Halfedges around a face
     * 
     * @param graph the graph
     * @param face  the face
     * @return std::vector<HalfedgeIndex> the edges around the face
     */
    static std::vector<HalfedgeIndex> getHalfedgesOfFace(const PlanarGraph& graph, const FaceIndex& face);

    /**
     * @brief Get the Degree of a vertex (number of incident edges)
     * 
     * @param graph the graph
     * @param vertex teh vertex 
     * @return size_t the degree of the vertex
     */
    static size_t getDegree(const PlanarGraph& graph, const VertexIndex& vertex);
};


}