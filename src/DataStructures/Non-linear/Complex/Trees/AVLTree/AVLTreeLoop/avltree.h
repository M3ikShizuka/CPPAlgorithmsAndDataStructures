// The loop method is better than recursion:
// + No stack overflow with big number of items.
// + Lower performance costs than the recursion method.
// - It uses an extra field in node for store a link to parent.
#pragma once
#include <iostream>
#include <string>
#include "head.h"

template <class DataType, class SizeType = size_t>
struct AVLTreeNodeLoop {
    using value_type = DataType;
    using size_type = SizeType;
    using signed_size_type = std::make_signed_t<size_type>;
    using node_type = AVLTreeNodeLoop<value_type, size_type>;
    using node_type_ptr = std::shared_ptr<node_type>;

    value_type key {0};
    node_type_ptr left{};
    node_type_ptr right{};
    node_type_ptr parent{};
    size_type height {1};

    CONSTEXPR20 AVLTreeNodeLoop() = default;
    CONSTEXPR20 explicit AVLTreeNodeLoop(const value_type& key) : key(key) {}

    CONSTEXPR20 ~AVLTreeNodeLoop() = default;

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
class AVLTreeLoop {
private:
    using value_type = DataType;
    using const_reference = const DataType &;
    using size_type = SizeType;
    using signed_size_type = std::make_signed_t<size_type>;
    using node_type = AVLTreeNodeLoop<value_type, size_type>;
    using node_type_ptr = std::shared_ptr<node_type>;

    node_type_ptr root{};

public:
    CONSTEXPR20 AVLTreeLoop() = default;

    CONSTEXPR20 explicit AVLTreeLoop(const std::vector<value_type> &vec) {
        const size_type vec_size = vec.size();
        for (size_type index = 0; index < vec_size; index++) {
            insert(vec[index]);
        }
    }

    CONSTEXPR20 explicit AVLTreeLoop(const value_type *start, const value_type *end) {
        size_type it_index = 0;
        for (auto it = start; it != end; it++, it_index++) {
            insert(*it);
        }
    }

    CONSTEXPR20 ~AVLTreeLoop() = default;

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
    void printTree(node_type_ptr node, bool leftNode = false) {
        if (!node) {
            return;
        }

        std::string indent{};
        size_type tree_level = 0;
        node_type_ptr node_prev;
        node_type_ptr node_current;
        auto node_next = node;
        bool get_second_node = false;
        // Loop to move up to the node's parent.
        do {
            // Loop to move down to the tree's right node.
            do {
                // Checks if the left child node of this node has already been processed.
                if (!get_second_node) {
                    // Loop to move down to the tree's left node.
                    do {
                        node_current = node_next;
                        // Process current node.
                        printNodeInfo(node_current, tree_level, indent, leftNode);
                        // Set the left node for next iteration.
                        node_next = node_current->left;
                        leftNode = true;
                        tree_level++;
                    } while (node_next);

                    tree_level--;
                } else {
                    node_current = node_next;
                }
                // Set the right node for next iteration.
                node_next = node_current->right;
                leftNode = false;
                get_second_node = false;
                tree_level++;
            } while (node_next);

            tree_level--;

            // Find next node in the another subtree for next iteration.
            // Find out whether node_current is the left or right node for parent.
            if (node_current->parent) {
                // If node_current is the right node to its parent, then continue up the tree.
                node_next = node_current;
                do {
                    node_prev = node_next;
                    node_next = node_next->parent;
                    tree_level--;
                } while (node_next && node_next->right == node_prev);

                get_second_node = true;
            }
        } while (node_next);
    }

    void printNodeInfo(node_type_ptr node, size_type &tree_level, std::string &indent,
                       bool leftNode) {
        indent = indent.substr(0, tree_level * 3);
        std::string output = indent;

        if (leftNode) {
            output += "L----";
            indent += "|  ";
        } else {
            output += "R----";
            indent += "   ";
        }

        std::cout << output << node->key << "(h: " << node->height << ")" << std::endl;
    }

    NODISCARD CONSTEXPR20 bool verifyCorrectness(const std::vector<value_type> &key_array) const {
        // Verify the correctness of the tree.
        std::cout << "Verify correctness:" << std::endl;
        const bool result = verifyNode(root, key_array);
        if (result) {
            std::cout << std::endl << "Verification successful" << std::endl;
        } else {
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
            root = insertNodeInTree(root, key);
        } else {
            root = createNewNode(key);
        }
    }

