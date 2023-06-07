#include <gtest/gtest.h>
#include <vector>
#include "redblacktree.h"

using test_data_type = int;
using test_data_count = size_t;
using test_data_result_pair = std::pair<test_data_type, Node_Color>;

class RedBlackTreeLoopTest : public ::testing::Test {
protected:
    RedBlackTreeLoopTest() = default;

    static const test_data_count array_values_elements_count{ 7 };
    std::vector<test_data_type> array_values {30, 35, 40, 20, 10, 24, 39};
	std::vector<test_data_result_pair> array_values_result {{35, Node_Color::black}, {20, Node_Color::red}, {10, Node_Color::black}, {30, Node_Color::black}, {24, Node_Color::red}, {40, Node_Color::black}, {39, Node_Color::red}};

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

// GCC: "undefined reference" if variables defined inside RedBlackTreeLoopTest
const test_data_count RedBlackTreeLoopTest::array_values_elements_count;

std::ostream& operator<<(std::ostream& os, const RedBlackTreeLoopTest::ObjectA &obj) {
    return os << obj.get();
}

template <class T>
std::ostream& operator<<(std::ostream& os, const std::shared_ptr<T> &psobj) {
    return os << *psobj;
}

using object_type = RedBlackTreeLoopTest::ObjectA;
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

TEST_F(RedBlackTreeLoopTest, Insert) {
    std::cout << "TEST_F(RedBlackTreeLoopTest, Insert) start" << std::endl;
    RedBlackTreeLoop<test_data_type> rbtree{};

    std::cout << "Insert values to RB tree" << std::endl;
    for (const auto value : array_values) {
        // rotate: 40, 10, 24, 39
        rbtree.insert(value);
        rbtree.printTree(rbtree.getRoot());
        std::cout << "=============" << std::endl;
    }

    // Insert same value.
    rbtree.insert(array_values[0]);

    ASSERT_TRUE(rbtree.verifyCorrectness(array_values_result)) << "RB tree stores data incorrectly!";
    std::cout << "TEST_F(RedBlackTreeLoopTest, Insert) end" << std::endl;
}

TEST_F(RedBlackTreeLoopTest, InsertObject) {
    std::cout << "TEST_F(RedBlackTreeLoopTest, InsertObject) start" << std::endl;
    object_type objects_array[array_values_elements_count];
    std::vector<std::pair<object_type, Node_Color>> objects_array_result(array_values_elements_count);

    for (test_data_count index = 0; index < array_values_elements_count; index++) {
        objects_array[index] = object_type{array_values[index]};
        objects_array_result[index] = {object_type{array_values_result[index].first}, array_values_result[index].second};
    }

    RedBlackTreeLoop<object_type> rbtree{};

    std::cout << "Insert objects to RB tree" << std::endl;
    for (const auto &obj : objects_array) {
        rbtree.insert(obj);
        rbtree.printTree(rbtree.getRoot());
        std::cout << "=============" << std::endl;
    }

    rbtree.printTree(rbtree.getRoot());

    ASSERT_TRUE(rbtree.verifyCorrectness(objects_array_result)) << "RB tree stores objects data incorrectly!";
    std::cout << "TEST_F(RedBlackTreeLoopTest, InsertObject) end" << std::endl;
}

TEST_F(RedBlackTreeLoopTest, InsertPointers) {
    std::cout << "TEST_F(RedBlackTreeLoopTest, InsertPointers) start" << std::endl;
    store_smart_ptr_type objects_array[array_values_elements_count];
    std::vector<std::pair<store_smart_ptr_type, Node_Color>> objects_array_result(array_values_elements_count);

    for (test_data_count index = 0; index < array_values_elements_count; index++) {
        objects_array[index] = std::make_shared<object_type>(array_values[index]);
        objects_array_result[index] = {std::make_shared<object_type>(array_values_result[index].first), array_values_result[index].second};
    }

    RedBlackTreeLoop<store_smart_ptr_type> rbtree{};

    std::cout << "Insert smart pointers to RB tree" << std::endl;
    for (const auto &obj : objects_array) {
        rbtree.insert(obj);
    }

    rbtree.printTree(rbtree.getRoot());

    ASSERT_TRUE(rbtree.verifyCorrectness(objects_array_result)) << "RB tree stores objects data incorrectly!";
    std::cout << "TEST_F(RedBlackTreeLoopTest, InsertPointers) end" << std::endl;
}

TEST_F(RedBlackTreeLoopTest, Delete) {
    std::cout << "TEST_F(RedBlackTreeLoopTest, Delete) start" << std::endl;
    RedBlackTreeLoop<object_type> rbtreeObj{};
    std::vector<std::pair<store_smart_ptr_type, Node_Color>> objects_array_result(array_values_elements_count);

    for (test_data_count index = 0; index < array_values_elements_count; index++) {
        rbtreeObj.insert(object_type{array_values[index]});
        objects_array_result[index] = {std::make_shared<object_type>(array_values_result[index].first), array_values_result[index].second};
    }

    rbtreeObj.printTree(rbtreeObj.getRoot());
    rbtreeObj.deleteValue(object_type{39});

    RedBlackTreeLoop<test_data_type> rbtree{array_values};

    rbtree.printTree(rbtree.getRoot());
    ASSERT_TRUE(rbtree.verifyCorrectness(std::vector<test_data_result_pair>{array_values_result}));

    std::cout << "Delete values to RB tree" << std::endl;

    // Value don't exist in the tree. Nothing will happen. This is for code coverage.
    rbtree.deleteValue(1337);
    ASSERT_TRUE(rbtree.verifyCorrectness(std::vector<test_data_result_pair>(array_values_result)));

    rbtree.insert(41);
    std::cout << "=============" << std::endl;
    rbtree.printTree(rbtree.getRoot());
    rbtree.insert(42);
    std::cout << "=============" << std::endl;
    rbtree.printTree(rbtree.getRoot());
    rbtree.insert(50);
    std::cout << "=============" << std::endl;
    rbtree.printTree(rbtree.getRoot());
    rbtree.insert(55);
    std::cout << "=============" << std::endl;
    rbtree.printTree(rbtree.getRoot());
    rbtree.insert(60);
    std::cout << "=============" << std::endl;
    rbtree.printTree(rbtree.getRoot());
    rbtree.insert(65);
    std::cout << "=============" << std::endl;
    rbtree.printTree(rbtree.getRoot());

    rbtree.deleteValue(10);
    std::cout << "=============" << std::endl;
    rbtree.printTree(rbtree.getRoot());
    rbtree.deleteValue(24);
    std::cout << "=============" << std::endl;
    rbtree.printTree(rbtree.getRoot());
    rbtree.deleteValue(39);
    std::cout << "=============" << std::endl;
    rbtree.printTree(rbtree.getRoot());
    // Right Left case.
    rbtree.deleteValue(20);
    std::cout << "=============" << std::endl;
    rbtree.printTree(rbtree.getRoot());
    rbtree.deleteValue(40);
    std::cout << "=============" << std::endl;
    rbtree.printTree(rbtree.getRoot());
    // Right Right case.
    rbtree.deleteValue(35);
    std::cout << "=============" << std::endl;
    rbtree.printTree(rbtree.getRoot());
    rbtree.insert(35);
    std::cout << "=============" << std::endl;
    rbtree.printTree(rbtree.getRoot());
    rbtree.insert(40);
    std::cout << "=============" << std::endl;
    rbtree.printTree(rbtree.getRoot());
    rbtree.insert(56);
    std::cout << "=============" << std::endl;
    rbtree.printTree(rbtree.getRoot());
    rbtree.insert(57);
    std::cout << "=============" << std::endl;
    rbtree.printTree(rbtree.getRoot());
    // Left Left case.
    rbtree.deleteValue(65);
    std::cout << "=============" << std::endl;
    rbtree.printTree(rbtree.getRoot());
    rbtree.insert(58);
    std::cout << "=============" << std::endl;
    rbtree.printTree(rbtree.getRoot());
    rbtree.deleteValue(55);
    std::cout << "=============" << std::endl;
    rbtree.printTree(rbtree.getRoot());
    // Left Right case.
    rbtree.deleteValue(60);
    std::cout << "=============" << std::endl;
    rbtree.printTree(rbtree.getRoot());

/*
R----42(black)
   L----35(black)
   |  L----30(black)
   |  R----41(black)
   |     L----40(red)
   R----56(black)
      L----50(black)
      R----58(black)
         L----57(red)
 */

    ASSERT_TRUE(rbtree.verifyCorrectness(std::vector<test_data_result_pair>({
        {42, Node_Color::black}, {35, Node_Color::black}, {30, Node_Color::black},
        {41, Node_Color::black}, {40, Node_Color::red}, {56, Node_Color::black},
        {50, Node_Color::black}, {58, Node_Color::black}, {57, Node_Color::red}
    })));
    rbtree.insert(59);
    rbtree.insert(60);
    rbtree.insert(61);
    rbtree.insert(62);
    rbtree.insert(63);
    rbtree.insert(65);
    rbtree.insert(64);
    rbtree.insert(66);
    rbtree.insert(70);
    std::cout << "=============" << std::endl;
    rbtree.printTree(rbtree.getRoot());
    rbtree.deleteValue(61);
    std::cout << "=============" << std::endl;
    rbtree.printTree(rbtree.getRoot());
    rbtree.deleteValue(65);
    std::cout << "=============" << std::endl;
    rbtree.printTree(rbtree.getRoot());
    rbtree.deleteValue(59);
    std::cout << "=============" << std::endl;
    rbtree.printTree(rbtree.getRoot());
    rbtree.deleteValue(70);
    std::cout << "=============" << std::endl;
    rbtree.printTree(rbtree.getRoot());
    rbtree.deleteValue(66);
    std::cout << "=============" << std::endl;
    rbtree.printTree(rbtree.getRoot());
    rbtree.deleteValue(56);
    std::cout << "=============" << std::endl;
    rbtree.printTree(rbtree.getRoot());
    rbtree.deleteValue(57);
    std::cout << "=============" << std::endl;
    rbtree.printTree(rbtree.getRoot());

    rbtree.insert(29);
    rbtree.insert(28);
    rbtree.insert(27);
    rbtree.insert(26);
    std::cout << "=============" << std::endl;
    rbtree.printTree(rbtree.getRoot());

    // Delete all.
    rbtree.deleteValue(40);
    rbtree.deleteValue(50);
    rbtree.deleteValue(30);
    rbtree.deleteValue(35);
    rbtree.deleteValue(41);
    rbtree.deleteValue(42);
    rbtree.deleteValue(60);
    rbtree.deleteValue(58);
    rbtree.deleteValue(63);
    rbtree.deleteValue(62);
    rbtree.deleteValue(64);
    rbtree.deleteValue(29);
    rbtree.deleteValue(28);
    rbtree.deleteValue(27);
    ASSERT_TRUE(rbtree.verifyCorrectness(std::vector<test_data_result_pair>({{26, Node_Color::black}})));
    rbtree.insert(27);
    rbtree.deleteValue(26);
    ASSERT_TRUE(rbtree.verifyCorrectness(std::vector<test_data_result_pair>({{27, Node_Color::black}})));
    rbtree.deleteValue(27);
    std::cout << "=============" << std::endl;
    rbtree.printTree(rbtree.getRoot());
    ASSERT_TRUE(rbtree.verifyCorrectness(std::vector<test_data_result_pair>({})));
    std::cout << "TEST_F(RedBlackTreeLoopTest, Delete) end" << std::endl;
}

TEST_F(RedBlackTreeLoopTest, Search) {
    std::cout << "TEST_F(RedBlackTreeLoopTest, Search) start" << std::endl;
    RedBlackTreeLoop<test_data_type> rbtree{};
    // Search in empty structure.
    ASSERT_FALSE(rbtree.search(31));
    rbtree = RedBlackTreeLoop<test_data_type>{array_values};

    std::cout << "Search values in RB tree" << std::endl;
    for (test_data_count index = 0; index < array_values_elements_count; index++) {
        ASSERT_TRUE(rbtree.search(array_values[index]));
    }

    // Search for value that is not in the structure.
    ASSERT_FALSE(rbtree.search(1337));
    std::cout << "TEST_F(RedBlackTreeLoopTest, Search) end" << std::endl;
}

TEST_F(RedBlackTreeLoopTest, FailedVerify) {
    std::cout << "TEST_F(RedBlackTreeLoopTest, Search) start" << std::endl;
    RedBlackTreeLoop<test_data_type> rbtree{};
    ASSERT_FALSE(rbtree.verifyCorrectness(std::vector<test_data_result_pair>{{31, Node_Color::black}}));
    rbtree.insert(31);
    rbtree.insert(13);
    ASSERT_FALSE(rbtree.verifyCorrectness(std::vector<test_data_result_pair>{{31, Node_Color::black}}));
    ASSERT_FALSE(rbtree.verifyCorrectness(std::vector<test_data_result_pair>{{31, Node_Color::black}, {0, Node_Color::black}}));
    ASSERT_FALSE(rbtree.verifyCorrectness(std::vector<test_data_result_pair>{{31, Node_Color::black}, {13, Node_Color::black}}));
    ASSERT_FALSE(rbtree.verifyCorrectness(std::vector<test_data_result_pair>{{31, Node_Color::black}, {13, Node_Color::red}, {0, Node_Color::black}}));
    ASSERT_TRUE(rbtree.verifyCorrectness(std::vector<test_data_result_pair>{{31, Node_Color::black}, {13, Node_Color::red}}));
    rbtree.printTree(rbtree.getRoot());
    std::cout << "TEST_F(RedBlackTreeLoopTest, Search) end" << std::endl;
}