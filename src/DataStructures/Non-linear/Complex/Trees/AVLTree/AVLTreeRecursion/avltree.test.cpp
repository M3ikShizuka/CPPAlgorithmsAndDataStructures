#include <gtest/gtest.h>
#include <vector>
#include "avltree.h"

using test_data_type = int;
using test_data_count = size_t;

class AVLTreeRecursionTest : public ::testing::Test {
protected:
    AVLTreeRecursionTest() = default;

    static const test_data_count array_values_elements_count{ 7 };
    std::vector<test_data_type> array_values {30, 35, 40, 20, 10, 24, 39};
    std::vector<test_data_type> array_values_result {30, 20, 10, 24, 39, 35, 40};
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

        NODISCARD CONSTEXPR20 bool operator>= (const ObjectA& obj) const {
            return this->value >= obj.value;
        }

        NODISCARD CONSTEXPR20 bool operator<= (const ObjectA& obj) const {
            return this->value <= obj.value;
        }

        NODISCARD CONSTEXPR20 bool operator== (const ObjectA& obj) const {
            return this->value == obj.value;
        }

        NODISCARD CONSTEXPR20 bool operator!= (const ObjectA& obj) const {
            return this->value != obj.value;
        }

        NODISCARD CONSTEXPR20 test_data_type get() const {
            return value;
        }

        CONSTEXPR20 void set(const test_data_type& val) {
            value = val;
        }

    private:
        test_data_type value{};
    };
};

// GCC: "undefined reference" if variables defined inside AVLTreeRecursionTest
const test_data_count AVLTreeRecursionTest::array_values_elements_count;

std::ostream& operator<<(std::ostream& os, const AVLTreeRecursionTest::ObjectA &obj) {
    return os << obj.get();
}

template <class T>
std::ostream& operator<<(std::ostream& os, const std::shared_ptr<T> &psobj) {
    return os << *psobj;
}

using object_type = AVLTreeRecursionTest::ObjectA;
using store_smart_ptr_type = std::shared_ptr<object_type>;

/*
input: 33 13 53 9 21 61 8 11 32 14 54 10

 R----13(h: 4)
   L----9(h: 3)
   |  L----8(h: 1)
   |  R----11(h: 2)
   |     L----10(h: 1)
   R----33(h: 3)
      L----21(h: 2)
      |  L----14(h: 1)
      |  R----32(h: 1)
      R----54(h: 2)
         L----53(h: 1)
         R----61(h: 1)

store: 13 9 8 11 10 33 21 14 32 54 53 61
 */

TEST_F(AVLTreeRecursionTest, Insert) {
    std::cout << "TEST_F(AVLTreeRecursionTest, Insert) start" << std::endl;
    AVLTreeRecursion<test_data_type> avltree{};

    std::cout << "Insert values to avl tree" << std::endl;
    for (const auto value : array_values) {
        // rotate: 40, 10, 24, 39
        avltree.insert(value);
    }

    avltree.printTree(avltree.getRoot());

    // Insert same value.
    avltree.insert(array_values[0]);

    ASSERT_TRUE(avltree.verifyCorrectness(array_values_result)) << "AVL tree stores data incorrectly!";
    std::cout << "TEST_F(AVLTreeRecursionTest, Insert) end" << std::endl;
}

TEST_F(AVLTreeRecursionTest, InsertObject) {
    std::cout << "TEST_F(AVLTreeRecursionTest, InsertObject) start" << std::endl;
    object_type objects_array[array_values_elements_count];
    std::vector<object_type> objects_array_result(array_values_elements_count);

    for (test_data_count index = 0; index < array_values_elements_count; index++) {
        objects_array[index] = object_type{array_values[index]};
        objects_array_result[index] = object_type{array_values_result[index]};
    }

    AVLTreeRecursion<object_type> avltree{};

    std::cout << "Insert objects to avl tree" << std::endl;
    for (const auto &obj : objects_array) {
        avltree.insert(obj);
        avltree.printTree(avltree.getRoot());
        std::cout << "=============" << std::endl;
    }

    avltree.printTree(avltree.getRoot());

    ASSERT_TRUE(avltree.verifyCorrectness(objects_array_result)) << "AVL tree stores objects data incorrectly!";
    std::cout << "TEST_F(AVLTreeRecursionTest, InsertObject) end" << std::endl;
}

