#pragma once
#include <type_traits>
#include <memory>
#include "head.h"
#include "comparators.h"

template <class DataType>
struct AVLNode {
    using size_type = size_t;

    char factor;
    size_type height;
};

template <class DataType, class Comparator = ComparatorGreater<DataType>>
class AVLTree {
private:
	using value_type = DataType;
	using const_reference = const DataType&;
	using size_type = size_t;

	std::shared_ptr<AVLNode<value_type>> head;
	Comparator comp{};

public:
	CONSTEXPR20 AVLTree() = default;

	CONSTEXPR20 explicit AVLTree(const value_type* start, const value_type* end) {
        // TODO
	}

	CONSTEXPR20 ~AVLTree() = default;

	CONSTEXPR20 void insert(value_type&& value) {
		insertElement(value);
	}

	CONSTEXPR20 void insert(const value_type& value) {
		insertElement(value);
	}

//	CONSTEXPR20 void remove(size_type index) {
//        // TODO
//	}
//
//	NODISCARD CONSTEXPR20 size_type size() const {
//        // TODO
//	}
//
//	NODISCARD CONSTEXPR20 bool isEmpty() const {
//        // TODO
//	}

private:
	CONSTEXPR20 void insertElement(const value_type& value) {
        // TODO
	}

	CONSTEXPR20 void balance(size_type current_element) {
        // TODO
	}
};