#pragma once
#include "ClosestPairDataStructure.h"
#include <unordered_set>
#include <queue>


/**
    * @brief naive approach in runtixme O(n)
    * ToDo: implement Advanced approach
    */

namespace straight_skeleton {
    template<typename RObj, typename BObj, typename DistFn>
    class NaiveClosestPair : public ClosestPairDataStructure<RObj, BObj> {
    public:
        /**
         * @brief Constructor accepting a distance function.
         * @param distFn A callable that takes (RObj, BObj) and returns a double.
         */
        explicit NaiveClosestPair(DistFn distFn);

        // Override interface methods
        void insertR(const RObj &r) override;

        void insertB(const BObj &b) override;

        void removeR(const RObj &r) override;

        void removeB(const BObj &b) override;

        size_t rSetSize() override;

        size_t bSetSize() override;

        std::optional<std::pair<RObj, BObj> > getMinPair() override;

        std::optional<std::pair<RObj, BObj> > extractMinPair() override;

        bool empty() const override;

    private:
        // Internal representation
        std::unordered_set<RObj> rSet_;
        std::unordered_set<BObj> bSet_;

        // Define a structure to hold pairs and their distances
        struct Pair {
            RObj r;
            BObj b;
            double dist;

            Pair(const RObj &r_, const BObj &b_, double d_)
                : r(r_), b(b_), dist(d_) {
            }
        };

        // Comparator for the min-heap
        struct ComparePair {
            bool operator()(const Pair &lhs, const Pair &rhs) const {
                return lhs.dist > rhs.dist; // Min-heap based on distance
            }
        };

        // Priority queue (min-heap) to store all possible pairs
        std::priority_queue<Pair, std::vector<Pair>, ComparePair> minHeap_;

        // Distance function
        DistFn distFn_;

        // Helper to rebuild the heap (used after removals)
        void rebuildHeap();
    };
}