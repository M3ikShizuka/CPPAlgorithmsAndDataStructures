#pragma once
#include <iostream>
#include <string>
#include "head.h"

template <class DataType, class SizeType = size_t>
struct BinarySearchTreeNodeLoop {
    using value_type = DataType;
    using size_type = SizeType;
    using node_type = BinarySearchTreeNodeLoop<value_type, size_type>;
    using node_type_ptr = std::shared_ptr<node_type>;

    value_type key {0};
    node_type_ptr left{};
    node_type_ptr right{};

    CONSTEXPR20 BinarySearchTreeNodeLoop() = default;
    CONSTEXPR20 explicit BinarySearchTreeNodeLoop(const value_type& key) : key(key) {}

    CONSTEXPR20 ~BinarySearchTreeNodeLoop() = default;
};

template <class DataType, class SizeType = size_t>
class BinarySearchTreeLoop {
private:
    using value_type = DataType;
    using const_reference = const DataType &;
    using size_type = SizeType;
    using node_type = BinarySearchTreeNodeLoop<value_type, size_type>;
    using node_type_ptr = std::shared_ptr<node_type>;

    node_type_ptr root{};

public:
    CONSTEXPR20 BinarySearchTreeLoop() = default;

    CONSTEXPR20 explicit BinarySearchTreeLoop(const std::vector<value_type> &vec) {
        const size_type vec_size = vec.size();
        for (size_type index = 0; index < vec_size; index++) {
            insert(vec[index]);
        }
    }

    CONSTEXPR20 explicit BinarySearchTreeLoop(const value_type *start, const value_type *end) {
        size_type it_index = 0;
        for (auto it = start; it != end; it++, it_index++) {
            insert(*it);
        }
    }

    CONSTEXPR20 ~BinarySearchTreeLoop() = default;

    NODISCARD CONSTEXPR20 bool search(const value_type &&value) const {
        return searchElement(value);
    }

    NODISCARD CONSTEXPR20 bool search(const value_type &value) const {
        return searchElement(value);
    }

    CONSTEXPR20 void insert(const value_type &&value) {
        insertElement(value);
    }

    CONSTEXPR20 void insert(const value_type &value) {
        insertElement(value);
    }

    CONSTEXPR20 void deleteValue(const value_type &&value) {
        deleteElement(value);
    }

    CONSTEXPR20 void deleteValue(const value_type &value) {
        deleteElement(value);
    }

    CONSTEXPR20 auto getRoot() const {
        return root;
    }

    // Print the tree
    void printTree(node_type_ptr node) {
        if (node) {
            std::string indent{};
            printNodeInfo(node, indent, false);
        }
    }

    void printNodeInfo(node_type_ptr node, std::string indent, bool leftNode) {
        std::string output = indent;

        if (leftNode) {
            output += "L----";
            indent += "|  ";
        } else {
            output += "R----";
            indent += "   ";
        }

        std::cout << output << node->key << std::endl;

        if (node->left) {
            printNodeInfo(node->left, indent, true);
        }

        if (node->right) {
            printNodeInfo(node->right, indent, false);
        }
    }

    NODISCARD CONSTEXPR20 bool verifyCorrectness(const std::vector<value_type>& key_array) const {
        // Verify the correctness of the tree.
        std::cout << "Verify correctness:" << std::endl;
        const bool result = verifyNodeWrap(root, key_array);
        if (result) {
            std::cout << std::endl << "Verification successful" << std::endl;
        }
        else {
            std::cout << std::endl << "Verification failed" << std::endl;
        }

        return result;
    }

private:
    CONSTEXPR20 bool searchElement(const value_type &key) const {
        if (root) {
            return searchNode(root, key);
        }

        return false;
    }

