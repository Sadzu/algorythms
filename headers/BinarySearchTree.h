#include <iostream>
#include <vector>
#include <stack>
#include <stdexcept>
#include <algorithm>

template <typename Key, typename Data>
class BinarySearchTree {
private:
    struct Node {
        Key key;
        Data data;
        Node* left;
        Node* right;
        Node* parent;

        Node(const Key& k, const Data& d, Node* p = nullptr)
            : key(k), data(d), left(nullptr), right(nullptr), parent(p) {}
    };

    Node* root;
    size_t size;
    mutable size_t nodesVisited;

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

    static Node* findMin(Node* node) {
        while (node && node->left) {
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
            inOrderTraversal(node->left, keys);
            keys.push_back(node->key);
            inOrderTraversal(node->right, keys);
        }
    }

    void transplant(Node* u, Node* v) {
        if (!u->parent) {
            root = v;
        } else if (u == u->parent->left) {
            u->parent->left = v;
        } else {
            u->parent->right = v;
        }
        if (v) {
            v->parent = u->parent;
        }
    }

    void removeNode(Node* node) {
        if (!node->left) {
            transplant(node, node->right);
        } else if (!node->right) {
            transplant(node, node->left);
        } else {
            Node* y = findMin(node->right);
            if (y->parent != node) {
                transplant(y, y->right);
                y->right = node->right;
                y->right->parent = y;
            }
            transplant(node, y);
            y->left = node->left;
            y->left->parent = y;
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
            return findMin(node->right);
        }
        Node* parent = node->parent;
        while (parent && node == parent->right) {
            node = parent;
            parent = parent->parent;
        }
        return parent;
    }

public:
    class Iterator {
    protected:
        Node* current;
        std::stack<Node*> stack;
        Node *_root;

        void pushLeft(Node* node) {
            while (node) {
                stack.push(node);
                node = node->left;
            }
        }

    public:
        Iterator(Node* node = nullptr, Node *baseRoot = nullptr) : current(node), _root(baseRoot) {
            if (current) pushLeft(current);
        }

        Data& operator*() {
            if (current) return current->data;
            throw std::runtime_error("Dereference of end iterator");
        }

        Iterator& operator++() {
            if (stack.empty()) {
                current = nullptr;
                return *this;
            }
            current = stack.top();
            stack.pop();
            if (current->right) pushLeft(current->right);
            return *this;
        }

        Iterator operator++(int) {
            Iterator temp = *this;
            ++(*this);
            return temp;
        }

        Iterator& operator--() {
            if (!current) {
                current = findMax(_root);
                pushLeft(current);
                return *this;
            }
            current = findPrevious(current);
            return *this;
        }

        Iterator operator--(int) {
            Iterator temp = *this;
            --(*this);
            return temp;
        }

        bool operator==(const Iterator& other) const {
            return current == other.current;
        }

        bool operator!=(const Iterator& other) const {
            return !(*this == other);
        }
    };

    class ReverseIterator {
    protected:
        Node* current;
        std::stack<Node*> stack;
        Node *_root;

        void pushRight(Node* node) {
            while (node) {
                stack.push(node);
                node = node->right;
            }
        }

    public:
        ReverseIterator(Node* node = nullptr, Node *baseRoot = nullptr) : current(node), _root(baseRoot) {
            if (current) pushRight(current);
        }

        Data& operator*() {
            if (current) return current->data;
            throw std::runtime_error("Dereference of end iterator");
        }

        ReverseIterator& operator++() {
            if (stack.empty()) {
                current = nullptr;
                return *this;
            }
            current = stack.top();
            stack.pop();
            if (current->left) pushRight(current->left);
            return *this;
        }

        ReverseIterator operator++(int) {
            ReverseIterator temp = *this;
            ++(*this);
            return temp;
        }

        ReverseIterator& operator--() {
            if (!current) {
                current = findMin(_root);
                pushRight(current);
                return *this;
            }
            current = findNext(current);
            return *this;
        }

        ReverseIterator operator--(int) {
            ReverseIterator temp = *this;
            --(*this);
            return temp;
        }

        bool operator==(const ReverseIterator& other) const {
            return current == other.current;
        }

        bool operator!=(const ReverseIterator& other) const {
            return !(*this == other);
        }
    };

    BinarySearchTree() : root(nullptr), size(0), nodesVisited(0) {}

    BinarySearchTree(const BinarySearchTree& other) : root(nullptr), size(0), nodesVisited(0) {
        root = copyTree(other.root);
        size = other.size;
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

    void insert(const Key& key, const Data& data) {
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
                } else {
                    current = current->right;
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

    Iterator begin() const {
        return Iterator(findMin(root), root);
    }

    Iterator end() const {
        return Iterator();
    }

    ReverseIterator rbegin() const {
        return ReverseIterator(findMax(root), root);
    }

    ReverseIterator rend() const {
        return ReverseIterator();
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
};
