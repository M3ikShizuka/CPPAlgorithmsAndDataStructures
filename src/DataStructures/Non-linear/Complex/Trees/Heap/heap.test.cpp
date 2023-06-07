#include <gtest/gtest.h>
#include <memory>
#include "heap.h"

using test_data_type = int;
using test_data_count = size_t;

class HeapTest : public ::testing::Test {
protected:
    HeapTest() :
        array_values{ 3, 9, min_value_second, 5, min_value, 4, max_value_second, 7, max_value, 6 }
	{
	}

    const test_data_type max_value{ 100 };
    const test_data_type max_value_second{ 99 };
    const test_data_type min_value{ 1 };
    const test_data_type min_value_second{ 2 };
    static const test_data_count array_values_elements_count{ 10 };
	test_data_type array_values[array_values_elements_count];

public:
    class ObjectA {
    public:
//        CONSTEXPR20 ObjectA() = default;

        ObjectA() {
            std::cout << "constructor ObjectA()" << std::endl;
        }

        /*CONSTEXPR20*/ explicit ObjectA(test_data_type value) :
            value{ value }
        {
            std::cout << "constructor explicit ObjectA(test_data_type value)" << std::endl;
        }

        ~ObjectA() {
            std::cout << "destructor ~ObjectA()" << std::endl;
        }

        NODISCARD CONSTEXPR20 bool operator> (const ObjectA& obj) const {
            return this->value > obj.value;
        }

        NODISCARD CONSTEXPR20 bool operator< (const ObjectA& obj) const {
            return this->value < obj.value;
        }

//        NODISCARD CONSTEXPR20 bool operator== (const ObjectA& obj) {
//            return this->value == obj.value;
//        }

        NODISCARD CONSTEXPR20 test_data_type get() const {
            return value;
        }

    private:
        test_data_type value{};
    };
};

// GCC: "undefined reference" if variables defined inside HeapTest
const test_data_count HeapTest::array_values_elements_count;

NODISCARD CONSTEXPR20 bool operator==(const HeapTest::ObjectA& obj1, const HeapTest::ObjectA& obj2) {
    return obj1.get() == obj2.get();
}

NODISCARD CONSTEXPR20 bool operator>=(const HeapTest::ObjectA& obj1, const HeapTest::ObjectA& obj2) {
    return obj1.get() >= obj2.get();
}

NODISCARD CONSTEXPR20 bool operator<=(const HeapTest::ObjectA& obj1, const HeapTest::ObjectA& obj2) {
    return obj1.get() <= obj2.get();
}

using object_type = HeapTest::ObjectA;
using store_smart_ptr_type = std::shared_ptr<object_type>;

TEST_F(HeapTest, PeekMax) {
    Heap<test_data_type> heap{ array_values, array_values + array_values_elements_count };
    auto peek_value = heap.peek();
    ASSERT_EQ(peek_value, max_value);
}

TEST_F(HeapTest, PeekMin) {
    Heap<test_data_type, ComparatorLess<test_data_type>> heap{ array_values, array_values + array_values_elements_count };
    auto peek_value = heap.peek();
    ASSERT_EQ(peek_value, min_value);
}

TEST_F(HeapTest, HeapifyMaxInitializedByArray) {
    Heap<test_data_type> heap{ array_values, array_values + array_values_elements_count };
    auto prev_value = heap.peek();

    for (test_data_count index = 0; index < array_values_elements_count; index++) {
        auto current_value = heap.peek();
        ASSERT_GE(prev_value, current_value);
        prev_value = current_value;
        heap.remove(0);
    }
}

TEST_F(HeapTest, HeapifyMinInitializedByArray) {
    Heap<test_data_type, ComparatorLess<test_data_type>> heap{ array_values, array_values + array_values_elements_count };
    auto prev_value = heap.peek();

    for (test_data_count index = 0; index < array_values_elements_count; index++) {
        auto current_value = heap.peek();
        ASSERT_LE(prev_value, current_value);
        prev_value = current_value;
        heap.remove(0);
    }
}

TEST_F(HeapTest, HeapifyMaxObjectInitializedByArray) {
    object_type objects_array[array_values_elements_count];
    for (test_data_count index = 0; index < array_values_elements_count; index++) {
        objects_array[index] = object_type{ array_values[index] };
    }

    Heap<object_type> heap{ objects_array, objects_array + array_values_elements_count };
    auto prev_value = heap.peek();

    for (test_data_count index = 0; index < array_values_elements_count; index++) {
        auto current_value = heap.peek();
        ASSERT_GE(prev_value, current_value);
        prev_value = current_value;
        heap.remove(0);
    }
}

