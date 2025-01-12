#include <gtest/gtest.h>
#include <stdexcept>
#include <vector>
#include <utility>
#include "straight_skeleton/event_management/EventHeap.h"  // Adjust this include path as needed

// We will use MinHeapTriangleRay<int> for demonstration purposes.
//
// If you want to test the class with std::shared_ptr<Triangle>,
// you would need to include the Triangle definition and adapt the
// test accordingly.

namespace straight_skeleton {
    /*
    // Test fixture for MinHeapTriangleRay<int>
    class MinHeapTriangleRayTest : public ::testing::Test {};

    // Test constructing an empty heap and basic properties
    TEST_F(MinHeapTriangleRayTest, EmptyHeapTest) {
        EXPECT_TRUE(heap.is_empty());
        EXPECT_EQ(heap.get_size(), 0);

        // Accessing min from an empty heap should throw
        EXPECT_THROW(heap.get_min(), std::out_of_range);
        EXPECT_THROW(heap.get_min_weight(), std::out_of_range);
        EXPECT_THROW(heap.extract_min(), std::out_of_range);
    }

    // Test constructing with an initial list of (element, weight) pairs
    TEST_F(MinHeapTriangleRayTest, ConstructWithValues) {
        std::vector<std::pair<int, double>> values = {
            {10, 2.5},
            {20, 1.0},
            {30, 3.0},
            {40, 0.75}
        };

        EventHeap<int> heap(values);

        EXPECT_FALSE(heap.is_empty());
        EXPECT_EQ(heap.get_size(), values.size());

        // Minimum element should be 40 (weight = 0.75)
        EXPECT_EQ(heap.get_min(), 40);
        EXPECT_NEAR(heap.get_min_weight(), 0.75, 1e-9);
    }

    // Test insertion and retrieving min
    TEST_F(MinHeapTriangleRayTest, InsertAndGetMin) {
        EventHeap<int> heap;

        // Insert some values
        heap.insert(10, 5.0);
        heap.insert(20, 1.0);
        heap.insert(30, 3.0);
        heap.insert(40, 2.0);

        EXPECT_EQ(heap.get_size(), 4u);
        // Minimum element should be 20 (weight = 1.0)
        EXPECT_EQ(heap.get_min(), 20);
        EXPECT_NEAR(heap.get_min_weight(), 1.0, 1e-9);

        // Insert something smaller
        heap.insert(50, 0.5);
        EXPECT_EQ(heap.get_min(), 50);
        EXPECT_NEAR(heap.get_min_weight(), 0.5, 1e-9);
    }

    // Test duplicate insertion throws an exception
    TEST_F(MinHeapTriangleRayTest, DuplicateInsertionThrows) {
        EventHeap<int> heap;

        heap.insert(10, 2.0);
        // Insert the same element again
        EXPECT_THROW(heap.insert(10, 3.0), std::invalid_argument);
    }

    // Test extracting the minimum element
    TEST_F(MinHeapTriangleRayTest, ExtractMin) {
        // Prepare a heap
        std::vector<std::pair<int, double>> values = {
            {10, 2.5},
            {20, 1.0},
            {30, 3.0},
            {40, 0.75}
        };
        EventHeap<int> heap(values);

        // The minimum is 40 (weight = 0.75)
        EXPECT_EQ(heap.extract_min(), 40);
        // Next minimum is 20 (weight = 1.0)
        EXPECT_EQ(heap.extract_min(), 20);

        EXPECT_EQ(heap.get_size(), 2u);  // two extractions

        // Check new min
        EXPECT_EQ(heap.get_min(), 10);
        EXPECT_NEAR(heap.get_min_weight(), 2.5, 1e-9);
    }

    // Test removing an arbitrary value
    TEST_F(MinHeapTriangleRayTest, RemoveValue) {
        // Prepare a heap
        std::vector<std::pair<int, double>> values = {
            {10, 2.5},
            {20, 1.0},
            {30, 3.0},
            {40, 0.75}
        };
        EventHeap<int> heap(values);

        EXPECT_EQ(heap.get_size(), 4u);
        EXPECT_EQ(heap.get_min(), 40);

        // Remove 20 (weight = 1.0)
        heap.remove(20);
        EXPECT_EQ(heap.get_size(), 3u);
        EXPECT_EQ(heap.get_min(), 40);
        EXPECT_THROW(heap.remove(20), std::invalid_argument);

        // Remove 40 (weight = 0.75)
        heap.remove(40);
        EXPECT_EQ(heap.get_size(), 2u);

        // Now the min should be 10
        EXPECT_EQ(heap.get_min(), 10);
        EXPECT_NEAR(heap.get_min_weight(), 2.5, 1e-9);

        // Removing non-existent element
        EXPECT_THROW(heap.remove(999), std::invalid_argument);
    }

    // Test that removing from an empty heap throws as expected
    TEST_F(MinHeapTriangleRayTest, RemoveFromEmptyHeapThrows) {
        EventHeap<int> heap;
        EXPECT_THROW(heap.remove(123), std::invalid_argument);
    }

    // Example test using print_heap (not strictly necessary, but can be helpful)
    TEST_F(MinHeapTriangleRayTest, PrintHeapDebug) {
        std::vector<std::pair<int, double>> values = {
            {3, 0.3}, {2, 0.2}, {1, 0.1}
        };
        EventHeap<int> heap(values);

        // You can capture stdout and compare if you want, but usually this is
        // just for debugging. We'll just call it to ensure it doesn't crash.
        heap.print_heap();
        SUCCEED();  // If it didn't crash or throw, we succeed
    }

    // You can add more tests as you see fit for edge cases, large inputs, etc.
    */
}