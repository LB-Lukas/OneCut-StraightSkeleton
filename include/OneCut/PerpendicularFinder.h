#pragma once

#include "IStraightSkeleton.h"
#include "StraightSkeletonTypes.h"
#include "utils/GeometryUtil.h"
#include "utils/IntersectionUtil.h"

namespace OneCut {

/**
 * @struct PerpSegment
 * @brief Represents a segment of a perpendicular fold with its associated face.
 */
struct PerpSegment {
    Point start;        ///< Starting point of the perpendicular segment
    Point end;          ///< Ending point of the perpendicular segment
    int faceIndex;      ///< Index of the face this segment belongs to
};

/**
 * @struct PerpendicularHit
 * @brief Represents the result of a perpendicular intersection calculation.
 */
struct PerpendicularHit {
    bool isValid;       ///< True if a valid intersection was found
    Point intersection; ///< Point of intersection (if valid)
    int edgeIndex;      ///< Index of the intersected edge
};

/**
 * @typedef PerpChain
 * @brief A sequence of connected perpendicular segments forming a complete fold line.
 */
typedef std::vector<PerpSegment> PerpChain;

/**
 * @class PerpendicularFinder
 * @brief Computes perpendicular folds for one-cut origami based on a straight skeleton.
 *
 * This class calculates the perpendicular fold lines that connect from polygon vertices
 * to the skeleton edges, used in the one-cut origami algorithm.
 */
class PerpendicularFinder {
   public:
    /// Maximum iterations allowed for finding perpendiculars
    static const int MAX_ITERATIONS = 30;
    /// X-dimension boundary of the paper (for validation)
    static const int PAPER_BORDER_X = 600;
    /// Y-dimension boundary of the paper (for validation)
    static const int PAPER_BORDER_Y = 600;

    /**
     * @brief Constructs a PerpendicularFinder for the given skeleton.
     * @param skeleton The straight skeleton to compute perpendiculars for.
     */
    PerpendicularFinder(const IStraightSkeleton& skeleton);

    /**
     * @brief Finds all perpendicular fold chains in the skeleton.
     * @return Vector of perpendicular chains, where each chain represents
     *         a complete fold line across multiple faces.
     *
     * @note The algorithm traces perpendiculars until they either:
     *       - Reach the paper boundary
     *       - Fail to find a valid intersection
     *       - Exceed MAX_ITERATIONS
     */
    std::vector<PerpChain> findPerpendiculars();

   private:
    const IStraightSkeleton& skeleton; ///< Reference to the straight skeleton

    /**
     * @brief Computes the intersection of a perpendicular from a vertex to a face edge.
     * @param vertex The starting vertex of the perpendicular.
     * @param face The face to compute intersection with.
     * @param edgeIndex Index of the edge to check against.
     * @return PerpendicularHit structure containing intersection results.
     */
    PerpendicularHit computePerpendicularIntersection(
        const Point& vertex, 
        const ISkeletonFace& face, 
        int edgeIndex);

    /**
     * @brief Finds the index of the edge in a face closest to a starting point.
     * @param face The face to search in.
     * @param startPoint The reference point for finding the nearest edge.
     * @return Index of the most relevant edge for perpendicular calculation.
     */
    int findEdgeIndex(const ISkeletonFace& face, const Point& startPoint) const;
};

}  // namespace OneCut