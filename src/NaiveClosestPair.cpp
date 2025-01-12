#include "straight_skeleton/event_management/NaiveClosestPair.h"
#include "straight_skeleton/event_management/SkeletonEventRay.h"
#include "straight_skeleton/event_management/SkeletonEventTriangle.h"

#include <algorithm>

namespace straight_skeleton {
    // Constructor
    template <typename RObj, typename BObj, typename DistFn>
    NaiveClosestPair<RObj, BObj, DistFn>::NaiveClosestPair(DistFn distFn)
        : distFn_(distFn) {}

    // Insert into Set R
    template <typename RObj, typename BObj, typename DistFn>
    void NaiveClosestPair<RObj, BObj, DistFn>::insertR(const RObj& r) {
        // element already inserted
        if (rSet_.find(r) != rSet_.end()) {
            throw std::invalid_argument("NaiveClosestPair::insertR: Duplicate R object.");
        }
        rSet_.insert(r);

        // Compute distances to all B objects and add to heap
        for (const auto& b : bSet_) {
            double d = distFn_(r, b);
            minHeap_.emplace(r, b, d);
        }
    }

    // Insert into Set B
    template <typename RObj, typename BObj, typename DistFn>
    void NaiveClosestPair<RObj, BObj, DistFn>::insertB(const BObj& b) {
        // element already inserted
        if (bSet_.find(b) != bSet_.end()) {
            throw std::invalid_argument("NaiveClosestPair::insertB: Duplicate B object.");
        }
        bSet_.insert(b);

        // Compute distances to all R objects and add to heap
        for (const auto& r : rSet_) {
            double d = distFn_(r, b);
            minHeap_.emplace(r, b, d);
        }
    }

    // Remove from R
    template <typename RObj, typename BObj, typename DistFn>
    void NaiveClosestPair<RObj, BObj, DistFn>::removeR(const RObj& r) {
        if (rSet_.find(r) == rSet_.end()) {
            throw std::invalid_argument("NaiveClosestPair::removeR: R object not found.");
        }
        rSet_.erase(r);
        rebuildHeap(); // Rebuild the heap to remove all pairs involving r
    }

    // Remove from B
    template <typename RObj, typename BObj, typename DistFn>
    void NaiveClosestPair<RObj, BObj, DistFn>::removeB(const BObj& b) {
        if (bSet_.find(b) == bSet_.end()) {
            throw std::invalid_argument("NaiveClosestPair::removeB: B object not found.");
        }
        bSet_.erase(b);
        rebuildHeap(); // Rebuild the heap to remove all pairs involving b
    }

    // Get amount of elements in R
    template<typename RObj, typename BObj, typename DistFn>
    size_t NaiveClosestPair<RObj, BObj, DistFn>::rSetSize() {
        return rSet_.size();
    }

    // get amount of elements in B
    template<typename RObj, typename BObj, typename DistFn>
    size_t NaiveClosestPair<RObj, BObj, DistFn>::bSetSize() {
        return bSet_.size();
    }

    // todo: restructure
    // Get the minimal pair without removing it
    template <typename RObj, typename BObj, typename DistFn>
    std::optional<std::pair<RObj, BObj>> NaiveClosestPair<RObj, BObj, DistFn>::getMinPair() {
        while (!minHeap_.empty()) {
            const Pair& top = minHeap_.top();
            // Check if both r and b are still present
            if (rSet_.find(top.r) != rSet_.end() && bSet_.find(top.b) != bSet_.end()) {
                return std::make_pair(top.r, top.b);
            }
            // Invalid pair, remove it
            minHeap_.pop();
        }
        return std::nullopt; // No valid pairs
    }

    // Extract the minimal pair and remove it from the heap
    template <typename RObj, typename BObj, typename DistFn>
    std::optional<std::pair<RObj, BObj>> NaiveClosestPair<RObj, BObj, DistFn>::extractMinPair() {
        auto minPairOpt = getMinPair();
        if (minPairOpt.has_value()) {
            // Remove this pair from the heap by popping it
            minHeap_.pop();

            auto [rObj, bObj] = *minPairOpt;

            this->removeB(bObj);
            this->removeR(rObj);

            return minPairOpt;
        }
        return std::nullopt;
    }

    // Check if the data structure is empty
    template <typename RObj, typename BObj, typename DistFn>
    bool NaiveClosestPair<RObj, BObj, DistFn>::empty() const {
        return rSet_.empty() || bSet_.empty();
    }

    // Rebuild the heap by re-inserting all valid pairs
    template <typename RObj, typename BObj, typename DistFn>
    void NaiveClosestPair<RObj, BObj, DistFn>::rebuildHeap() {
        // Clear the heap
        while (!minHeap_.empty()) {
            minHeap_.pop();
        }

        // Re-insert all valid pairs
        for (const auto& r : rSet_) {
            for (const auto& b : bSet_) {
                double d = distFn_(r, b);
                minHeap_.emplace(r, b, d);
            }
        }
    }

    // todo: implement distFunct
    // Explicit template instantiation
    double distanceFunc(const SkeletonEventRay& r, const SkeletonEventTriangle& b);

    double distanceFunc(const Point& a, const Point& b) {
        Vector dist_vector = a - b;
        return sqrt(CGAL::to_double(dist_vector.squared_length()));
    }

    template class NaiveClosestPair<SkeletonEventRay, SkeletonEventTriangle,
            double(*)(const SkeletonEventRay&, const SkeletonEventTriangle&)>;

    template class NaiveClosestPair<Point, Point, double(*)(const Point&, const Point&)>;

}