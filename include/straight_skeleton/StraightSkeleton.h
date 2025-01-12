#pragma once

#include "event_management/EventHeap.h"
#include "event_management/SkeletonEvent.h"
#include "StraightSkeletonTypes.h"



namespace straight_skeleton {
    class StraightSkeleton {
    private:
        EventHeap<SkeletonEventPtr> event_heap;
    public:
        /// \brief Constructs the straight skeleton from a polygon.
        explicit StraightSkeleton(const std::vector<Point>& polygon_points);

    //private:
        //todo: make private
        PlanarGraph graph;
        // Internal graph where edges/vertices will be stored.

        /// \brief Computes a “ray” to be used in the skeleton logic.
        /// \throws std::runtime_error if lines are parallel or no intersection found.
        static Line computeRay(const Point& l_point,
                        const Point& ll_point,
                        const Point& r_point,
                        const Point& rr_point);

        /// \brief Set up the EventHeap
        void processPolygon(const std::vector<Point>& polygon_points);

        /// \brief Example placeholders for edge/vertex event logic if you expand further:
        void handleVertexEvent(std::shared_ptr<VertexEvent> vertex_event);
        void handleEdgeEvent(const std::shared_ptr<EdgeEvent> &edge_event);

        void handleSplitEvent(std::shared_ptr<SplitEvent> split_event);

        /// \brief Example debug/trial function
        bool testPolygonConnection();

        void processRayCalculation();
    };
}

