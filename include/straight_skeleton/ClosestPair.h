#pragma once

#include "IClosestPair.h"
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <limits>

namespace straight_skeleton {
/**
 * @brief An interface for a Closest Pair Data Structure.
 * 
 * @tparam R The type of the first set.
 * @tparam B The type of the second set.
 * @tparam d A distance function that takes two objects of type R and B and returns a double.
 */
template <typename R, typename B, typename d>
class ClosestPair : public IClosestPair<R, B> {
   public:
    ClosestPair();

    /**
     * @brief Insert an object into the first set.
     * 
     * @param r Object of the type of the first set.
     */
    void insertFirst(const R &r) override;


    /**
     * @brief Insert an object into the second set.
     * 
     * @param b Object of the type of the second set.
     */
    void insertSecond(const B &b) override;

    /**
     * @brief Remove an object from the first set.
     * 
     * @param r Object of the type of the first set.
     */
    void removeFirst(const R &r) override;

    /**
     * @brief Remove an object from the second set.
     * 
     * @param b Object of the type of the second set.
     */
    void removeSecond(const B &b) override;

    /**
     * @brief Get the size of the first set.
     * 
     * @return size_t 
     */
    size_t firstSetSize() const override;

    /**
     * @brief Get the size of the second set.
     * 
     * @return size_t 
     */
    size_t secondSetSize() const override;

    /**
     * @brief Get the Min Pair object
     * 
     * @return std::pair<R, B> 
     */
    std::pair<R, B> getMinPair() const override;

    /**
     * @brief Extract the Min Pair object
     * 
     * @return std::pair<R, B> 
     */
    std::pair<R, B> extractMinPair() override;

    /**
     * @brief Check if one of the sets is empty
     * 
     * @return true one of the sets is empty
     * @return false both sets are not empty
     */
    bool isEmpty() const override;

    /**
     * @brief Check if the first set is empty
     * 
     * @return true the first set is empty
     * @return false the first set is not empty
     */
    bool isFirstSetEmpty() const override;

    /**
     * @brief Check if the second set is empty 
     * 
     * @return true the second set is empty
     * @return false the second set is not empty
     */
    bool isSecondSetEmpty() const override;

    /**
     * @brief Get the minimum distance of the closest pair.
     * 
     * @return double 
     */
    double getMinDistance() const override;

   private:
    std::vector<R> firstSet_;
    std::vector<B> secondSet_;
    d distanceFunction_;
};

}  // namespace straight_skeleton