TEST_F(HeapTest, HeapifyMinObjectInitializedByArray) {
    object_type objects_array[array_values_elements_count];
    for (test_data_count index = 0; index < array_values_elements_count; index++) {
        objects_array[index] = object_type{array_values[index]};
    }

    Heap<object_type, ComparatorLess<object_type>> heap{objects_array, objects_array + array_values_elements_count};
    auto prev_value = heap.peek();

    for (test_data_count index = 0; index < array_values_elements_count; index++) {
        auto current_value = heap.peek();
        ASSERT_LE(prev_value, current_value);
        prev_value = current_value;
        heap.remove(0);
    }
}

TEST_F(HeapTest, HeapifyMaxPointersInitializedByArray) {
    store_smart_ptr_type objects_array[array_values_elements_count];
    for (test_data_count index = 0; index < array_values_elements_count; index++) {
        objects_array[index] = std::make_shared<object_type>(array_values[index]);
    }

    Heap<store_smart_ptr_type, decltype(compPtrMax<store_smart_ptr_type>)> heap{objects_array, objects_array + array_values_elements_count};
    auto prev_value = heap.peek();

    for (test_data_count index = 0; index < array_values_elements_count; index++) {
        auto current_value = heap.peek();
        ASSERT_GE(*prev_value, *current_value) << "heapify is incorrect when deleting.";
        prev_value = current_value;
        heap.remove(0);
    }
}

TEST_F(HeapTest, HeapifyMinPointersInitializedByArray) {
    store_smart_ptr_type objects_array[array_values_elements_count];
    for (test_data_count index = 0; index < array_values_elements_count; index++) {
        objects_array[index] = std::make_shared<object_type>(array_values[index]);
    }

    Heap<store_smart_ptr_type, decltype(compPtrMin<store_smart_ptr_type>)> heap{objects_array, objects_array + array_values_elements_count};
    auto prev_value = heap.peek();

    for (test_data_count index = 0; index < array_values_elements_count; index++) {
        auto current_value = heap.peek();
        ASSERT_LE(*prev_value, *current_value) << "heapify is incorrect when deleting.";
        prev_value = current_value;
        heap.remove(0);
    }
}

TEST_F(HeapTest, HeapifyMaxWhenInsertingAndDeleting) {
    Heap<test_data_type> heap{ array_values_elements_count };
    auto peek_value = array_values[0];

    for (test_data_type array_value : array_values) {
        if (peek_value < array_value) {
            peek_value = array_value;
        }

        heap.insert(array_value);
        ASSERT_EQ(peek_value, heap.peek()) << "heapify is incorrect when inserting.";
    }

    auto prev_value = peek_value;

    for (test_data_count index = 0; index < array_values_elements_count; index++) {
        auto current_value = heap.peek();
        ASSERT_GE(prev_value, current_value) << "heapify is incorrect when deleting.";
        prev_value = current_value;
        heap.remove(0);
    }
}

TEST_F(HeapTest, HeapifyMinWhenInsertingAndDeleting) {
    Heap<test_data_type, ComparatorLess<test_data_type>> heap{ array_values_elements_count };
    auto peek_value = array_values[0];

    for (test_data_type array_value : array_values) {
        if (peek_value > array_value) {
            peek_value = array_value;
        }

        heap.insert(array_value);
        ASSERT_EQ(peek_value, heap.peek()) << "heapify is incorrect when inserting.";
    }

    auto prev_value = peek_value;

    for (test_data_count index = 0; index < array_values_elements_count; index++) {
        auto current_value = heap.peek();
        ASSERT_LE(prev_value, current_value) << "heapify is incorrect when deleting.";
        prev_value = current_value;
        heap.remove(0);
    }
}

TEST_F(HeapTest, HeapifyMaxObjectWhenInsertingAndDeleting) {
    Heap<object_type> heap{ array_values_elements_count };
    auto peek_value = array_values[0];

    for (test_data_type array_value : array_values) {
        if (peek_value < array_value) {
            peek_value = array_value;
        }

        heap.insert(object_type{ array_value });
        ASSERT_EQ(object_type{ peek_value }, heap.peek()) << "heapify is incorrect when inserting.";
    }

    auto prev_value = object_type{ peek_value };

    for (test_data_count index = 0; index < array_values_elements_count; index++) {
        auto current_value = heap.peek();
        ASSERT_GE(prev_value, current_value) << "heapify is incorrect when deleting.";
        prev_value = current_value;
        heap.remove(0);
    }
}

