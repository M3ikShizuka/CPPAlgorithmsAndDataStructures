#pragma once
#include "swap.h"
#include "comparators.h"

using size_type = size_t;

template<typename DataType, typename Comparator>
CONSTEXPR20 void bubbleSort(std::vector<DataType>& vec, Comparator comp = ComparatorGreater<DataType>()) {
    const auto size = vec.size();
    if (size < 2) {
        return;
    }

    // Iterate over the array with a number equal to the size of the array minus one.
    for (size_type iteration = 0; iteration < size - 1; iteration++) {
        // For each iteration, go through all adjacent numbers, find the peak number among each pair, and swap their places in sort order.
        bool swapped = false;

        for (size_type sub_iteration = 0; sub_iteration < size - 1; sub_iteration++) {
            if (comp(vec[sub_iteration + 1], vec[sub_iteration])) {
                swap(vec, sub_iteration + 1, sub_iteration);
                swapped = true;
            }
        }

        if (!swapped) {
            // If no one sequence of two elements was swapped during the current iteration, the data are sorted.
            return;
        }
    }
}