    CONSTEXPR20 node_type_ptr insertNodeInTree(node_type_ptr node, const value_type &key) {
        // Search the node to insert.
        auto node_current = searchNodeForInsert(node, key);
        if (!node_current) {
            // The same key value already exist in tree.
            return root;
        }

        // Create and insert node.
        auto node_new = createNewNode(key);
        // Make a link with the previous node.
        makeLinkWithPreviousNode(node_new, node_current);

        // Node has inserted in tree.
        return processModifiedNodesAfterInsert(key, node_current);
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

    NODISCARD CONSTEXPR20 node_type_ptr processModifiedNodesAfterInsert(const value_type& key, node_type_ptr from_node) {
        // Update the height and rebalance.
        auto node_next = from_node;
        node_type_ptr node_current;

        do {
            node_current = node_next;
            // Update the height.
            node_current->updateHeight();
            // Rebalance the node.
            node_current = rebalanceInsert(node_current, key);
            // Get parent node.
            node_next = node_current->parent;
            if (node_next) {
                // Set node_current to parent node.
                if (isLess(key, node_next->key)) {
                    // left subtree
                    node_next->left = node_current;
                } else if (isGreater(key, node_next->key)) {
                    // Right Left case.
                    node_next->right = node_current;
                }
            }
        } while (node_next);

        return node_current;
    }

    CONSTEXPR20 node_type_ptr rebalanceInsert(node_type_ptr node, const value_type &key) {
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
        } else if (balance_factor < -1) {
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

    CONSTEXPR20 void deleteElement(const value_type &key) {
        if (root) {
            root = deleteNodeInTree(root, key);
        }
    }

    CONSTEXPR20 node_type_ptr deleteNodeInTree(node_type_ptr node, const value_type &key) {
        // Search the node to delete.
        auto node_current = searchNodeForDelete(node, key);
        if (!node_current) {
            // The Node was not found.
            return root;
        }

        // Delete the node.
        auto node_parent = node_current->parent;
        node_current = deleteNodePreProcess(node_current, node_parent);
        if (!node_current) {
            // The node has no children and rebalance is not needed.
            if (!node_parent) {
                return root;
            }

            // The rebalance is needed for parent node.
            node_current = node_parent;
        }

        // The Node was found.
        return processModifiedNodesAfterDelete(key, node_current);
    }

    CONSTEXPR20 node_type_ptr searchNodeForDelete(node_type_ptr node, const value_type &key) {
        auto node_current = node;

        do {
            if (isLess(key, node_current->key)) {
                // left subtree
                node_current = node_current->left;
            } else if (isGreater(key, node_current->key)) {
                // right subtree
                node_current = node_current->right;
            } else {
                // Node found, delete it.
                break;
            }
        } while (node_current);

        return node_current;
    }

    CONSTEXPR20 node_type_ptr deleteNodePreProcess(node_type_ptr node, node_type_ptr node_parent) {
        if (node->left && node->right) {
            // Node has two children.
            // Get inorder successor. Smallest in the right tree.
            auto node_min = getMinimumValueNode(node->right);
            auto node_old = node;
            auto node_min_parent = node_min->parent;
            const auto node_min_key = node_min->key;
            node->key = node_min->key;
            // Delete the old value's node.
            // This one time recursion. The min node will never have a left child, only a right child maybe that grater than current node.
            node_min = deleteNodePreProcess(node_min, node_min_parent);
            if (node_min) {
                node->right = processModifiedNodesAfterDelete(node_min_key, node_min, node_old);
            }
            else if (node != node_min_parent) { // The "else if" Check is necessary.
                node->right = processModifiedNodesAfterDelete(node_min_key, node_min_parent, node_old);
            }
            // The rebalance is not needed.
        } else {
            if (node->left) {
                // Node has only left child.
                auto left_node = node->left;
                // Fix relations.
                if (node_parent) {
                    swapChildNodeForParent(node, node_parent, left_node);
                }
                // Delete the node.
                deleteNode(node);
                node = left_node;
                // The rebalance is not needed.
            } else if (node->right) {
                // Node has only right child.
                auto right_node = node->right;
                // Fix relations.
                if (node_parent) {
                    swapChildNodeForParent(node, node_parent, right_node);
                }
                // Delete the node.
                deleteNode(node);
                node = right_node;
                // The rebalance is not needed.
            } else {
                // Node has no children at all.
                deleteNode(node);

                if (node == root) {
                    root = nullptr;
                }

                node = nullptr;
                // The rebalance is needed for parent node.
            }
        }

        return node;
    }

    NODISCARD CONSTEXPR20 node_type_ptr processModifiedNodesAfterDelete(const value_type& key, node_type_ptr from_node, node_type_ptr to_node = nullptr) {
        auto node_next = from_node;
        node_type_ptr node_current;

        do {
            node_current = node_next;
            // Update the height.
            node_current->updateHeight();
            // Rebalance the node.
            node_current = rebalanceDelete(node_current);
            // Get parent node.
            node_next = node_current->parent;
            if (node_next) {
                // Set node_current to parent node.
                if (isLess(key, node_next->key)) {
                    // left subtree
                    node_next->left = node_current;
                }
                else if (isGreater(key, node_next->key)) {
                    // Right Left case.
                    node_next->right = node_current;
                }
            }
        } while(node_next != to_node);

        return node_current;
    }

    CONSTEXPR20 node_type_ptr rebalanceDelete(node_type_ptr node) {
        const signed_size_type balance_factor = node->getBalanceFactor();
        if (balance_factor > 1) {
            if (node->left->getBalanceFactor() >= 0) {
                // Left Left case.
                return rotateRight(node);
            }
            else {
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
        if (node->right) {
            node->right->parent = node;
        }

        right_node->parent = node->parent;
        right_node->left = node;
        right_node->left->parent = right_node;

        // Update the height.
        node->updateHeight();
        right_node->updateHeight();
        return right_node;
    }

    CONSTEXPR20 auto rotateRight(node_type_ptr node) {
        const auto left_node = node->left;

        node->left = left_node->right;
        if (node->left) {
            node->left->parent = node;
        }

        left_node->parent = node->parent;
        left_node->right = node;
        left_node->right->parent = left_node;

        // Update the height.
        node->updateHeight();
        left_node->updateHeight();
        return left_node;
    }

    CONSTEXPR20 node_type_ptr createNewNode(const value_type& key) const {
        return std::make_shared<node_type>(key);
    }

    CONSTEXPR20 void makeLinkWithPreviousNode(node_type_ptr node, node_type_ptr node_prev) const {
        node->parent = node_prev;

        if (isLess(node->key, node_prev->key)) {
            // left subtree
            node_prev->left = node;
        } else if (isGreater(node->key, node_prev->key)) {
            // right subtree
            node_prev->right = node;
        }
    }

    CONSTEXPR20 void swapChildNodeForParent(node_type_ptr node, node_type_ptr node_parent, node_type_ptr node_new_child) const {
        node_new_child->parent = node_parent;

        if (node_parent->left == node) {
            node_parent->left = node_new_child;
        } else if (node_parent->right == node) {
            node_parent->right = node_new_child;
        }
    }

    CONSTEXPR20 void deleteNode(node_type_ptr node) const {
        if (node->left) {
            node->left = nullptr;
        }

        if (node->right) {
            node->right = nullptr;
        }

        auto parent_node = node->parent;
        if (parent_node) {
            node->parent = nullptr;

            if (parent_node->left == node) {
                parent_node->left = nullptr;
            }
            else if (parent_node->right == node) {
                parent_node->right = nullptr;
            }
        }
    }

    NODISCARD CONSTEXPR20 node_type_ptr getMinimumValueNode(node_type_ptr node) const {
        node_type_ptr node_current = node;

        while (node_current->left) {
            node_current = node_current->left;
        }

        return node_current;
    }

    NODISCARD CONSTEXPR20 bool verifyNode(node_type_ptr node, const std::vector<value_type>& key_array) const {
        if (!node) {
            if (key_array.empty()) {
                return true;
            }

            return false;
        }

        const auto key_array_size = key_array.size();
        size_type index = 0;
        node_type_ptr node_prev;
        node_type_ptr node_current;
        auto node_next = node;
        bool get_second_node = false;
        // Loop to move up to the node's parent.
        do {
            // Loop to move down to the tree's right node.
            do {
                // Checks if the left child node of this node has already been processed.
                if (!get_second_node) {
                    // Loop to move down to the tree's left node.
                    do {
                        node_current = node_next;
                        // Process current node.
                        if (key_array_size <= index) {
                            return false;
                        }

                        if (!areEqual(node_current->key, key_array[index])) {
                            return false;
                        }

                        index++;
                        // Set the left node for next iteration.
                        node_next = node_current->left;
                    } while (node_next);
                }
                else {
                    node_current = node_next;
                }

                // Set the right node for next iteration.
                node_next = node_current->right;
                get_second_node = false;
            } while (node_next);

            // Find next node in the another subtree for next iteration.
            // Find out whether node_current is the left or right node for parent.
            if (node_current->parent) {
                // If node_current is the right node to its parent, then continue up the tree.
                node_next = node_current;
                do {
                    node_prev = node_next;
                    node_next = node_next->parent;
                } while (node_next && node_next->right == node_prev);

                get_second_node = true;
            }
        } while(node_next);

        if (key_array_size != index) {
            return false;
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