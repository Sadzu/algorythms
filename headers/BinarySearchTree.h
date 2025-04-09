#pragma once

#include <iostream>
#include <vector>
#include <stack>
#include <stdexcept>

template <typename Key, typename Data>
class BinarySearchTree {
protected: // Изменено на protected
    struct Node {
        Key key;
        Data data;
        Node* left;
        Node* right;
        Node* parent;

        Node(const Key& k, const Data& d, Node* p = nullptr)
            : key(k), data(d), left(nullptr), right(nullptr), parent(p) {}
    };

protected: // root теперь protected
    Node* root;
    size_t size;
    mutable size_t nodesVisited;

    // Protected accessors for Node members
    Node* getLeft(Node* node) const { return node ? node->left : nullptr; }
    Node* getRight(Node* node) const { return node ? node->right : nullptr; }
    Node* getParent(Node* node) const { return node ? node->parent : nullptr; }
    Key getKey(Node* node) const { return node->key; }
    Data& getData(Node* node) const { return node->data; }
    void setLeft(Node* node, Node* left) { if (node) node->left = left; }
    void setRight(Node* node, Node* right) { if (node) node->right = right; }
    void setParent(Node* node, Node* parent) { if (node) node->parent = parent; }
    Node* getRoot() const { return root; }
    void setRoot(Node *newRoot){root = newRoot;}

private:
    void destroyTree(Node* node) {
        if (node) {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }

    Node* copyTree(Node* node, Node* parent = nullptr) {
        if (!node) return nullptr;
        Node* newNode = new Node(node->key, node->data, parent);
        newNode->left = copyTree(node->left, newNode);
        newNode->right = copyTree(node->right, newNode);
        return newNode;
    }

    Node* findNode(const Key& key) const {
        nodesVisited = 0;
        Node* current = root;
        while (current) {
            ++nodesVisited;
            if (key == current->key) {
                return current;
            } else if (key < current->key) {
                current = current->left;
            } else {
                current = current->right;
            }
        }
        return nullptr;
    }

    static Node* findMin(Node* node, size_t& copyNodesVisited) {
        //copyNodesVisited = 0;
        while (node && node->left) {
            ++copyNodesVisited;
            node = node->left;
        }
        return node;
    }

    static Node* findMax(Node* node) {
        while (node && node->right) {
            node = node->right;
        }
        return node;
    }

    void inOrderTraversal(Node* node, std::vector<Key>& keys) const {
        if (node) {
            keys.push_back(node->key);
            inOrderTraversal(node->left, keys);
            inOrderTraversal(node->right, keys);
        }
    }

    void transplant(Node* u, Node* v) {
        if (!u->parent) {
            root = v;
        } else if (u == u->parent->left) {
            u->parent->left = v;
            nodesVisited++;
        } else {
            u->parent->right = v;
            nodesVisited++;
        }
        if (v) {
            v->parent = u->parent;
            nodesVisited++;
        }
    }

    void removeNode(Node* node) {
        if (!node->left) {
            transplant(node, node->right);
        } else if (!node->right) {
            transplant(node, node->left);
        } else {
            Node* y = findMin(node->right, nodesVisited);
            if (y->parent != node) {
                transplant(y, y->right);
                y->right = node->right;
                nodesVisited++;
                y->right->parent = y;
                nodesVisited++;
            }
            transplant(node, y);
            y->left = node->left;
            nodesVisited++;
            y->left->parent = y;
            nodesVisited++;
        }
        delete node;
        --size;
    }

    void printTree(Node* node, int level = 0) const {
        if (node) {
            printTree(node->right, level + 1);
            for (int i = 0; i < level; ++i) {
                std::cout << "    ";
            }
            std::cout << node->key << " (" << node->data << ")" << std::endl;
            printTree(node->left, level + 1);
        }
    }

    size_t countNodesGreaterThan(const Key& key) const {
        size_t count = 0;
        Node* current = root;
        while (current) {
            if (key < current->key) {
                count += 1 + countNodes(current->right);
                current = current->left;
            } else {
                current = current->right;
            }
        }
        return count;
    }

    size_t countNodes(Node* node) const {
        if (!node) return 0;
        return 1 + countNodes(node->left) + countNodes(node->right);
    }

    static Node* findPrevious(Node* node) {
        if (node->left) {
            return findMax(node->left);
        }
        Node* parent = node->parent;
        while (parent && node == parent->left) {
            node = parent;
            parent = parent->parent;
        }
        return parent;
    }

    static Node* findNext(Node* node) {
        if (node->right) {
            size_t a = 0;
            return findMin(node->right, a);
        }
        Node* parent = node->parent;
        while (parent && node == parent->right) {
            node = parent;
            parent = parent->parent;
        }
        return parent;
    }

public:
    class iterator {
    private:
        Node* current;
        BinarySearchTree* tree;

    public:
        iterator(Node* node, BinarySearchTree* tree_ptr) : current(node), tree(tree_ptr) {}

        iterator& operator++() {
            if (!current) return *this;

            Node* next_node = findNext(current);
            current = next_node;
            return *this;
        }

        iterator operator++(int) {
            iterator temp = *this;
            ++(*this);
            return temp;
        }

        iterator& operator--() {
            if (!current) { // Special case: if already at end(), go to last element
                current = tree->findMax(tree->root);
                return *this;
            }

            Node* prev_node = findPrevious(current);
            current = prev_node;
            return *this;
        }

        iterator operator--(int) {
            iterator temp = *this;
            --(*this);
            return temp;
        }

        bool operator==(const iterator& other) const {
            return current == other.current;
        }

        bool operator!=(const iterator& other) const {
            return !(*this == other);
        }

        Data& operator*() {
            if (current == nullptr) {
                throw std::runtime_error("Dereference of end() iterator");
            }
            return current->data;
        }

        std::pair<const Key&, Data&> operator->() {
            return std::make_pair(current->key, current->data);
        }
    };


    // Reverse Iterator
    class reverse_iterator {
    private:
        Node* current;
        BinarySearchTree* tree;

    public:
        reverse_iterator(Node* node, BinarySearchTree* tree_ptr) : current(node), tree(tree_ptr) {}

        reverse_iterator& operator++() {  // Note: ++ goes *backwards* for reverse iterator
           if (!current) return *this;

            Node* prev_node = findPrevious(current); // findPrevious still goes to the *logical* previous, even in reverse
            current = prev_node;
            return *this;
        }

        reverse_iterator operator++(int) {
            reverse_iterator temp = *this;
            ++(*this);
            return temp;
        }

        reverse_iterator& operator--() {  // Decrement goes forward in the *logical* sense for reverse iterators
            if (!current) { // Special case: if already at rend(), go to last element
                size_t a = 0;
                current = tree->findMin(tree->root, a);
                return *this;
            }

            Node* next_node = findNext(current); // findNext still goes to the *logical* next, even in reverse
            current = next_node;
            return *this;
        }


        reverse_iterator operator--(int) {
            reverse_iterator temp = *this;
            --(*this);
            return temp;
        }

        bool operator==(const reverse_iterator& other) const {
            return current == other.current;
        }

        bool operator!=(const reverse_iterator& other) const {
            return !(*this == other);
        }

        Data& operator*() {
            if (current == nullptr) {
                throw std::runtime_error("Dereference of end() iterator");
            }
            return current->data;
        }

        std::pair<const Key&, Data&> operator->() {
            return std::make_pair(current->key, current->data);
        }
    };

    iterator begin() {
        size_t a = 0;
        return iterator(findMin(root, a), this);
    }

    iterator end() {
        return iterator(nullptr, this);  // End iterator points to nullptr
    }

    reverse_iterator rbegin() {
        return reverse_iterator(findMax(root), this);
    }

    reverse_iterator rend() {
        return reverse_iterator(nullptr, this); // End iterator points to nullptr
    }

    BinarySearchTree() : root(nullptr), size(0) {
        nodesVisited = 0;
    }

    BinarySearchTree(const BinarySearchTree& other) : root(nullptr), size(0) {
        root = copyTree(other.root);
        size = other.size;
        nodesVisited = 0;
    }

    ~BinarySearchTree() {
        destroyTree(root);
    }

    size_t getSize() const {
        return size;
    }

    void clear() {
        destroyTree(root);
        root = nullptr;
        size = 0;
    }

    bool isEmpty() const {
        return root == nullptr;
    }

    Data& operator[](const Key& key) {
        Node* node = findNode(key);
        if (node) return node->data;
        throw std::runtime_error("Key not found");
    }

    bool insert(const Key& key, const Data& data) {
        nodesVisited = 0;
        Node* newNode = new Node(key, data);
        if (!root) {
            root = newNode;
        } else {
            Node* current = root;
            Node* parent = nullptr;
            while (current) {
                parent = current;
                if (key < current->key) {
                    current = current->left;
                    nodesVisited++;
                } else if (key > current->key) {
                    current = current->right;
                    nodesVisited++;
                } else {
                    return false;
                }
            }
            newNode->parent = parent;
            if (key < parent->key) {
                parent->left = newNode;
            } else {
                parent->right = newNode;
            }
        }
        ++size;

        return true;
    }

    bool remove(const Key& key) {
        Node* node = findNode(key);
        if (node) {
            removeNode(node);
            return true;
        } else {
            return false;
        }
    }

    std::vector<Key> getKeys() const {
        std::vector<Key> keys;
        inOrderTraversal(root, keys);
        return keys;
    }

    void print() const {
        printTree(root);
    }

    size_t getNodesVisited() const {
        return nodesVisited;
    }

    size_t countGreaterThan(const Key& key) const {
        return countNodesGreaterThan(key);
    }

    bool contains(const Key& key) const {
        Node* node = findNode(key);
        return node != nullptr;
    }
};