#include <gtest/gtest.h>
#include <vector>
#include "binary_search_tree.h"

using test_data_type = int;
using test_data_count = size_t;

class BinarySearchTreeLoopTest : public ::testing::Test {
protected:
    BinarySearchTreeLoopTest() = default;

    static const test_data_count array_values_elements_count{ 7 };
    std::vector<test_data_type> array_values {30, 35, 40, 20, 10, 24, 39};
	std::vector<test_data_type> array_values_result {30, 20, 10, 24, 35, 40, 39};

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

// GCC: "undefined reference" if variables defined inside BinarySearchTreeLoopTest
const test_data_count BinarySearchTreeLoopTest::array_values_elements_count;

std::ostream& operator<<(std::ostream& os, const BinarySearchTreeLoopTest::ObjectA &obj) {
    return os << obj.get();
}

template <class T>
std::ostream& operator<<(std::ostream& os, const std::shared_ptr<T> &psobj) {
    return os << *psobj;
}

using object_type = BinarySearchTreeLoopTest::ObjectA;
using store_smart_ptr_type = std::shared_ptr<object_type>;

TEST_F(BinarySearchTreeLoopTest, Insert) {
    std::cout << "TEST_F(BinarySearchTreeLoopTest, Insert) start" << std::endl;
    BinarySearchTreeLoop<test_data_type> bstree{};

    std::cout << "Insert values to RB tree" << std::endl;
    for (const auto value : array_values) {
        bstree.insert(value);
    }

    bstree.printTree(bstree.getRoot());

    // Insert same value.
    bstree.insert(array_values[0]);

    ASSERT_TRUE(bstree.verifyCorrectness(array_values_result)) << "RB tree stores data incorrectly!";
    std::cout << "TEST_F(BinarySearchTreeLoopTest, Insert) end" << std::endl;
}

TEST_F(BinarySearchTreeLoopTest, InsertObject) {
    std::cout << "TEST_F(BinarySearchTreeLoopTest, InsertObject) start" << std::endl;
    object_type objects_array[array_values_elements_count];
    std::vector<object_type> objects_array_result(array_values_elements_count);

    for (test_data_count index = 0; index < array_values_elements_count; index++) {
        objects_array[index] = object_type{array_values[index]};
        objects_array_result[index] = object_type{array_values_result[index]};
    }

    BinarySearchTreeLoop<object_type> bstree{};

    std::cout << "Insert objects to RB tree" << std::endl;
    for (const auto &obj : objects_array) {
        bstree.insert(obj);
        bstree.printTree(bstree.getRoot());
        std::cout << "=============" << std::endl;
    }

    bstree.printTree(bstree.getRoot());

    ASSERT_TRUE(bstree.verifyCorrectness(objects_array_result)) << "RB tree stores objects data incorrectly!";
    std::cout << "TEST_F(BinarySearchTreeLoopTest, InsertObject) end" << std::endl;
}

TEST_F(BinarySearchTreeLoopTest, InsertPointers) {
    std::cout << "TEST_F(BinarySearchTreeLoopTest, InsertPointers) start" << std::endl;
    store_smart_ptr_type objects_array[array_values_elements_count];
    std::vector<store_smart_ptr_type> objects_array_result(array_values_elements_count);

    for (test_data_count index = 0; index < array_values_elements_count; index++) {
        objects_array[index] = std::make_shared<object_type>(array_values[index]);
        objects_array_result[index] = std::make_shared<object_type>(array_values_result[index]);
    }

    BinarySearchTreeLoop<store_smart_ptr_type> bstree{};

    std::cout << "Insert smart pointers to RB tree" << std::endl;
    for (const auto &obj : objects_array) {
        bstree.insert(obj);
    }

    bstree.printTree(bstree.getRoot());

    ASSERT_TRUE(bstree.verifyCorrectness(objects_array_result)) << "RB tree stores objects data incorrectly!";
    std::cout << "TEST_F(BinarySearchTreeLoopTest, InsertPointers) end" << std::endl;
}

TEST_F(BinarySearchTreeLoopTest, Delete) {
    std::cout << "TEST_F(BinarySearchTreeLoopTest, Delete) start" << std::endl;
    BinarySearchTreeLoop<object_type> bstreeObj{};
    std::vector<store_smart_ptr_type> objects_array_result(array_values_elements_count);

    for (test_data_count index = 0; index < array_values_elements_count; index++) {
        bstreeObj.insert(object_type{array_values[index]});
        objects_array_result[index] = std::make_shared<object_type>(array_values_result[index]);
    }

    bstreeObj.printTree(bstreeObj.getRoot());
    bstreeObj.deleteValue(object_type{39});

    BinarySearchTreeLoop<test_data_type> bstree{array_values};

    bstree.printTree(bstree.getRoot());
    ASSERT_TRUE(bstree.verifyCorrectness(std::vector<test_data_type>{array_values_result}));

    std::cout << "Delete values to RB tree" << std::endl;

    // Value don't exist in the tree. Nothing will happen. This is for code coverage.
    bstree.deleteValue(1337);
    ASSERT_TRUE(bstree.verifyCorrectness(std::vector<test_data_type>(array_values_result)));

    bstree.insert(41);
    bstree.insert(42);
    bstree.insert(50);
    bstree.insert(55);
    bstree.insert(60);
    bstree.insert(65);
    std::cout << "=============" << std::endl;
    bstree.printTree(bstree.getRoot());
    bstree.deleteValue(10);
    std::cout << "=============" << std::endl;
    bstree.printTree(bstree.getRoot());
    bstree.deleteValue(24);
    std::cout << "=============" << std::endl;
    bstree.printTree(bstree.getRoot());
    bstree.deleteValue(39);
    std::cout << "=============" << std::endl;
    bstree.printTree(bstree.getRoot());
    // Right Left case.
    bstree.deleteValue(20);
    std::cout << "=============" << std::endl;
    bstree.printTree(bstree.getRoot());
    bstree.deleteValue(40);
    std::cout << "=============" << std::endl;
    bstree.printTree(bstree.getRoot());
    // Right Right case.
    bstree.deleteValue(35);
    std::cout << "=============" << std::endl;
    bstree.printTree(bstree.getRoot());
    bstree.insert(35);
    std::cout << "=============" << std::endl;
    bstree.printTree(bstree.getRoot());
    bstree.insert(40);
    std::cout << "=============" << std::endl;
    bstree.printTree(bstree.getRoot());
    bstree.insert(56);
    std::cout << "=============" << std::endl;
    bstree.printTree(bstree.getRoot());
    bstree.insert(57);
    std::cout << "=============" << std::endl;
    bstree.printTree(bstree.getRoot());
    // Left Left case.
    bstree.deleteValue(65);
    std::cout << "=============" << std::endl;
    bstree.printTree(bstree.getRoot());
    bstree.insert(58);
    std::cout << "=============" << std::endl;
    bstree.printTree(bstree.getRoot());
    bstree.deleteValue(55);
    std::cout << "=============" << std::endl;
    bstree.printTree(bstree.getRoot());
    // Left Right case.
    bstree.deleteValue(60);
    std::cout << "=============" << std::endl;
    bstree.printTree(bstree.getRoot());

    /*
 R----30
   R----41
      L----35
      |  R----40
      R----42
         R----50
            R----56
               R----57
                  R----58

30, 41, 35, 40, 42, 50, 56, 57, 58
     */

    ASSERT_TRUE(bstree.verifyCorrectness(std::vector<test_data_type>({30, 41, 35, 40, 42, 50, 56, 57, 58})));
    bstree.insert(59);
    bstree.insert(60);
    bstree.insert(61);
    bstree.insert(62);
    bstree.insert(63);
    bstree.insert(64);
    bstree.insert(65);
    bstree.insert(66);
    bstree.insert(70);
    std::cout << "=============" << std::endl;
    bstree.printTree(bstree.getRoot());
    bstree.deleteValue(61);
    std::cout << "=============" << std::endl;
    bstree.printTree(bstree.getRoot());
    bstree.deleteValue(65);
    std::cout << "=============" << std::endl;
    bstree.printTree(bstree.getRoot());
    bstree.deleteValue(59);
    std::cout << "=============" << std::endl;
    bstree.printTree(bstree.getRoot());
    bstree.deleteValue(70);
    std::cout << "=============" << std::endl;
    bstree.printTree(bstree.getRoot());
    bstree.deleteValue(66);
    std::cout << "=============" << std::endl;
    bstree.printTree(bstree.getRoot());
    bstree.deleteValue(56);
    std::cout << "=============" << std::endl;
    bstree.printTree(bstree.getRoot());
    bstree.deleteValue(57);
    std::cout << "=============" << std::endl;
    bstree.printTree(bstree.getRoot());

    bstree.insert(23);
    bstree.insert(28);
    bstree.insert(26);
    bstree.insert(24);
    bstree.insert(27);
    bstree.insert(21);
    bstree.insert(22);
    bstree.printTree(bstree.getRoot());
    // Delete all.
    bstree.deleteValue(23);
    bstree.deleteValue(28);
    bstree.deleteValue(26);
    bstree.deleteValue(24);
    bstree.deleteValue(27);
    bstree.deleteValue(21);
    bstree.deleteValue(22);
    bstree.deleteValue(50);
    bstree.deleteValue(41);
    bstree.deleteValue(35);
    bstree.deleteValue(30);
    bstree.deleteValue(40);
    bstree.deleteValue(42);
    bstree.deleteValue(60);
    bstree.deleteValue(58);
    bstree.deleteValue(63);
    bstree.deleteValue(62);
    bstree.deleteValue(64);
    bstree.printTree(bstree.getRoot());
    ASSERT_TRUE(bstree.verifyCorrectness(std::vector<test_data_type>({})));
    std::cout << "TEST_F(BinarySearchTreeLoopTest, Delete) end" << std::endl;
}

TEST_F(BinarySearchTreeLoopTest, Search) {
    std::cout << "TEST_F(BinarySearchTreeLoopTest, Search) start" << std::endl;
    BinarySearchTreeLoop<test_data_type> bstree{};
    // Search in empty structure.
    ASSERT_FALSE(bstree.search(31));
    bstree = BinarySearchTreeLoop<test_data_type>{array_values};

    std::cout << "Search values in RB tree" << std::endl;
    for (test_data_count index = 0; index < array_values_elements_count; index++) {
        ASSERT_TRUE(bstree.search(array_values[index]));
    }

    // Search for value that is not in the structure.
    ASSERT_FALSE(bstree.search(1337));
    std::cout << "TEST_F(BinarySearchTreeLoopTest, Search) end" << std::endl;
}

TEST_F(BinarySearchTreeLoopTest, FailedVerify) {
    std::cout << "TEST_F(BinarySearchTreeLoopTest, Search) start" << std::endl;
    BinarySearchTreeLoop<test_data_type> bstree{};
    ASSERT_FALSE(bstree.verifyCorrectness(std::vector<test_data_type>{31}));
    bstree.insert(31);
    bstree.insert(13);
    ASSERT_FALSE(bstree.verifyCorrectness(std::vector<test_data_type>{31}));
    ASSERT_FALSE(bstree.verifyCorrectness(std::vector<test_data_type>{31, 0}));
    ASSERT_FALSE(bstree.verifyCorrectness(std::vector<test_data_type>{31, 13, 0}));
    std::cout << "TEST_F(BinarySearchTreeLoopTest, Search) end" << std::endl;
}