    CONSTEXPR20 bool searchNode(const node_type_ptr node, const value_type &key) const {
        auto node_current = node;

        do {
            if (isLess(key, node_current->key)) {
                // left subtree
                node_current = node_current->left;
            } else if (isGreater(key, node_current->key)) {
                // right subtree
                node_current = node_current->right;
            } else {
                // Node found.
                break;
            }
        } while (node_current);

        // The Node was not found.
        if (!node_current) {
            return false;
        }

        // Element found.
        return true;
    }

    CONSTEXPR20 void insertElement(const value_type &key) {
        if (root) {
            insertNodeInTree(root, key);
        } else {
            root = createNewNode(key);
        }
    }

    CONSTEXPR20 void insertNodeInTree(node_type_ptr node, const value_type &key) {
        // Search the node to insert.
        auto node_current = searchNodeForInsert(node, key);
        if (!node_current) {
            // The same key value already exist in tree.
            return;
        }

        // Create and insert node.
        auto node_new = createNewNode(key);
        // Make a link with the previous node.
        makeLinkWithPreviousNode(node_new, node_current);
    }

    CONSTEXPR20 node_type_ptr searchNodeForInsert(node_type_ptr node, const value_type &key) {
        auto node_next = node;
        node_type_ptr node_current;

        do {
            node_current = node_next;

            if (isLess(key, node_current->key)) {
                // left subtree
                node_next = node_current->left;
            } else if (isGreater(key, node_current->key)) {
                // right subtree
                node_next = node_current->right;
            } else {
                // The same key value already exist in tree.
                return nullptr;
            }
        } while (node_next);

        return node_current;
    }

    CONSTEXPR20 void deleteElement(const value_type &key) {
        if (root) {
            deleteNodeInTree(root, key);
        }
    }

    CONSTEXPR20 void deleteNodeInTree(node_type_ptr node, const value_type &key) {
        // Search the node to delete.
        auto [node_current, node_prev] = searchNodeForDelete(node, key);
        if (!node_current) {
            // The Node was not found.
            return;
        }

        // Delete the node.
        deleteNodePreProcess(node_current, node_prev);
    }

    CONSTEXPR20 std::pair<node_type_ptr, node_type_ptr> searchNodeForDelete(node_type_ptr node, const value_type &key) {
        auto node_current = node;
        node_type_ptr node_prev;

        do {
            if (isLess(key, node_current->key)) {
                // left subtree
                node_prev = node_current;
                node_current = node_current->left;
            } else if (isGreater(key, node_current->key)) {
                // right subtree
                node_prev = node_current;
                node_current = node_current->right;
            } else {
                // Node found, delete it.
                break;
            }
        } while (node_current);

        return {node_current, node_prev};
    }

    CONSTEXPR20 void deleteNodePreProcess(node_type_ptr node, node_type_ptr node_parent) {
        if (node->left && node->right) {
            // Node has two children.
            // Get inorder successor. Smallest in the right tree.
            auto [node_min, node_min_parent] = getMinimumValueNode(node->right, node);
            node->key = node_min->key;
            // Delete the old value's node.
            // This one time recursion. The min node will never have a left child, only a right child maybe that grater than current node.
            deleteNodePreProcess(node_min, node_min_parent);
        } else {
            if (node->left) {
                // Node has only left child.
                auto left_node = node->left;
                // Fix relations.
                if (node_parent) {
                    swapChildNodeForParent(node, node_parent, left_node);
                }
                else if (node == root) {
                    setRoot(left_node);
                }
                // Delete the node.
                deleteNode(node, node_parent);
            } else if (node->right) {
                // Node has only right child.
                auto right_node = node->right;
                // Fix relations.
                if (node_parent) {
                    swapChildNodeForParent(node, node_parent, right_node);
                }
                else if (node == root) {
                    setRoot(right_node);
                }
                // Delete the node.
                deleteNode(node, node_parent);
            } else {
                // Node has no children at all.
                deleteNode(node, node_parent);

                if (node == root) {
                    setRoot(nullptr);
                }
            }
        }
    }

    CONSTEXPR20 void setRoot(const node_type_ptr node) {
        root = node;
    }

