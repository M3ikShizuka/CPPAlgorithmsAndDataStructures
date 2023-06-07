#include <gtest/gtest.h>
#include "mergesort.h"

using test_data_type = int;
using test_data_count = size_t;

class MergeSortTest : public ::testing::Test {
protected:
    MergeSortTest() :
            array_values{ 3, 6, min_value_second, 5, min_value, 4, max_value_second, 7, max_value, 6 }
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
        CONSTEXPR20 ObjectA() = default;

        CONSTEXPR20 explicit ObjectA(test_data_type value) :
                value{ value }
        {

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

// GCC: "undefined reference" if variables defined inside MergeSortTest
const test_data_count MergeSortTest::array_values_elements_count;

NODISCARD CONSTEXPR20 bool operator==(const MergeSortTest::ObjectA& obj1, const MergeSortTest::ObjectA& obj2) {
    return obj1.get() == obj2.get();
}

NODISCARD CONSTEXPR20 bool operator>=(const MergeSortTest::ObjectA& obj1, const MergeSortTest::ObjectA& obj2) {
    return obj1.get() >= obj2.get();
}

NODISCARD CONSTEXPR20 bool operator<=(const MergeSortTest::ObjectA& obj1, const MergeSortTest::ObjectA& obj2) {
    return obj1.get() <= obj2.get();
}

using object_type = MergeSortTest::ObjectA;
using store_smart_ptr_type = std::shared_ptr<object_type>;

TEST_F(MergeSortTest, Greater) {
    std::vector<test_data_type> vec(array_values, array_values + array_values_elements_count);

    mergeSort(vec, ComparatorGreater<test_data_type>());
    auto prev_value = vec[0];

    for (test_data_count index = 0; index < array_values_elements_count; index++) {
        auto current_value = vec[index];
        ASSERT_GE(prev_value, current_value);
        prev_value = current_value;
    }
}

TEST_F(MergeSortTest, Less) {
    std::vector<test_data_type> vec(array_values, array_values + array_values_elements_count);

    mergeSort(vec, ComparatorLess<test_data_type>());
    auto prev_value = vec[0];

    for (test_data_count index = 0; index < array_values_elements_count; index++) {
        auto current_value = vec[index];
        ASSERT_LE(prev_value, current_value);
        prev_value = current_value;
    }
}

TEST_F(MergeSortTest, GreaterObject) {
    object_type objects_array[array_values_elements_count];
    for (test_data_count index = 0; index < array_values_elements_count; index++) {
        objects_array[index] = object_type{ array_values[index] };
    }

    std::vector<object_type> vec(objects_array, objects_array + array_values_elements_count);

    mergeSort(vec, ComparatorGreater<object_type>());
    auto prev_obj = vec[0];

    for (test_data_count index = 0; index < array_values_elements_count; index++) {
        auto current_obj = vec[index];
        ASSERT_GE(prev_obj, current_obj);
        prev_obj = current_obj;
    }
}

TEST_F(MergeSortTest, LessObject) {
    object_type objects_array[array_values_elements_count];
    for (test_data_count index = 0; index < array_values_elements_count; index++) {
        objects_array[index] = object_type{ array_values[index] };
    }

    std::vector<object_type> vec(objects_array, objects_array + array_values_elements_count);

    mergeSort(vec, ComparatorLess<object_type>());
    auto prev_obj = vec[0];

    for (test_data_count index = 0; index < array_values_elements_count; index++) {
        auto current_obj = vec[index];
        ASSERT_LE(prev_obj, current_obj);
        prev_obj = current_obj;
    }
}

TEST_F(MergeSortTest, GreaterPointersWithCustomComparator) {
    store_smart_ptr_type objects_array[array_values_elements_count];
    for (test_data_count index = 0; index < array_values_elements_count; index++) {
        objects_array[index] = std::make_shared<object_type>(array_values[index]);
    }

    std::vector<store_smart_ptr_type> vec(objects_array, objects_array + array_values_elements_count);

    mergeSort(vec, compPtrMax<store_smart_ptr_type>);
    auto prev_obj = vec[0];

    for (test_data_count index = 0; index < array_values_elements_count; index++) {
        auto current_obj_ptr = vec[index];
        ASSERT_GE(*prev_obj, *current_obj_ptr) << "heapify is incorrect when deleting.";
        prev_obj = current_obj_ptr;
    }
}

TEST_F(MergeSortTest, LessPointersWithCustomComparator) {
    store_smart_ptr_type objects_array[array_values_elements_count];
    for (test_data_count index = 0; index < array_values_elements_count; index++) {
        objects_array[index] = std::make_shared<object_type>(array_values[index]);
    }

    std::vector<store_smart_ptr_type> vec(objects_array, objects_array + array_values_elements_count);

    mergeSort(vec, compPtrMin<store_smart_ptr_type>);
    auto prev_obj = vec[0];

    for (test_data_count index = 0; index < array_values_elements_count; index++) {
        auto current_obj_ptr = vec[index];
        ASSERT_LE(*prev_obj, *current_obj_ptr) << "heapify is incorrect when deleting.";
        prev_obj = current_obj_ptr;
    }
}

TEST_F(MergeSortTest, SizeLessTwo) {
    std::vector<test_data_type> vec(array_values, array_values + 1);
    mergeSort(vec, ComparatorGreater<test_data_type>());
}