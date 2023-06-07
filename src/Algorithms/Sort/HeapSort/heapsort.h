#pragma once
#include "swap.h"
#include "comparators.h"

using size_type = size_t;

template<typename DataType, typename Comparator>
CONSTEXPR20 void heapSort(std::vector<DataType>& vec, Comparator comp = ComparatorGreater<DataType>()) {
    const auto size = vec.size();
    if (size < 2) {
        return;
    }

    // Build heap.
    heapifyStart(vec, vec.size(), comp);

    // Sort
    for (auto index = size - 1; index != 0; index--) {
        swap(vec, static_cast<size_type>(0), index);

        if (index > 1) { // Don't swap left child with peak when index == 1 inside heapify().
            heapify(vec, 0, index, comp);
        }
    }
}

template<typename DataType, typename Comparator>
CONSTEXPR20 void heapifyStart(std::vector<DataType>& vec, size_type size, Comparator comp) {
    auto index = size / 2;
    if (index != 0) {
        do {
            index--;
            heapify(vec, index, size, comp);
        } while (index != 0); // we can use unsigned type
    }
}

template<typename DataType, typename Comparator>
CONSTEXPR20 void heapify(std::vector<DataType>& vec, size_type current_element, size_type size, Comparator comp) {
    auto local_peek_node_index = current_element;
    const auto left_node_index = 2 * current_element + 1;

    // Check left node value.
    if (!comp(vec[left_node_index], vec[local_peek_node_index])) {
        local_peek_node_index = left_node_index;
    }

    const auto right_node_index = 2 * current_element + 2;

    // Check right node value.
    if (
            right_node_index < size &&
            !comp(vec[right_node_index], vec[local_peek_node_index])
            ) {
        local_peek_node_index = right_node_index;
    }

    if (local_peek_node_index != current_element) {
        // Swap elements
        swap(vec, current_element, local_peek_node_index);

        const auto left_node_index_next = 2 * local_peek_node_index + 1;
        if (left_node_index_next < size) {
            heapify(vec, local_peek_node_index, size, comp);
        }
    }
}