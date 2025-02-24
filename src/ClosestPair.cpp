#include "straight_skeleton/ClosestPair.h"
#include "straight_skeleton/StraightSkeletonTypes.h"
#include "straight_skeleton/EuclideanDistance.h"

namespace straight_skeleton {

template <typename R, typename B, typename d>
ClosestPair<R, B, d>::ClosestPair() : distanceFunction_(d()) {}

template <typename R, typename B, typename d>
void ClosestPair<R, B, d>::insertFirst(const R &r) {
    firstSet_.push_back(r);
}

template <typename R, typename B, typename d>
void ClosestPair<R, B, d>::insertSecond(const B &b) {
    secondSet_.push_back(b);
}

template <typename R, typename B, typename d>
void ClosestPair<R, B, d>::removeFirst(const R &r) {
    firstSet_.erase(std::remove(firstSet_.begin(), firstSet_.end(), r), firstSet_.end());
}

template <typename R, typename B, typename d>
void ClosestPair<R, B, d>::removeSecond(const B &b) {
    secondSet_.erase(std::remove(secondSet_.begin(), secondSet_.end(), b), secondSet_.end());
}

template <typename R, typename B, typename d>
size_t ClosestPair<R, B, d>::firstSetSize() const {
    return firstSet_.size();
}

template <typename R, typename B, typename d>
size_t ClosestPair<R, B, d>::secondSetSize() const {
    return secondSet_.size();
}

template <typename R, typename B, typename d>
bool ClosestPair<R, B, d>::isEmpty() const {
    return firstSet_.empty() || secondSet_.empty();
}

template <typename R, typename B, typename d>
bool ClosestPair<R, B, d>::isFirstSetEmpty() const {
    return firstSet_.empty();
}

template <typename R, typename B, typename d>
bool ClosestPair<R, B, d>::isSecondSetEmpty() const {
    return secondSet_.empty();
}

template <typename R, typename B, typename d>
double ClosestPair<R, B, d>::getMinDistance() const {
    if (isEmpty()) {
        throw std::runtime_error("One of the sets is empty");
    }

    double minDistance = std::numeric_limits<double>::max();
    for (const auto &r : firstSet_) {
        for (const auto &b : secondSet_) {
            double dist = distanceFunction_(r, b);
            if (dist < minDistance) {
                minDistance = dist;
            }
        }
    }
    return minDistance;
}

template <typename R, typename B, typename d>
std::pair<R, B> ClosestPair<R, B, d>::getMinPair() const {
    if (isEmpty()) {
        throw std::runtime_error("One of the sets is empty");
    }

    double minDistance = std::numeric_limits<double>::max();
    std::pair<R, B> minPair;

    for (const auto &r : firstSet_) {
        for (const auto &b : secondSet_) {
            double dist = distanceFunction_(r, b);
            if (dist < minDistance) {
                minDistance = dist;
                minPair = {r, b};
            }
        }
    }
    return minPair;
}

template <typename R, typename B, typename d>
std::pair<R, B> ClosestPair<R, B, d>::extractMinPair() {
    if (isEmpty()) {
        throw std::runtime_error("One of the sets is empty");
    }

    double minDistance = std::numeric_limits<double>::max();
    std::pair<R, B> minPair;

    for (const auto &r : firstSet_) {
        for (const auto &b : secondSet_) {
            double dist = distanceFunction_(r, b);
            if (dist < minDistance) {
                minDistance = dist;
                minPair = {r, b};
            }
        }
    }

    removeFirst(minPair.first);
    removeSecond(minPair.second);

    return minPair;
}


template class straight_skeleton::ClosestPair<
    Point,
    Point,
    straight_skeleton::EuclideanDistance
>;


}  // namespace straight_skeleton