# pragma once

namespace straight_skeleton {

/**
 * @enum VertexType
 * @brief Enumerates the types of vertices in a straight skeleton.
 * 
 * @var VertexType::NoType
 * No specific type.
 * 
 * @var VertexType::Skeleton
 * Vertex is part of the skeleton.
 * 
 * @var VertexType::CutGraph
 * Vertex is part of the cut graph.
 * 
 * @var VertexType::Both
 * Vertex is part of both the skeleton and the cut graph.
 */
enum class VertexType {
    NoType = 0,
    Skeleton = 1, 
    CutGraph = 2,
    Both = 3
};


/**
 * @enum EdgeType
 * @brief Enumerates the types of edges in a straight skeleton.
 * 
 * @var EdgeType::NoType
 * No specific type.
 * 
 * @var EdgeType::Skeleton
 * Edge is part of the skeleton.
 * 
 * @var EdgeType::CutGraph
 * Edge is part of the cut graph.
 * 
 * @var EdgeType::Both
 * Edge is part of both the skeleton and the cut graph.
 * 
 * @var EdgeType::Perpendicular
 * Edge is perpendicular.
 * 
 * @var EdgeType::Mountain
 * Edge represents a mountain fold.
 * 
 * @var EdgeType::Valley
 * Edge represents a valley fold.
 */
enum class EdgeType {
    NoType = 0,
    Skeleton = 1,
    CutGraph = 2, 
    Both = 3,
    Perpendicular = 4,
    Mountain = 5,
    Valley = 6
};

enum class FaceDirection {
    INTERIOR,
    EXTERIOR
};


}