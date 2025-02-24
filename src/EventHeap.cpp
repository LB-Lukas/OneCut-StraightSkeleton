#include "straight_skeleton/event_management/EventHeap.h"

#include "straight_skeleton/event_management/SkeletonEvent.h"
#include "straight_skeleton/event_management/SkeletonEventTriangle.h"

namespace straight_skeleton {
template <typename T>
size_t EventHeap<T>::parent(size_t i) {
    return (i - 1) / 2;
}

template <typename T>
size_t EventHeap<T>::left(size_t i) {
    return 2 * i + 1;
}

template <typename T>
size_t EventHeap<T>::right(size_t i) {
    return 2 * i + 2;
}

template <typename T>
void EventHeap<T>::swap(size_t i, size_t j) {
    T val_i = heap[i];
    T val_j = heap[j];
    std::swap(heap[i], heap[j]);
    index_map[val_i] = j;
    index_map[val_j] = i;
}

template <typename T>
void EventHeap<T>::heapify_down(size_t i) {
    while (true) {
        size_t l = left(i), r = right(i);
        size_t smallest = i;
        if (l < heap.size() && weight_map[heap[l]] < weight_map[heap[smallest]]) {
            smallest = l;
        }
        if (r < heap.size() && weight_map[heap[r]] < weight_map[heap[smallest]]) {
            smallest = r;
        }
        if (smallest != i) {
            swap(i, smallest);
            i = smallest;
        } else {
            break;
        }
    }
}

template <typename T>
void EventHeap<T>::heapify_up(size_t i) {
    while (i > 0) {
        size_t p = parent(i);
        if (weight_map[heap[i]] < weight_map[heap[p]]) {
            swap(i, p);
            i = p;
        } else {
            break;
        }
    }
}

template <typename T>
EventHeap<T>::EventHeap() = default;

template <typename T>
EventHeap<T>::EventHeap(const std::vector<std::pair<T, double>>& pairs) {
    for (auto& p : pairs) {
        insert(p.first, p.second);
    }
}

template <typename T>
double EventHeap<T>::get_min_weight() const {
    if (heap.empty()) {
        throw std::out_of_range("Heap is empty!");
    }
    return weight_map.at(heap[0]);
}

template <typename T>
size_t EventHeap<T>::get_size() const {
    return heap.size();
}

template <typename T>
void EventHeap<T>::insert(const T& value, double weight) {
    if (index_map.find(value) != index_map.end()) {
        throw std::invalid_argument("Duplicate values are not supported!");
    }
    heap.push_back(value);
    size_t idx = heap.size() - 1;
    index_map[value] = idx;
    weight_map[value] = weight;
    heapify_up(idx);
}

template <typename T>
T EventHeap<T>::extract_min() {
    if (heap.empty()) {
        throw std::out_of_range("Heap is empty!");
    }
    T min_val = heap[0];
    T last_val = heap.back();
    heap[0] = last_val;
    index_map[last_val] = 0;

    heap.pop_back();
    index_map.erase(min_val);

    if (!heap.empty()) {
        heapify_down(0);
    }
    return min_val;
}

template <typename T>
void EventHeap<T>::remove(const T& value) {
    auto it = index_map.find(value);
    if (it == index_map.end()) {
        throw std::invalid_argument("Value not found in the heap!");
    }
    size_t i = it->second;
    T last_val = heap.back();
    heap[i] = last_val;
    index_map[last_val] = i;

    heap.pop_back();
    index_map.erase(value);

    if (i < heap.size()) {
        heapify_down(i);
        heapify_up(i);
    }
}

template <typename T>
const T& EventHeap<T>::get_min() const {
    if (heap.empty()) {
        throw std::out_of_range("Heap is empty!");
    }
    return heap[0];
}

template <typename T>
bool EventHeap<T>::is_empty() const {
    return heap.empty();
}

template <typename T>
void EventHeap<T>::print_heap() const {
    std::cout << "print heap, size: " << get_size() << std::endl;
    for (auto& val : heap) {
        std::cout << val << " " << weight_map.at(val) << " " << index_map.at(val) << std::endl;
    }
    std::cout << std::endl;
}

// explicit instantiation
template class EventHeap<SkeletonEventPtr>;
}  // namespace straight_skeleton
