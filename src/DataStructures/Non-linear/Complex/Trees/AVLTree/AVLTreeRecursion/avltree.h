// The recursion method is bad:
// - It is the cause of stack overflow with big number of items.
//   ERROR_CODE: (NTSTATUS) 0xc00000fd - A new guard page for the stack cannot be created.
// - It requires additional performance costs for calling the functions of forming a new function frame and placing the return address in the stack.
#pragma once
#include <iostream>
#include <type_traits>
#include <string>
#include <memory>
#include "head.h"
#include "comparators.h"

template <class DataType, class SizeType = size_t>
struct AVLTreeNodeRecursion {
    using value_type = DataType;
    using size_type = SizeType;
    using signed_size_type = std::make_signed_t<size_type>;
    using node_type = AVLTreeNodeRecursion<value_type, size_type>;
    using node_type_ptr = std::shared_ptr<node_type>;
    
    value_type key {0};
    node_type_ptr left;
    node_type_ptr right;
    size_type height {1};

    CONSTEXPR20 AVLTreeNodeRecursion() = default;
    CONSTEXPR20 explicit AVLTreeNodeRecursion(const value_type& key) : key(key) {}

    CONSTEXPR20 ~AVLTreeNodeRecursion() = default;

    NODISCARD CONSTEXPR20 signed_size_type getBalanceFactor() const {
        size_type left_height = 0;
        size_type right_height = 0;

        if (left) {
            left_height = left->height;
        }

        if (right) {
            right_height = right->height;
        }

        return static_cast<signed_size_type>(left_height - right_height);
    }

    CONSTEXPR20 void updateHeight() {
        size_type max_height = 0;

        if (left) {
            max_height = left->height;
        }

        if (right && max_height < right->height) {
            max_height = right->height;
        }

        height = max_height + 1;
    }
};

template <class DataType, class SizeType = size_t>
class AVLTreeRecursion {
private:
    using value_type = DataType;
    using const_reference = const DataType&;
    using size_type = SizeType;
    using signed_size_type = std::make_signed_t<size_type>;
    using node_type = AVLTreeNodeRecursion<value_type, size_type>;
    using node_type_ptr = std::shared_ptr<node_type>;

    node_type_ptr root;

public:
    CONSTEXPR20 AVLTreeRecursion() = default;

    CONSTEXPR20 explicit AVLTreeRecursion(const std::vector<value_type>& vec) {
        const size_type vec_size = vec.size();
        for (size_type index = 0; index < vec_size; index++) {
            insert(vec[index]);
        }
    }

    CONSTEXPR20 explicit AVLTreeRecursion(const value_type* start, const value_type* end) {
        size_type it_index = 0;
        for (auto it = start; it != end; it++, it_index++) {
            insert(*it);
        }
    }

    CONSTEXPR20 ~AVLTreeRecursion() = default;

    NODISCARD CONSTEXPR20 bool search(const value_type&& value) const {
        return searchElement(value);
    }

    NODISCARD CONSTEXPR20 bool search(const value_type& value) const {
        return searchElement(value);
    }

    CONSTEXPR20 void insert(const value_type&& value) {
        insertElement(value);
    }

    CONSTEXPR20 void insert(const value_type& value) {
        insertElement(value);
    }

    CONSTEXPR20 void deleteValue(const value_type&& value) {
        deleteElement(value);
    }

