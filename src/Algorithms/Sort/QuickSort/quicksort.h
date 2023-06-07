#pragma once
#include "swap.h"
#include "comparators.h"

using size_type = size_t;

template<typename DataType, typename Comparator>
static CONSTEXPR20 void quickSortInternal(std::vector<DataType>& vec, size_type low, size_type high, Comparator comp);
template<typename DataType, typename Comparator>
static CONSTEXPR20 size_type partition(std::vector<DataType>& vec, size_type low, size_type high);
template<typename DataType, typename Comparator>
NODISCARD static CONSTEXPR20 bool searchLessThanPivot(std::vector<DataType>& vec, DataType pivot, size_type index_search_greater, size_type high, Comparator comp);

template<typename DataType, typename Comparator>
CONSTEXPR20 void quickSort(std::vector<DataType>& vec, Comparator comp = ComparatorGreater<DataType>()) {
    const auto size = vec.size();
    if (size < 2) {
        return;
    }

    quickSortInternal<DataType, Comparator>(vec, 0, size - 1, comp);
}

template<typename DataType, typename Comparator>
static CONSTEXPR20 void quickSortInternal(std::vector<DataType>& vec, size_type low, size_type high, Comparator comp) {
    // Note: Check before call function faster than call and check inside function.
    // Sorting and separation.
    const auto pivot_index = partition<DataType, Comparator>(vec, low, high, comp);

    // Sorting left part.
    const auto new_high = pivot_index - 1;
    if (
            pivot_index != 0 &&
            low < new_high
    ) {
        quickSortInternal<DataType, Comparator>(vec, low, new_high, comp);
    }

    // Sorting right part.
    if (pivot_index + 1 < high) {
        quickSortInternal<DataType, Comparator>(vec, pivot_index + 1, high, comp);
    }
}

template<typename DataType, typename Comparator>
static CONSTEXPR20 size_type partition(std::vector<DataType>& vec, size_type low, size_type high, Comparator comp) {
    // Always choose the last element as the pivot.
    const auto pivot = vec[high];
    size_type second_pointer = high;

    for (size_type index_search_greater = low; index_search_greater < high; index_search_greater++) {
        if (!comp(vec[index_search_greater], pivot)) {
            second_pointer = index_search_greater;
            if (!searchLessThanPivot<DataType, Comparator>(vec, pivot, index_search_greater, high, comp)) {
                return second_pointer;
            }
        }
    }

    // Finally, pivot will be swapped with greater element for which less element than pivot won't be found.
    return second_pointer;
}

template<typename DataType, typename Comparator>
NODISCARD static CONSTEXPR20 bool searchLessThanPivot(std::vector<DataType>& vec, DataType pivot, size_type index_search_greater, size_type high, Comparator comp) {
    for (size_type index_search_less = index_search_greater + 1; index_search_less < high; index_search_less++) {
        if (comp(vec[index_search_less], pivot)) {
            // swap the element which is greater than pivot with the element which is less than pivot.
            swap(vec, index_search_greater, index_search_less);
            return true;
        }
    }

    // No element less than pivot wasn't found. Swap the greater element with pivot.
    swap(vec, index_search_greater, high);
    return false;
}