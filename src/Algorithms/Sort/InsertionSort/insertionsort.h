#pragma once
#include "swap.h"
#include "comparators.h"

using size_type = size_t;

template<typename DataType, typename Comparator>
CONSTEXPR20 void insertionSort(std::vector<DataType>& vec, Comparator comp = ComparatorGreater<DataType>()) {
    const auto size = vec.size();
    if (size < 2) {
        return;
    }

    // Iterate over the array from element index = 1 to the number of array elements.
    for (size_type iteration = 1; iteration < size; iteration++) {

        // If the left element is the peak element against the current one.
        if (comp(vec[iteration], vec[iteration - 1])) {
            DataType temp = std::move(vec[iteration]);
            size_type sub_iteration = iteration;

            do {
                sub_iteration--;
                // Move the element by the current index to the right.
                vec[sub_iteration + 1] = std::move(vec[sub_iteration]);
            } while (
                    sub_iteration != 0 // (we can use unsigned type)
                    &&
                    // If the left element is the peak element against the current one.
                    comp(temp, vec[sub_iteration - 1])
                );

            // Move our key element by the current index.
            vec[sub_iteration] = std::move(temp);
        }
    }
}