    CONSTEXPR20 void deleteValue(const value_type& value) {
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

        std::cout << output << node->key << "(h: " << node->height << ")" << std::endl;

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
    CONSTEXPR20 bool searchElement(const value_type& key) const {
        if (root) {
            return searchNode(root, key);
        }

        return false;
    }

    CONSTEXPR20 bool searchNode(const node_type_ptr node, const value_type& key) const {
        if (isLess(key, node->key)) {
            if (node->left) {
                // left subtree
                return searchNode(node->left, key);
            }

            // If value don't exist in tree.
            return false;
        }
        else if (isGreater(key, node->key)) {
            if (node->right) {
                // right subtree
                return searchNode(node->right, key);
            }

            // If value don't exist in tree.
            return false;
        }

        // Element found.
        return true;
    }

    CONSTEXPR20 void insertElement(const value_type& key) {
        if (root) {
            root = insertNodeInTree(root, key);
        }
        else {
            root = createNewNode(key);
        }
    }

    CONSTEXPR20 node_type_ptr insertNodeInTree(node_type_ptr node, const value_type& key) {
        // Create and insert node.
        if (!node) {
            return createNewNode(key);
        }
        else if (!insertNode(node, key)) {
            // Discard a duplicate key. Do not insert the value into the tree.
            return node;
        }

        // Node inserted in tree.
        return processModifiedNodesAfterInsert(node, key);
    }

    CONSTEXPR20 bool insertNode(node_type_ptr node, const value_type& key) {
        // Search the node to insert.
        if (isLess(key, node->key)) {
            // left subtree
            node->left = insertNodeInTree(node->left, key);
            return true;
        }
        else if (isGreater(key, node->key)) {
            // right subtree
            node->right = insertNodeInTree(node->right, key);
            return true;
        }

        // The same key value already exist in tree.
        return false;
    }

    NODISCARD CONSTEXPR20 node_type_ptr processModifiedNodesAfterInsert(node_type_ptr node, const value_type& key) {
        // Update the height.
        node->updateHeight();
        // Rebalance the node.
        return rebalanceInsert(node, key);
    }

    CONSTEXPR20 node_type_ptr rebalanceInsert(node_type_ptr node, const value_type& key) {
        const signed_size_type balance_factor = node->getBalanceFactor();
        if (balance_factor > 1) {
            if (isLess(key, node->left->key)) {
                // Left Left case.
                return rotateRight(node);
            }
            else {
                // NOTE: Use "else if" if you are going to modify AVLTree to store duplicate keys.
                // else if (isGreater(key, node->left->key)) { // Use "else if" to prevent problem if new key == node->key
                // Left Right case.
                node->left = rotateLeft(node->left);
                return rotateRight(node);
            }
        }
        else if (balance_factor < -1) {
            if (isGreater(key, node->right->key)) {
                // Right Right case.
                return rotateLeft(node);
            }
            else {
                // NOTE: Use "else if" if you are going to modify AVLTree to store duplicate keys.
                // else if (isLess(key, node->right->key)) { // Use "else if" to prevent problem if new key == node->key
                // Right Left case.
                node->right = rotateRight(node->right);
                return rotateLeft(node);
            }
        }

        return node;
    }

    CONSTEXPR20 void deleteElement(const value_type& key) {
        if (root) {
            root = deleteNodeInTree(root, key);
        }
    }

    CONSTEXPR20 node_type_ptr deleteNodeInTree(node_type_ptr node, const value_type& key) {
        // Search the node to delete.
        if (isLess(key, node->key)) {
            if (node->left) {
                // left subtree
                node->left = deleteNodeInTree(node->left, key);
            }
            else {
                // If value don't exist in tree.
                return node;
            }
        }
        else if (isGreater(key, node->key)) {
            if (node->right) {
                // right subtree
                node->right = deleteNodeInTree(node->right, key);
            }
            else {
                // If value don't exist in tree.
                return node;
            }
        }
        else {
            // Node found, delete it.
            if (!deleteNodePreProcess(node)) {
                // Discard a duplicate key. Do not delete the value into the tree.
                return node;
            }
        }

        // Node deleted in tree.
        return processModifiedNodesAfterDelete(node);
    }

    CONSTEXPR20 bool deleteNodePreProcess(node_type_ptr& node) {
        // We pass argument "node" by reference here for change it.
        if (node->left && node->right) {
            // Node has two children.
            // Get inorder successor. Smallest in the right tree.
            const auto node_min = getMinimumValueNode(node->right);
            node->key = node_min->key;
            // This one time recursion. The min node will never have a left child, only a right child maybe that grater than current node.
            node->right = deleteNodeInTree(node->right, node_min->key);
        } else {
            if (node->left) {
                // Node has only left child.
                node = node->left;
            } else if (node->right) {
                // Node has only right child.
                node = node->right;
            } else {
                // Node has no children at all.
                node = nullptr;
                // Don't rebalance on return.
                return false;
            }
        }

        // Rebalance on return.
        return true;
    }

    NODISCARD CONSTEXPR20 node_type_ptr processModifiedNodesAfterDelete(node_type_ptr node) {
        // Update the height.
        node->updateHeight();
        // Rebalance the node.
        return rebalanceDelete(node);
    }

    CONSTEXPR20 node_type_ptr rebalanceDelete(node_type_ptr node) {
        const signed_size_type balance_factor = node->getBalanceFactor();
        if (balance_factor > 1) {
            if (node->left->getBalanceFactor() >= 0) {
                // Left Left case.
                return rotateRight(node);
            } else {
                // Left Right case.
                node->left = rotateLeft(node->left);
                return rotateRight(node);
            }
        }
        else if (balance_factor < -1) {
            if (node->right->getBalanceFactor() <= 0) {
                // Right Right case.
                return rotateLeft(node);
            }
            else {
                // Right Left case.
                node->right = rotateRight(node->right);
                return rotateLeft(node);
            }
        }

        return node;
    }

    CONSTEXPR20 auto rotateLeft(node_type_ptr node) {
        const auto right_node = node->right;
        node->right = right_node->left;
        right_node->left = node;
        // Update the height.
        node->updateHeight();
        right_node->updateHeight();
        return right_node;
    }

    CONSTEXPR20 auto rotateRight(node_type_ptr node) {
        const auto left_node = node->left;
        node->left = left_node->right;
        left_node->right = node;
        // Update the height.
        node->updateHeight();
        left_node->updateHeight();
        return left_node;
    }

    CONSTEXPR20 node_type_ptr createNewNode(const value_type& key) const {
        return std::make_shared<node_type>(key);
    }

    CONSTEXPR20 node_type_ptr getMinimumValueNode(const node_type_ptr node) const {
        node_type_ptr node_current = node;

        while (node_current->left) {
            node_current = node_current->left;
        }

        return node_current;
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