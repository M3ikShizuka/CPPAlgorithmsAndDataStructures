#pragma once
#include <vector>
#include "head.h"

template<typename DataType, typename SizeType>
CONSTEXPR20 void swap(std::vector<DataType>& vec, SizeType index_first, SizeType index_second) {
    DataType value_temp = std::move(vec[index_first]);
    vec[index_first] = std::move(vec[index_second]);
    vec[index_second] = std::move(value_temp);
}