TEST_F(AVLTreeRecursionTest, InsertPointers) {
    std::cout << "TEST_F(AVLTreeRecursionTest, InsertPointers) start" << std::endl;
    store_smart_ptr_type objects_array[array_values_elements_count];
    std::vector<store_smart_ptr_type> objects_array_result(array_values_elements_count);

    for (test_data_count index = 0; index < array_values_elements_count; index++) {
        objects_array[index] = std::make_shared<object_type>(array_values[index]);
        objects_array_result[index] = std::make_shared<object_type>(array_values_result[index]);
    }

    AVLTreeRecursion<store_smart_ptr_type> avltree{};

    std::cout << "Insert smart pointers to avl tree" << std::endl;
    for (const auto &obj : objects_array) {
        avltree.insert(obj);
    }

    avltree.printTree(avltree.getRoot());

    ASSERT_TRUE(avltree.verifyCorrectness(objects_array_result)) << "AVL tree stores objects data incorrectly!";
    std::cout << "TEST_F(AVLTreeRecursionTest, InsertPointers) end" << std::endl;
}

TEST_F(AVLTreeRecursionTest, Delete) {
    std::cout << "TEST_F(AVLTreeRecursionTest, Delete) start" << std::endl;
    AVLTreeRecursion<object_type> avltreeObj{};
    std::vector<store_smart_ptr_type> objects_array_result(array_values_elements_count);

    for (test_data_count index = 0; index < array_values_elements_count; index++) {
        avltreeObj.insert(object_type{array_values[index]});
        objects_array_result[index] = std::make_shared<object_type>(array_values_result[index]);
    }

    avltreeObj.printTree(avltreeObj.getRoot());
    avltreeObj.deleteValue(object_type{39});

    AVLTreeRecursion<test_data_type> avltree{array_values};

    avltree.printTree(avltree.getRoot());
    ASSERT_TRUE(avltree.verifyCorrectness(std::vector<test_data_type>{array_values_result}));

    std::cout << "Delete values to avl tree" << std::endl;

    // Value don't exist in the tree. Nothing will happen. This is for code coverage.
    avltree.deleteValue(1337);
    ASSERT_TRUE(avltree.verifyCorrectness(std::vector<test_data_type>(array_values_result)));

/*
R----30(h: 3)
   L----20(h: 2)
   |  L----10(h: 1)
   |  R----24(h: 1)
   R----39(h: 2)
      L----35(h: 1)
      R----40(h: 1)

{30, 20, 10, 24, 39, 35, 40}
 */

    avltree.insert(41);
    avltree.insert(42);
    avltree.insert(50);
    avltree.insert(55);
    avltree.insert(60);
    avltree.insert(65);
    std::cout << "=============" << std::endl;
    avltree.printTree(avltree.getRoot());
    avltree.deleteValue(10);
    std::cout << "=============" << std::endl;
    avltree.printTree(avltree.getRoot());
    avltree.deleteValue(24);
    std::cout << "=============" << std::endl;
    avltree.printTree(avltree.getRoot());
    avltree.deleteValue(39);
    std::cout << "=============" << std::endl;
    avltree.printTree(avltree.getRoot());
    // Right Left case.
    avltree.deleteValue(20);
    std::cout << "=============" << std::endl;
    avltree.printTree(avltree.getRoot());
    avltree.deleteValue(40);
    std::cout << "=============" << std::endl;
    avltree.printTree(avltree.getRoot());
    // Right Right case.
    avltree.deleteValue(35);
    std::cout << "=============" << std::endl;
    avltree.printTree(avltree.getRoot());
    avltree.insert(35);
    std::cout << "=============" << std::endl;
    avltree.printTree(avltree.getRoot());
    avltree.insert(40);
    std::cout << "=============" << std::endl;
    avltree.printTree(avltree.getRoot());
    avltree.insert(56);
    std::cout << "=============" << std::endl;
    avltree.printTree(avltree.getRoot());
    avltree.insert(57);
    std::cout << "=============" << std::endl;
    avltree.printTree(avltree.getRoot());
    // Left Left case.
    avltree.deleteValue(65);
    std::cout << "=============" << std::endl;
    avltree.printTree(avltree.getRoot());
    avltree.insert(58);
    std::cout << "=============" << std::endl;
    avltree.printTree(avltree.getRoot());
    avltree.deleteValue(55);
    std::cout << "=============" << std::endl;
    avltree.printTree(avltree.getRoot());
    // Left Right case.
    avltree.deleteValue(60);
    std::cout << "=============" << std::endl;
    avltree.printTree(avltree.getRoot());

//R----50(h: 4)
//    L----41(h: 3)
//    |  L----35(h: 2)
//    |  |  L----30(h: 1)
//    |  |  R----40(h: 1)
//    |  R----42(h: 1)
//    R----57(h: 2)
//        L----56(h: 1)
//        R----58(h: 1)

    ASSERT_TRUE(avltree.verifyCorrectness(std::vector<test_data_type>({50, 41, 35, 30, 40, 42, 57, 56, 58})));
    avltree.insert(59);
    avltree.insert(60);
    avltree.insert(61);
    avltree.insert(62);
    avltree.insert(63);
    avltree.insert(64);
    avltree.insert(65);
    avltree.insert(66);
    avltree.insert(70);
    std::cout << "=============" << std::endl;
    avltree.printTree(avltree.getRoot());
    avltree.deleteValue(61);
    std::cout << "=============" << std::endl;
    avltree.printTree(avltree.getRoot());
    avltree.deleteValue(65);
    std::cout << "=============" << std::endl;
    avltree.printTree(avltree.getRoot());
    avltree.deleteValue(59);
    std::cout << "=============" << std::endl;
    avltree.printTree(avltree.getRoot());
    avltree.deleteValue(70);
    std::cout << "=============" << std::endl;
    avltree.printTree(avltree.getRoot());
    avltree.deleteValue(66);
    std::cout << "=============" << std::endl;
    avltree.printTree(avltree.getRoot());
    avltree.deleteValue(56);
    std::cout << "=============" << std::endl;
    avltree.printTree(avltree.getRoot());
    avltree.deleteValue(57);
    std::cout << "=============" << std::endl;
    avltree.printTree(avltree.getRoot());

    /*
R----50(h: 4)
   L----41(h: 3)
   |  L----35(h: 2)
   |  |  L----30(h: 1)
   |  |  R----40(h: 1)
   |  R----42(h: 1)
   R----60(h: 3)
      L----58(h: 1)
      R----63(h: 2)
         L----62(h: 1)
         R----64(h: 1)
     */

    // Delete all.
    avltree.deleteValue(50);
    avltree.deleteValue(41);
    avltree.deleteValue(35);
    avltree.deleteValue(30);
    avltree.deleteValue(40);
    avltree.deleteValue(42);
    avltree.deleteValue(60);
    avltree.deleteValue(58);
    avltree.deleteValue(63);
    avltree.deleteValue(62);
    avltree.deleteValue(64);
    avltree.printTree(avltree.getRoot());
    ASSERT_TRUE(avltree.verifyCorrectness(std::vector<test_data_type>({})));
    std::cout << "TEST_F(AVLTreeRecursionTest, Delete) end" << std::endl;
}

