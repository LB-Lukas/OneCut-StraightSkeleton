#pragma once
#include <optional>
#include <utility>

#include "../StraightSkeletonTypes.h"

// maintain the closest distance
namespace straight_skeleton {
template <typename RObj, typename BObj>
class ClosestPairDataStructure {
   public:
    virtual ~ClosestPairDataStructure() = default;

    /**
     * @brief Insert an object into set R.
     * @param r The object to insert.
     */
    virtual void insertR(const RObj &r) = 0;

    /**
     * @brief Insert an object into set B.
     * @param b The object to insert.
     */
    virtual void insertB(const BObj &b) = 0;

    /**
     * @brief Remove an object from set R.
     * @param r The object to remove.
     */
    virtual void removeR(const RObj &r) = 0;

    /**
     * @brief Remove an object from set B.
     * @param b The object to remove.
     */
    virtual void removeB(const BObj &b) = 0;

    virtual size_t rSetSize() = 0;

    virtual size_t bSetSize() = 0;

    /**
     * @brief Get the pair (r, b) with the minimal distance.
     * @return An optional pair containing the closest pair, or std::nullopt if either set is empty.
     */
    virtual std::optional<std::pair<RObj, BObj> > getMinPair() = 0;

    /**
     * @brief Extract the pair (r, b) with the minimal distance and remove it from the data structure.
     * @return An optional pair containing the closest pair, or std::nullopt if either set is empty.
     */
    virtual std::optional<std::pair<RObj, BObj> > extractMinPair() = 0;

    /**
     * @brief Check if either set R or set B is empty.
     * @return True if either set is empty, false otherwise.
     */
    virtual bool empty() const = 0;
};
}  // namespace straight_skeleton
