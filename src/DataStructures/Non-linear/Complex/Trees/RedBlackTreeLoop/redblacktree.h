#pragma once
#include <iostream>
#include <string>
#include "head.h"

#define SENTINEL_DATA 0xDEADBEAF

enum class Node_Color {
    black = 0,
    red
};

template <class DataType, class SizeType = size_t>
struct RedBlackTreeNodeLoop {
    using value_type = DataType;
    using size_type = SizeType;
    using signed_size_type = std::make_signed_t<size_type>;
    using node_type = RedBlackTreeNodeLoop<value_type, size_type>;
    using node_type_ptr = std::shared_ptr<node_type>;

    value_type key {0};
    node_type_ptr left{};
    node_type_ptr right{};
    node_type_ptr parent{};
    Node_Color color{Node_Color::red};

    CONSTEXPR20 RedBlackTreeNodeLoop() = default;
    CONSTEXPR20 explicit RedBlackTreeNodeLoop(const value_type& key) : key(key) {}

    CONSTEXPR20 ~RedBlackTreeNodeLoop() = default;
};

template <class DataType, class SizeType = size_t>
class RedBlackTreeLoop {
private:
    using value_type = DataType;
    using const_reference = const DataType &;
    using size_type = SizeType;
    using signed_size_type = std::make_signed_t<size_type>;
    using node_type = RedBlackTreeNodeLoop<value_type, size_type>;
    using node_type_ptr = std::shared_ptr<node_type>;

    node_type_ptr node_sentinel{initSentinel()};
    node_type_ptr root{node_sentinel};

public:
    CONSTEXPR20 RedBlackTreeLoop() = default;

    CONSTEXPR20 explicit RedBlackTreeLoop(const std::vector<value_type> &vec) {
        const size_type vec_size = vec.size();
        for (size_type index = 0; index < vec_size; index++) {
            insert(vec[index]);
        }
    }

    CONSTEXPR20 explicit RedBlackTreeLoop(const value_type *start, const value_type *end) {
        size_type it_index = 0;
        for (auto it = start; it != end; it++, it_index++) {
            insert(*it);
        }
    }

    CONSTEXPR20 ~RedBlackTreeLoop() = default;

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
        if (node == node_sentinel) {
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
                    } while (node_next != node_sentinel);