TEST_F(HeapTest, HeapifyMinObjectWhenInsertingAndDeleting) {
    Heap<object_type, ComparatorLess<object_type>> heap{ array_values_elements_count };
    auto peek_value = array_values[0];

    for (test_data_type array_value : array_values) {
        if (peek_value > array_value) {
            peek_value = array_value;
        }

        heap.insert(object_type{ array_value });
        ASSERT_EQ(object_type{ peek_value }, heap.peek()) << "heapify is incorrect when inserting.";
    }

    auto prev_value = object_type{ peek_value };

    for (test_data_count index = 0; index < array_values_elements_count; index++) {
        auto current_value = heap.peek();
        ASSERT_LE(prev_value, current_value) << "heapify is incorrect when deleting.";
        prev_value = current_value;
        heap.remove(0);
    }
}

TEST_F(HeapTest, HeapifyMaxPointersInsertingAndDeletingWithCustomComparator) {
    Heap<store_smart_ptr_type, decltype(compPtrMax<store_smart_ptr_type>)> heap{array_values_elements_count };
    auto peek_value = array_values[0];

    for (test_data_type & array_value : array_values) {
        if (peek_value < array_value) {
            peek_value = array_value;
        }

        heap.insert(std::make_shared<object_type>(array_value));
        ASSERT_EQ(object_type(peek_value), *heap.peek()) << "heapify is incorrect when inserting.";
    }

    auto prev_value = heap.peek();

    for (test_data_count index = 0; index < array_values_elements_count; index++) {
        auto current_value = heap.peek();
        ASSERT_GE(*prev_value, *current_value) << "heapify is incorrect when deleting.";
        prev_value = current_value;
        heap.remove(0);
    }
}

TEST_F(HeapTest, HeapifyMinPointersInsertingAndDeletingWithCustomComparator) {
    Heap<store_smart_ptr_type, decltype(compPtrMin<store_smart_ptr_type>)> heap{array_values_elements_count };
    auto peek_value = array_values[0];

    for (test_data_type & array_value : array_values) {
        if (peek_value > array_value) {
            peek_value = array_value;
        }

        heap.insert(std::make_shared<object_type>(array_value));
        ASSERT_EQ(object_type(peek_value), *heap.peek()) << "heapify is incorrect when inserting.";
    }

    auto prev_value = heap.peek();

    for (test_data_count index = 0; index < array_values_elements_count; index++) {
        auto current_value = heap.peek();
        ASSERT_LE(*prev_value, *current_value) << "heapify is incorrect when deleting.";
        prev_value = current_value;
        heap.remove(0);
    }
}

TEST_F(HeapTest, Insertion) {
    Heap<test_data_type> heap{ array_values_elements_count };

    for (test_data_type array_value : array_values) {
        heap.insert(array_value);
    }

    ASSERT_EQ(heap.peek(), max_value);
}

TEST_F(HeapTest, Deletion) {
    Heap<test_data_type> heap{ array_values, array_values + array_values_elements_count };
    heap.remove(0);
    ASSERT_EQ(heap.peek(), max_value_second);
    // Code coverage index >= size.
    heap.remove(10);
}

//TEST_F(HeapTest, DeletionNegativeIndex) {
//    Heap<test_data_type> heap{ array_values, array_values + array_values_elements_count };
//    heap.deletion(-1);
//    ASSERT_EQ(heap.peek(), max_value_second);
//}
//
//TEST_F(HeapTest, DeletionIndexGreaterSize) {
//    Heap<test_data_type> heap{ array_values, array_values + array_values_elements_count };
//    heap.deletion(array_values_elements_count);
//    ASSERT_EQ(heap.peek(), max_value_second);
//}

TEST_F(HeapTest, Size) {
    Heap<test_data_type> heap{ array_values, array_values + array_values_elements_count };
    ASSERT_EQ(heap.size(), array_values_elements_count);
}

TEST_F(HeapTest, IsEmpty) {
    Heap<test_data_type> heap{ array_values_elements_count };
    ASSERT_TRUE(heap.isEmpty());
    heap.insert(array_values[0]);
    ASSERT_FALSE(heap.isEmpty());
}