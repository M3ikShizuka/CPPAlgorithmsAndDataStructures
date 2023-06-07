#pragma once
#include "swap.h"
#include "comparators.h"

using size_type = size_t;

template<typename DataType, typename Comparator>
static CONSTEXPR20 void mergeSortInternal(std::vector<DataType>& vec, size_type low, size_type high, Comparator comp);
template<typename DataType, typename Comparator>
static CONSTEXPR20 void merge(std::vector<DataType>& vec, size_type low, size_type divided_border, size_type high, Comparator comp);

template<typename DataType, typename Comparator>
CONSTEXPR20 void mergeSort(std::vector<DataType>& vec, Comparator comp = ComparatorGreater<DataType>()) {
    const auto size = vec.size();
    if (size < 2) {
        return;
    }

    mergeSortInternal(vec, 0, size - 1, comp);
}

template<typename DataType, typename Comparator>
static CONSTEXPR20 void mergeSortInternal(std::vector<DataType>& vec, size_type low, size_type high, Comparator comp) {
    // Note: Check before call function faster than call and check inside function.
    const auto divided_border = low + (high - low) / 2;

    // Divide array.
    // Divide left subarray.
    if (
            divided_border != 0
            &&
            low < divided_border
            ) {
        mergeSortInternal<DataType, Comparator>(vec, low, divided_border, comp);
    }

    // Divide right subarray.
    const auto new_low = divided_border + 1;
    if (new_low < high) {
        mergeSortInternal<DataType, Comparator>(vec, new_low, high, comp);
    }

    // Conquer and combine array data.
    merge(vec, low, divided_border, high, comp);
}

template<typename DataType, typename Comparator>
static CONSTEXPR20 void merge(std::vector<DataType>& vec, size_type low, size_type divided_border, size_type high, Comparator comp) {
    size_type left_subarray_index = 0,
        right_subarray_index = 0,
        main_array_index = low;
    const auto left_subarray_last_index = divided_border - low,
        right_subarray_last_index = high - (divided_border + 1);

    // Move data from main array to sub arrays using move semantics.
    std::vector<DataType> vec_left(
            std::make_move_iterator(vec.begin() + low),
            std::make_move_iterator(vec.begin() + divided_border + 1)
            ),
        vec_right(
            std::make_move_iterator(vec.begin() + divided_border + 1),
            std::make_move_iterator(vec.begin() + high + 1)
            );

    do {
        if (comp(vec_left[left_subarray_index], vec_right[right_subarray_index])) {
            vec[main_array_index] = std::move(vec_left[left_subarray_index]);
            left_subarray_index++;
        }
        else {
            vec[main_array_index] = std::move(vec_right[right_subarray_index]);
            right_subarray_index++;
        }

        main_array_index++;
    } while(
            left_subarray_index <= left_subarray_last_index
            &&
            right_subarray_index <= right_subarray_last_index
            );

    // Move the remaining elements to the main array.
    if (left_subarray_index != (left_subarray_last_index + 1)) {
        std::move(
                std::make_move_iterator(vec_left.begin() + left_subarray_index),
                std::make_move_iterator(vec_left.begin() + left_subarray_last_index + 1),
                vec.begin() + main_array_index
                );
    } else if (right_subarray_index != (right_subarray_last_index + 1)) {
        std::move(
                std::make_move_iterator(vec_right.begin() + right_subarray_index),
                std::make_move_iterator(vec_right.begin() + right_subarray_last_index + 1),
                vec.begin() + main_array_index
        );
    }
}