                    tree_level--;
                } else {
                    node_current = node_next;
                }
                // Set the right node for next iteration.
                node_next = node_current->right;
                leftNode = false;
                get_second_node = false;
                tree_level++;
            } while (node_next != node_sentinel);

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
            } else {
                if (node_next == node_sentinel) {
                    // For reset node_sentinel
                    node_next = nullptr;
                }
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

        std::cout << output << node->key << "(" << (node->color == Node_Color::red? "red" : "black") << ")" << std::endl;
    }

    NODISCARD CONSTEXPR20 bool verifyCorrectness(const std::vector<std::pair<value_type, Node_Color>> &key_array) const {
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
    CONSTEXPR20 void setRoot(const node_type_ptr node) {
        root = node;
    }

    CONSTEXPR20 bool searchElement(const value_type &key) const {
        if (root != node_sentinel) {
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
        if (root != node_sentinel) {
            insertNodeInTree(root, key);
        } else {
            root = createNewNode(key);
            root->color = Node_Color::black;
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

        // We must start rebalance from new node.
        node_current = node_new;

        // Node has inserted in tree.
        if (node_current->parent->color == Node_Color::red) {
            rebalanceInsert(node_current);
        }
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
        } while (node_next != node_sentinel);

        return node_current;
    }

    CONSTEXPR20 void rebalanceInsert(node_type_ptr node) {
        auto node_current = node;

        do {
            auto node_parent = node_current->parent;
            auto node_grandparent = node_parent->parent;

            if (node_parent == node_grandparent->left) {
                auto node_uncle = node_grandparent->right;

                if (node_uncle->color == Node_Color::red) {
                    node_parent->color = Node_Color::black;
                    node_uncle->color = Node_Color::black;
                    node_grandparent->color = Node_Color::red;
                    node_current = node_grandparent;
                } else {
                    if (node_current == node_parent->right) {
                        node_current = node_parent;
                        auto top_node = rotateLeft(node_current);
                        changeRootIfNecessary(top_node);
                        // Update parent.
                        node_parent = node_current->parent;
                    }

                    node_parent->color = Node_Color::black;
                    node_grandparent->color = Node_Color::red;
                    auto top_node = rotateRight(node_grandparent);
                    changeRootIfNecessary(top_node);
                }
            } else {
                auto node_uncle = node_grandparent->left;

                if (node_uncle->color == Node_Color::red) {
                    node_parent->color = Node_Color::black;
                    node_uncle->color = Node_Color::black;
                    node_grandparent->color = Node_Color::red;
                    node_current = node_grandparent;
                } else {
                    if (node_current == node_parent->left) {
                        node_current = node_parent;
                        auto top_node = rotateRight(node_current);
                        changeRootIfNecessary(top_node);
                        // Update parent.
                        node_parent = node_current->parent;
                    }

                    node_parent->color = Node_Color::black;
                    node_grandparent->color = Node_Color::red;
                    auto top_node = rotateLeft(node_grandparent);
                    changeRootIfNecessary(top_node);
                }
            }
        } while(node_current != root && node_current->parent->color == Node_Color::red);

        // root
        root->color = Node_Color::black;
    }

    CONSTEXPR20 void deleteElement(const value_type &key) {
        if (root != node_sentinel) {
            deleteNodeInTree(root, key);
        }
    }

    CONSTEXPR20 void deleteNodeInTree(node_type_ptr node, const value_type &key) {
        // Search the node to delete.
        auto node_current = searchNodeForDelete(node, key);
        if (node_current == node_sentinel) {
            // The Node was not found.
            return;
        }

        // Delete the node.
        auto node_parent = node_current->parent;
        Node_Color original_color;
        std::tie(node_current, original_color) = deleteNodePreProcess(node_current, node_parent);
        if (!node_current) {
            // The node has no children and rebalance is not needed.
            if (!node_parent) {
                return;
            }
        }

        // Fix colors.
        if (original_color == Node_Color::black) {
            rebalanceDelete(node_current);
        }
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
        } while (node_current != node_sentinel);

        return node_current;
    }

    NODISCARD CONSTEXPR20 std::pair<node_type_ptr, Node_Color> deleteNodePreProcess(node_type_ptr node, node_type_ptr node_parent) {
        auto original_color = node->color;

        if (node->left != node_sentinel && node->right != node_sentinel) {
            // Node has two children.
            // Get inorder successor. Smallest in the right tree.
            auto [node_min, node_min_parent] = getMinimumValueNode(node->right, node);
            node->key = node_min->key;
            // Delete the old value's node.
            // This one time recursion. The min node will never have a left child, only a right child maybe that grater than current node.
            std::tie(node, original_color) = deleteNodePreProcess(node_min, node_min_parent);
        } else {
            if (node->left != node_sentinel) {
                // Node has only left child.
                auto left_node = node->left;
                // Fix relations.
                if (node_parent) {
                    swapChildNodeForParent(node, node_parent, left_node);
                }
                else {
                    removeParentLink(left_node);
                    setRoot(left_node);
                }
                // Delete the node.
                deleteNode(node);
                node = left_node;
            } else if (node->right != node_sentinel) {
                // Node has only right child.
                auto right_node = node->right;
                // Fix relations.
                if (node_parent) {
                    swapChildNodeForParent(node, node_parent, right_node);
                }
                else {
                    removeParentLink(right_node);
                    setRoot(right_node);
                }
                // Delete the node.
                deleteNode(node);
                node = right_node;
            } else {
                // Node has no children at all.
                if (node == root) {
                    deleteNode(node);
                    root = node_sentinel;
                    node = nullptr;
                } else {
                    // Fix parent.
                    if (node_parent->left == node) {
                        node_parent->left = node_sentinel;
                    }
                    else if (node_parent->right == node) {
                        node_parent->right = node_sentinel;
                    }

                    node = node_sentinel;
                    node_sentinel->parent = node_parent;
                }
            }
        }

        return {node, original_color};
    }

    CONSTEXPR20 void rebalanceDelete(node_type_ptr node) {
        auto node_current = node;

        while (node_current != root && node_current->color == Node_Color::black) {
            auto node_parent = node_current->parent;

            if (node_current == node_parent->left) {
                auto node_sibling = node_parent->right;

                if (node_sibling->color == Node_Color::red) {
                    // Set as black.
                    node_sibling->color = Node_Color::black;
                    node_parent->color = Node_Color::red;
                    auto top_node = rotateLeft(node_parent);
                    changeRootIfNecessary(top_node);
                    node_sibling = node_parent->right;
                }

                if (node_sibling->left->color == Node_Color::black && node_sibling->right->color == Node_Color::black) {
                    node_sibling->color = Node_Color::red;
                    node_current = node_parent;
                } else {
                    if (node_sibling->right->color == Node_Color::black) {
                        node_sibling->left->color = Node_Color::black;
                        node_sibling->color = Node_Color::red;
                        auto top_node = rotateRight(node_sibling);
                        changeRootIfNecessary(top_node);
                        node_sibling = node_parent->right;
                    }

                    node_sibling->color = node_parent->color;
                    node_parent->color = Node_Color::black;
                    node_sibling->right->color = Node_Color::black;
                    auto top_node = rotateLeft(node_parent);
                    changeRootIfNecessary(top_node);
                    node_current = root;
                }
            } else {
                auto node_sibling = node_parent->left;

                if (node_sibling->color == Node_Color::red) {
                    // Set as black.
                    node_sibling->color = Node_Color::black;
                    node_parent->color = Node_Color::red;
                    auto top_node = rotateRight(node_parent);
                    changeRootIfNecessary(top_node);
                    node_sibling = node_parent->left;
                }

                if (node_sibling->right->color == Node_Color::black && node_sibling->left->color == Node_Color::black) {
                    node_sibling->color = Node_Color::red;
                    node_current = node_parent;
                } else {
                    if (node_sibling->left->color == Node_Color::black) {
                        node_sibling->right->color = Node_Color::black;
                        node_sibling->color = Node_Color::red;
                        auto top_node = rotateLeft(node_sibling);
                        changeRootIfNecessary(top_node);
                        node_sibling = node_parent->left;
                    }

                    node_sibling->color = node_parent->color;
                    node_parent->color = Node_Color::black;
                    node_sibling->left->color = Node_Color::black;
                    auto top_node = rotateRight(node_parent);
                    changeRootIfNecessary(top_node);
                    node_current = root;
                }
            }
        }

        node_current->color = Node_Color::black;
    }

    CONSTEXPR20 void changeRootIfNecessary(node_type_ptr node) {
        if (!node->parent) {
            setRoot(node);
        }
    }

    CONSTEXPR20 auto rotateLeft(node_type_ptr node) const {
        const auto right_node = node->right;

        node->right = right_node->left;
        if (node->right != node_sentinel) {
            node->right->parent = node;
        }

        if (node->parent) {
            swapChildNodeForParent(node, node->parent, right_node);
        }
        else {
            removeParentLink(right_node);
        }

        right_node->left = node;
        right_node->left->parent = right_node;

        return right_node;
    }

    CONSTEXPR20 auto rotateRight(node_type_ptr node) const {
        const auto left_node = node->left;

        node->left = left_node->right;
        if (node->left != node_sentinel) {
            node->left->parent = node;
        }

        if (node->parent) {
            swapChildNodeForParent(node, node->parent, left_node);
        }
        else {
            removeParentLink(left_node);
        }

        left_node->right = node;
        left_node->right->parent = left_node;

        return left_node;
    }

    NODISCARD CONSTEXPR20 node_type_ptr createNewNode(const value_type& key) {
        auto node = std::make_shared<node_type>(key);
        node->left = node_sentinel;
        node->right = node_sentinel;
        return node;
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

    CONSTEXPR20 void removeParentLink(node_type_ptr node) const {
        node->parent = nullptr;
    }

    CONSTEXPR20 void deleteNode(node_type_ptr node) const {
        if (node->left != node_sentinel) {
            node->left = node_sentinel;
        }

        if (node->right != node_sentinel) {
            node->right = node_sentinel;
        }

        removeParentLink(node);
    }

    NODISCARD CONSTEXPR20 std::pair<node_type_ptr, node_type_ptr> getMinimumValueNode(node_type_ptr node, node_type_ptr node_start_parent) const {
        auto node_current = node;
        node_type_ptr node_prev;

        if (node_current->left == node_sentinel) {
            return {node_current, node_start_parent};
        }

        do {
            node_prev = node_current;
            node_current = node_current->left;
        } while (node_current->left != node_sentinel);

        return {node_current, node_prev};
    }

    NODISCARD CONSTEXPR20 node_type_ptr initSentinel() {
        auto node = std::make_shared<node_type>();
        node->color = Node_Color::black;
        node->left = nullptr;
        node->right = nullptr;
        *reinterpret_cast<int**>(&node->key) = reinterpret_cast<int*>(SENTINEL_DATA);
        return node;
    }

    NODISCARD CONSTEXPR20 bool verifyNode(node_type_ptr node, const std::vector<std::pair<value_type, Node_Color>>& key_array) const {
        if (node == node_sentinel) {
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

                        if (!areEqual(node_current->key, key_array[index].first)) {
                            return false;
                        }

                        if (node_current->color != key_array[index].second) {
                            return false;
                        }

                        index++;
                        // Set the left node for next iteration.
                        node_next = node_current->left;
                    } while (node_next != node_sentinel);
                }
                else {
                    node_current = node_next;
                }

                // Set the right node for next iteration.
                node_next = node_current->right;
                get_second_node = false;
            } while (node_next != node_sentinel);

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
            } else {
                // For reset node_sentinel
                node_next = nullptr;
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