TEST_F(AVLTreeRecursionTest, Search) {
    std::cout << "TEST_F(AVLTreeRecursionTest, Search) start" << std::endl;
    AVLTreeRecursion<test_data_type> avltree{};
    // Search in empty structure.
    ASSERT_FALSE(avltree.search(31));
    avltree = AVLTreeRecursion<test_data_type>{array_values};

    std::cout << "Search values in avl tree" << std::endl;
    for (test_data_count index = 0; index < array_values_elements_count; index++) {
        ASSERT_TRUE(avltree.search(array_values[index]));
    }

    // Search for value that is not in the structure.
    ASSERT_FALSE(avltree.search(1337));
    std::cout << "TEST_F(AVLTreeRecursionTest, Search) end" << std::endl;
}

TEST_F(AVLTreeRecursionTest, FailedVerify) {
    std::cout << "TEST_F(AVLTreeRecursionTest, Search) start" << std::endl;
    AVLTreeRecursion<test_data_type> avltree{};
    ASSERT_FALSE(avltree.verifyCorrectness(std::vector<test_data_type>{31}));
    avltree.insert(31);
    avltree.insert(13);
    ASSERT_FALSE(avltree.verifyCorrectness(std::vector<test_data_type>{31}));
    ASSERT_FALSE(avltree.verifyCorrectness(std::vector<test_data_type>{31, 0}));
    ASSERT_FALSE(avltree.verifyCorrectness(std::vector<test_data_type>{31, 13, 0}));
    std::cout << "TEST_F(AVLTreeRecursionTest, Search) end" << std::endl;
}

//void insertUntilStackOverflow(AVLTreeRecursion<test_data_type>& avltree) {
//    std::cout << "Insert values to avl tree until stack overflow" << std::endl;
//    for (test_data_count index = 0; index < 1000000000000; index++) {
//        avltree.insert(index);
//    }
//}
//
//TEST_F(AVLTreeRecursionTest, StackOverflow) {
//    !!! I didn't check it! !!!
//    std::cout << "TEST_F(AVLTreeRecursionTest, Insert) StackOverflow" << std::endl;
//    AVLTreeRecursion<test_data_type> avltree{};
//
//    ASSERT_EXIT(insertUntilStackOverflow(avltree), testing::ExitedWithCode(0xc00000fd), "Stack Overflow");
//
//    std::cout << "TEST_F(AVLTreeRecursionTest, Insert) StackOverflow" << std::endl;
//}