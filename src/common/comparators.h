#pragma once
#include "head.h"

// Functors
template<class DataType>
class ComparatorGreater {
public:
    NODISCARD CONSTEXPR20 bool operator()(const DataType& child, const DataType& peek) const {
        return child > peek;
    }
};

template<class DataType>
class ComparatorLess {
public:
    NODISCARD CONSTEXPR20 bool operator()(const DataType& child, const DataType& peek) const {
        return child < peek;
    }
};

// Lambdas are actually functors.
template<typename DataType>
CONSTEXPR20 auto compPtrMax = [](const DataType& child, const DataType& peek) -> bool {
    return *child > *peek;
};

template<typename DataType>
CONSTEXPR20 auto compPtrMin = [](const DataType& child, const DataType& peek) -> bool {
    return *child < *peek;
};