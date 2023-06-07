#pragma once
#include <type_traits>
#include <memory>
#include "head.h"
#include "comparators.h"

template <class DataType, class Comparator = ComparatorGreater<DataType>>
class Heap {
private:
	using value_type = DataType;
	using const_reference = const DataType&;
	using size_type = size_t;

	std::vector<value_type> data_array{};
	Comparator comp{};

public:
	CONSTEXPR20 Heap() = default;

	CONSTEXPR20 explicit Heap(size_type size) {
		data_array.reserve(size);
	}

	CONSTEXPR20 explicit Heap(const value_type* start, const value_type* end) {
		const size_type size = end - start;

		data_array.resize(size);

		size_type it_index = 0;
		for (auto it = start; it != end; it++, it_index++) {
			data_array[it_index] = *it;
		}

		heapifyStart(size);
	}

	CONSTEXPR20 ~Heap() = default;

	CONSTEXPR20 void insert(value_type&& value) {
		insertElement(value);
	}

	CONSTEXPR20 void insert(const value_type& value) {
		insertElement(value);
	}

	CONSTEXPR20 void remove(size_type index) {
		const auto size = data_array.size();

		if (index >= size) {
			return;
		}

        const auto new_size = size - 1;
		swap(index, new_size);
		data_array.pop_back();
		heapifyStart(new_size);
	}

	NODISCARD CONSTEXPR20 const_reference peek() const noexcept(noexcept(data_array.front())) /* strengthened */ {
		// Return value from root node
		return data_array.front();
	}

	NODISCARD CONSTEXPR20 size_type size() const {
		return data_array.size();
	}

	NODISCARD CONSTEXPR20 bool isEmpty() const {
		return data_array.empty();
	}

	CONSTEXPR20 void reserve(size_type size) {
		data_array.reserve(size);
	}

private:
	CONSTEXPR20 void insertElement(const value_type& value) {
		data_array.push_back(value);
		const size_type size = data_array.size();

		if (size != 0) {
			heapifyStart(size);
		}
	}

	CONSTEXPR20 void heapifyStart(size_type size) {
        auto index = size / 2;
        if (index != 0) {
            do {
                index--;
                heapify(index);
            } while (index != 0); // we can use unsigned type
        }
	}

	CONSTEXPR20 void heapify(size_type current_element) {
		const auto size = data_array.size();
		auto local_peek_node_index = current_element;
		const auto left_node_index = 2 * current_element + 1;

		// Check left node value.
		if (comp(data_array[left_node_index], data_array[local_peek_node_index])) {
			local_peek_node_index = left_node_index;
		}

		const auto right_node_index = 2 * current_element + 2;

		// Check right node value.
		if (
			right_node_index < size &&
			comp(data_array[right_node_index], data_array[local_peek_node_index])
			) {
			local_peek_node_index = right_node_index;
		}
		
		if (local_peek_node_index != current_element) {
			// Swap elements
			this->swap(current_element, local_peek_node_index);

            const auto left_node_index_next = 2 * local_peek_node_index + 1;
            if (left_node_index_next < size) {
                heapify(local_peek_node_index);
            }
		}
	}

	CONSTEXPR20 void swap(size_type index_first, size_type index_second) {
		value_type value_temp = std::move(data_array[index_first]);
		data_array[index_first] = std::move(data_array[index_second]);
		data_array[index_second] = std::move(value_temp);
	}
};