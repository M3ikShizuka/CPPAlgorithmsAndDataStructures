#include <gtest/gtest.h>
#include <memory>
#include "avltree.h"

using test_data_type = int;
using test_data_count = size_t;

class AVLTreeTest : public ::testing::Test {
protected:
    AVLTreeTest() :
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

// GCC: "undefined reference" if variables defined inside AVLTreeTest
const test_data_count AVLTreeTest::array_values_elements_count;

NODISCARD CONSTEXPR20 bool operator==(const AVLTreeTest::ObjectA& obj1, const AVLTreeTest::ObjectA& obj2) {
    return obj1.get() == obj2.get();
}

NODISCARD CONSTEXPR20 bool operator>=(const AVLTreeTest::ObjectA& obj1, const AVLTreeTest::ObjectA& obj2) {
    return obj1.get() >= obj2.get();
}

NODISCARD CONSTEXPR20 bool operator<=(const AVLTreeTest::ObjectA& obj1, const AVLTreeTest::ObjectA& obj2) {
    return obj1.get() <= obj2.get();
}

using object_type = AVLTreeTest::ObjectA;
using store_smart_ptr_type = std::shared_ptr<object_type>;

TEST_F(AVLTreeTest, Insert) {
    AVLTree<test_data_type> avltree{};
    avltree.insert(31);
    avltree.insert(21);
    avltree.insert(32);
    avltree.insert(42);
    avltree.insert(11);
}

//TEST_F(AVLTreeTest, AVLTreeifyMaxInitializedByArray) {
//    AVLTree<test_data_type> avltree{ array_values, array_values + array_values_elements_count };
//    auto prev_value = avltree.peek();
//
//    for (test_data_count index = 0; index < array_values_elements_count; index++) {
//        auto current_value = avltree.peek();
//        ASSERT_GE(prev_value, current_value);
//        prev_value = current_value;
//        avltree.remove(0);
//    }
//}
//
//TEST_F(AVLTreeTest, AVLTreeifyMinInitializedByArray) {
//    AVLTree<test_data_type, ComparatorLess<test_data_type>> avltree{ array_values, array_values + array_values_elements_count };
//    auto prev_value = avltree.peek();
//
//    for (test_data_count index = 0; index < array_values_elements_count; index++) {
//        auto current_value = avltree.peek();
//        ASSERT_LE(prev_value, current_value);
//        prev_value = current_value;
//        avltree.remove(0);
//    }
//}
//
//TEST_F(AVLTreeTest, AVLTreeifyMaxObjectInitializedByArray) {
//    object_type objects_array[array_values_elements_count];
//    for (test_data_count index = 0; index < array_values_elements_count; index++) {
//        objects_array[index] = object_type{ array_values[index] };
//    }
//
//    AVLTree<object_type> avltree{ objects_array, objects_array + array_values_elements_count };
//    auto prev_value = avltree.peek();
//
//    for (test_data_count index = 0; index < array_values_elements_count; index++) {
//        auto current_value = avltree.peek();
//        ASSERT_GE(prev_value, current_value);
//        prev_value = current_value;
//        avltree.remove(0);
//    }
//}
//
//TEST_F(AVLTreeTest, AVLTreeifyMinObjectInitializedByArray) {
//    object_type objects_array[array_values_elements_count];
//    for (test_data_count index = 0; index < array_values_elements_count; index++) {
//        objects_array[index] = object_type{array_values[index]};
//    }
//
//    AVLTree<object_type, ComparatorLess<object_type>> avltree{objects_array, objects_array + array_values_elements_count};
//    auto prev_value = avltree.peek();
//
//    for (test_data_count index = 0; index < array_values_elements_count; index++) {
//        auto current_value = avltree.peek();
//        ASSERT_LE(prev_value, current_value);
//        prev_value = current_value;
//        avltree.remove(0);
//    }
//}
//
//TEST_F(AVLTreeTest, AVLTreeifyMaxPointersInitializedByArray) {
//    store_smart_ptr_type objects_array[array_values_elements_count];
//    for (test_data_count index = 0; index < array_values_elements_count; index++) {
//        objects_array[index] = std::make_shared<object_type>(array_values[index]);
//    }
//
//    AVLTree<store_smart_ptr_type, decltype(compPtrMax<store_smart_ptr_type>)> avltree{objects_array, objects_array + array_values_elements_count};
//    auto prev_value = avltree.peek();
//
//    for (test_data_count index = 0; index < array_values_elements_count; index++) {
//        auto current_value = avltree.peek();
//        ASSERT_GE(*prev_value, *current_value) << "avltreeify is incorrect when deleting.";
//        prev_value = current_value;
//        avltree.remove(0);
//    }
//}
//
//TEST_F(AVLTreeTest, AVLTreeifyMinPointersInitializedByArray) {
//    store_smart_ptr_type objects_array[array_values_elements_count];
//    for (test_data_count index = 0; index < array_values_elements_count; index++) {
//        objects_array[index] = std::make_shared<object_type>(array_values[index]);
//    }
//
//    AVLTree<store_smart_ptr_type, decltype(compPtrMin<store_smart_ptr_type>)> avltree{objects_array, objects_array + array_values_elements_count};
//    auto prev_value = avltree.peek();
//
//    for (test_data_count index = 0; index < array_values_elements_count; index++) {
//        auto current_value = avltree.peek();
//        ASSERT_LE(*prev_value, *current_value) << "avltreeify is incorrect when deleting.";
//        prev_value = current_value;
//        avltree.remove(0);
//    }
//}
//
//TEST_F(AVLTreeTest, AVLTreeifyMaxWhenInsertingAndDeleting) {
//    AVLTree<test_data_type> avltree{ array_values_elements_count };
//    auto peek_value = array_values[0];
//
//    for (test_data_type array_value : array_values) {
//        if (peek_value < array_value) {
//            peek_value = array_value;
//        }
//
//        avltree.insert(array_value);
//        ASSERT_EQ(peek_value, avltree.peek()) << "avltreeify is incorrect when inserting.";
//    }
//
//    auto prev_value = peek_value;
//
//    for (test_data_count index = 0; index < array_values_elements_count; index++) {
//        auto current_value = avltree.peek();
//        ASSERT_GE(prev_value, current_value) << "avltreeify is incorrect when deleting.";
//        prev_value = current_value;
//        avltree.remove(0);
//    }
//}
//
//TEST_F(AVLTreeTest, AVLTreeifyMinWhenInsertingAndDeleting) {
//    AVLTree<test_data_type, ComparatorLess<test_data_type>> avltree{ array_values_elements_count };
//    auto peek_value = array_values[0];
//
//    for (test_data_type array_value : array_values) {
//        if (peek_value > array_value) {
//            peek_value = array_value;
//        }
//
//        avltree.insert(array_value);
//        ASSERT_EQ(peek_value, avltree.peek()) << "avltreeify is incorrect when inserting.";
//    }
//
//    auto prev_value = peek_value;
//
//    for (test_data_count index = 0; index < array_values_elements_count; index++) {
//        auto current_value = avltree.peek();
//        ASSERT_LE(prev_value, current_value) << "avltreeify is incorrect when deleting.";
//        prev_value = current_value;
//        avltree.remove(0);
//    }
//}
//
//TEST_F(AVLTreeTest, AVLTreeifyMaxObjectWhenInsertingAndDeleting) {
//    AVLTree<object_type> avltree{ array_values_elements_count };
//    auto peek_value = array_values[0];
//
//    for (test_data_type array_value : array_values) {
//        if (peek_value < array_value) {
//            peek_value = array_value;
//        }
//
//        avltree.insert(object_type{ array_value });
//        ASSERT_EQ(object_type{ peek_value }, avltree.peek()) << "avltreeify is incorrect when inserting.";
//    }
//
//    auto prev_value = object_type{ peek_value };
//
//    for (test_data_count index = 0; index < array_values_elements_count; index++) {
//        auto current_value = avltree.peek();
//        ASSERT_GE(prev_value, current_value) << "avltreeify is incorrect when deleting.";
//        prev_value = current_value;
//        avltree.remove(0);
//    }
//}
//
//TEST_F(AVLTreeTest, AVLTreeifyMinObjectWhenInsertingAndDeleting) {
//    AVLTree<object_type, ComparatorLess<object_type>> avltree{ array_values_elements_count };
//    auto peek_value = array_values[0];
//
//    for (test_data_type array_value : array_values) {
//        if (peek_value > array_value) {
//            peek_value = array_value;
//        }
//
//        avltree.insert(object_type{ array_value });
//        ASSERT_EQ(object_type{ peek_value }, avltree.peek()) << "avltreeify is incorrect when inserting.";
//    }
//
//    auto prev_value = object_type{ peek_value };
//
//    for (test_data_count index = 0; index < array_values_elements_count; index++) {
//        auto current_value = avltree.peek();
//        ASSERT_LE(prev_value, current_value) << "avltreeify is incorrect when deleting.";
//        prev_value = current_value;
//        avltree.remove(0);
//    }
//}
//
//TEST_F(AVLTreeTest, AVLTreeifyMaxPointersInsertingAndDeletingWithCustomComparator) {
//    AVLTree<store_smart_ptr_type, decltype(compPtrMax<store_smart_ptr_type>)> avltree{array_values_elements_count };
//    auto peek_value = array_values[0];
//
//    for (test_data_type & array_value : array_values) {
//        if (peek_value < array_value) {
//            peek_value = array_value;
//        }
//
//        avltree.insert(std::make_shared<object_type>(array_value));
//        ASSERT_EQ(object_type(peek_value), *avltree.peek()) << "avltreeify is incorrect when inserting.";
//    }
//
//    auto prev_value = avltree.peek();
//
//    for (test_data_count index = 0; index < array_values_elements_count; index++) {
//        auto current_value = avltree.peek();
//        ASSERT_GE(*prev_value, *current_value) << "avltreeify is incorrect when deleting.";
//        prev_value = current_value;
//        avltree.remove(0);
//    }
//}
//
//TEST_F(AVLTreeTest, AVLTreeifyMinPointersInsertingAndDeletingWithCustomComparator) {
//    AVLTree<store_smart_ptr_type, decltype(compPtrMin<store_smart_ptr_type>)> avltree{array_values_elements_count };
//    auto peek_value = array_values[0];
//
//    for (test_data_type & array_value : array_values) {
//        if (peek_value > array_value) {
//            peek_value = array_value;
//        }
//
//        avltree.insert(std::make_shared<object_type>(array_value));
//        ASSERT_EQ(object_type(peek_value), *avltree.peek()) << "avltreeify is incorrect when inserting.";
//    }
//
//    auto prev_value = avltree.peek();
//
//    for (test_data_count index = 0; index < array_values_elements_count; index++) {
//        auto current_value = avltree.peek();
//        ASSERT_LE(*prev_value, *current_value) << "avltreeify is incorrect when deleting.";
//        prev_value = current_value;
//        avltree.remove(0);
//    }
//}
//
//TEST_F(AVLTreeTest, Insertion) {
//    AVLTree<test_data_type> avltree{ array_values_elements_count };
//
//    for (test_data_type array_value : array_values) {
//        avltree.insert(array_value);
//    }
//
//    ASSERT_EQ(avltree.peek(), max_value);
//}
//
//TEST_F(AVLTreeTest, Deletion) {
//    AVLTree<test_data_type> avltree{ array_values, array_values + array_values_elements_count };
//    avltree.remove(0);
//    ASSERT_EQ(avltree.peek(), max_value_second);
//}
//
//TEST_F(AVLTreeTest, Size) {
//    AVLTree<test_data_type> avltree{ array_values, array_values + array_values_elements_count };
//    ASSERT_EQ(avltree.size(), array_values_elements_count);
//}
//
//TEST_F(AVLTreeTest, IsEmpty) {
//    AVLTree<test_data_type> avltree{ array_values_elements_count };
//    ASSERT_TRUE(avltree.isEmpty());
//    avltree.insert(array_values[0]);
//    ASSERT_FALSE(avltree.isEmpty());
//}