    CONSTEXPR20 node_type_ptr createNewNode(const value_type& key) const {
        return std::make_shared<node_type>(key);
    }

    CONSTEXPR20 void makeLinkWithPreviousNode(node_type_ptr node, node_type_ptr node_prev) const {
        if (isLess(node->key, node_prev->key)) {
            // left subtree
            node_prev->left = node;
        } else if (isGreater(node->key, node_prev->key)) {
            // right subtree
            node_prev->right = node;
        }
    }

    CONSTEXPR20 void swapChildNodeForParent(node_type_ptr node, node_type_ptr node_parent, node_type_ptr node_new_child) const {
        if (node_parent->left == node) {
            node_parent->left = node_new_child;
        } else if (node_parent->right == node) {
            node_parent->right = node_new_child;
        }
    }

    CONSTEXPR20 void deleteNode(node_type_ptr node, node_type_ptr node_parent) const {
        if (node->left) {
            node->left = nullptr;
        }

        if (node->right) {
            node->right = nullptr;
        }

        if (node_parent) {
            if (node_parent->left == node) {
                node_parent->left = nullptr;
            }
            else if (node_parent->right == node) {
                node_parent->right = nullptr;
            }
        }
    }

    NODISCARD CONSTEXPR20 std::pair<node_type_ptr, node_type_ptr> getMinimumValueNode(node_type_ptr node, node_type_ptr node_start_parent) const {
        auto node_current = node;
        node_type_ptr node_prev;

        if (!node_current->left) {
            return {node_current, node_start_parent};
        }

        do {
            node_prev = node_current;
            node_current = node_current->left;
        } while (node_current->left);

        return {node_current, node_prev};
    }

    NODISCARD CONSTEXPR20 bool verifyNodeWrap(const node_type_ptr node, const std::vector<value_type>& key_array) const {
        size_type index = 0;
        const bool result = verifyNode(node, key_array, index);
        if (result && key_array.size() > 0) {
            if (key_array.size() != index+1) {
                return false;
            }
        }

        return result;
    }

    NODISCARD CONSTEXPR20 bool verifyNode(const node_type_ptr node, const std::vector<value_type>& key_array, size_type& index) const {
        if (!node) {
            if (key_array.empty()) {
                return true;
            }

            return false;
        }

        std::cout << " " << node->key;

        // Process current node.
        if (key_array.size() <= index) {
            return false;
        }

        if (!areEqual(node->key, key_array[index])) {
            return false;
        }

        if (node->left) {
            index++;
            if (!verifyNode(node->left, key_array, index)) {
                return false;
            }
        }

        if (node->right) {
            index++;
            if (!verifyNode(node->right, key_array, index)) {
                return false;
            }
        }

        return true;
    }

    template<class T>
    NODISCARD CONSTEXPR20 bool areEqual(std::shared_ptr<T> smart_obj_a, std::shared_ptr<T> smart_obj_b) const {
        return *smart_obj_a == *smart_obj_b;
    }

    template<class T>
    NODISCARD CONSTEXPR20 bool areEqual(const T& value_a, const T& value_b) const {
        return value_a == value_b;
    }

    template<class T>
    NODISCARD CONSTEXPR20 bool isGreater(std::shared_ptr<T> smart_obj_a, std::shared_ptr<T> smart_obj_b) const {
        return *smart_obj_a > *smart_obj_b;
    }

    template<class T>
    NODISCARD CONSTEXPR20 bool isGreater(const T& value_a, const T& value_b) const {
        return value_a > value_b;
    }

    template<class T>
    NODISCARD CONSTEXPR20 bool isLess(std::shared_ptr<T> smart_obj_a, std::shared_ptr<T> smart_obj_b) const {
        return *smart_obj_a < *smart_obj_b;
    }

    template<class T>
    NODISCARD CONSTEXPR20 bool isLess(const T& value_a, const T& value_b) const {
        return value_a < value_b;
    }
};