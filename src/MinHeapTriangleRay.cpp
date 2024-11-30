#include <vector>
#include <unordered_map>
#include <stdexcept>
#include <iostream>

template <typename T>
class MinHeapTriangleRay {
private:
    std::vector<T> heap; // Vector to represent the heap
    std::unordered_map<T, size_t> index_map; // Map to track the indices of elements in the heap
    std::unordered_map<T, double> weight_map; // Map to track the weights of elements in the heap

    // Helper function to get the parent index
    size_t parent(size_t i) { return (i - 1) / 2; }

    // Helper function to get the left child index
    size_t left(size_t i) { return 2 * i + 1; }

    // Helper function to get the right child index
    size_t right(size_t i) { return 2 * i + 2; }

    // Swap two elements in the heap and update the index_map
    void swap(size_t i, size_t j) {
        std::swap(heap[i], heap[j]);
        index_map[heap[i]] = i;
        index_map[heap[j]] = j;
    }

    // Heapify down to maintain the heap property
    void heapify_down(size_t i) {
        size_t smallest = i;
        size_t l = left(i);
        size_t r = right(i);

        double l_weight = weight_map[heap[l]];
        double r_weight = weight_map[heap[r]];

        if (l < heap.size() && l_weight < r_weight)
            smallest = l;
        else if (r < heap.size() && r_weight < l_weight)
            smallest = r;

        if (smallest != i) {
            swap(i, smallest);
            heapify_down(smallest);
        }
    }

    // Heapify up to maintain the heap property
    void heapify_up(double i) {
        double parent_weight = weight_map[heap[parent(i)]];
        double el_weight = weight_map[heap[i]];

        while (i > 0 && el_weight < parent_weight) {
            swap(i, parent(i));
            i = parent(i);
        }
    }

public:

    MinHeapTriangleRay() {}

    /**
     *
     * @param element_weight_pairs : vector of elements with corresponding weights
     */
    MinHeapTriangleRay(const std::vector<std::pair<T, double>>& element_weight_pairs) {
        for(size_t i = 0; i < element_weight_pairs.size(); i++) {
            this->insert(element_weight_pairs[i].first, element_weight_pairs[i].second);
        }
    }

    // Insert a value into the heap
    void insert(const T& value, const double& weight) {
        if (index_map.find(value) != index_map.end()) {
            throw std::invalid_argument("Duplicate values are not supported!");
        }

        heap.push_back(value);
        size_t index = heap.size() - 1;
        index_map[value] = index;
        weight_map[value] = weight;
        heapify_up(index);
    }

    // Remove the smallest element (root) from the heap
    T extract_min() {
        if (heap.empty()) {
            throw std::out_of_range("Heap is empty!");
        }

        T min_value = heap[0];
        T last_value = heap.back();

        // Replace root with the last element and remove it
        heap[0] = last_value;
        index_map[last_value] = 0;
        heap.pop_back();
        index_map.erase(min_value);

        if (!heap.empty()) {
            heapify_down(0);
        }

        return min_value;
    }

    // Remove a specific element from the heap
    void remove(const T& value) {
        if (index_map.find(value) == index_map.end()) {
            throw std::invalid_argument("Value not found in the heap!");
        }

        size_t index = index_map[value];
        T last_value = heap.back();

        // Replace the target element with the last element and remove it
        heap[index] = last_value;
        index_map[last_value] = index;
        heap.pop_back();
        index_map.erase(value);

        // Heapify to maintain heap property
        if (index < heap.size()) {
            heapify_down(index);
            heapify_up(index);
        }
    }

    // Get the smallest element without removing it
    const T& get_min() const {
        if (heap.empty()) {
            throw std::out_of_range("Heap is empty!");
        }
        return heap[0];
    }

    // Check if the heap is empty
    bool is_empty() const {
        return heap.empty();
    }

    // Print the heap for debugging
    void print_heap() const {
        for (const auto& val : heap) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }
};
