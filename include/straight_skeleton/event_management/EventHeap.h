#pragma once

#include <iostream>
#include <stdexcept>
#include <unordered_map>
#include <vector>

namespace straight_skeleton {
/**
 * @brief A min-heap-like priority structure storing elements with weights.
 * @tparam T Type of element being stored.
 */
template <typename T>
class EventHeap {
   private:
    std::vector<T> heap;
    std::unordered_map<T, size_t> index_map;
    std::unordered_map<T, double> weight_map;

    static size_t parent(size_t i);

    static size_t left(size_t i);

    static size_t right(size_t i);

    void swap(size_t i, size_t j);

    void heapify_down(size_t i);

    void heapify_up(size_t i);

   public:
    EventHeap();

    explicit EventHeap(const std::vector<std::pair<T, double> > &element_weight_pairs);

    [[nodiscard]] double get_min_weight() const;

    [[nodiscard]] size_t get_size() const;

    void insert(const T &value, double weight);

    T extract_min();

    void remove(const T &value);

    const T &get_min() const;

    [[nodiscard]] bool is_empty() const;

    void print_heap() const;
};
}  // namespace straight_skeleton
