#pragma once
#include "swap.h"
#include "comparators.h"

using size_type = size_t;

template<typename DataType, typename Comparator>
CONSTEXPR20 void selectionSort(std::vector<DataType>& vec, Comparator comp = ComparatorGreater<DataType>()) {
    const auto size = vec.size();
    if (size < 2) {
        return;
    }

    // Iterate over the array with a number equal to the size of the array minus one.
    for (size_type iteration = 0; iteration < size - 1; iteration++) {
        size_type smallest_element = iteration;

        // Search for the peak (minimum/maximum) element from all unsorted elements of each cell of the array.
        for (size_type sub_iteration = iteration + 1; sub_iteration < size; sub_iteration++) {
            if (comp(vec[sub_iteration], vec[smallest_element])) {
                smallest_element = sub_iteration;
            }
        }

        if (smallest_element != iteration) {
            // If a peak element is found, swap it with the element by the iteration index.
            swap(vec, smallest_element, iteration);
        }
    }
}