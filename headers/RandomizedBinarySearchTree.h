#pragma once
#include <random> // Для std::random_device и std::mt19937
#include <iostream> // Для std::cout и std::endl
#include "BinarySearchTree.h"

template <typename Key, typename Data>
class RandomizedBinarySearchTree : public BinarySearchTree<Key, Data> {
private:
    // Random number generation
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_real_distribution<> dis;

    // Helper function to rotate right (iterative)
    typename BinarySearchTree<Key,Data>::Node* rotateRight(typename BinarySearchTree<Key,Data>::Node* y) {
        typename BinarySearchTree<Key,Data>::Node* x = this->getLeft(y);
        typename BinarySearchTree<Key,Data>::Node* T2 = this->getRight(x);

        // Perform rotation
        this->setRight(x, y);
        this->setLeft(y, T2);

        // Update parents
        if (T2) this->setParent(T2, y);
        this->setParent(x, this->getParent(y));
        this->setParent(y, x);
        typename BinarySearchTree<Key,Data>::Node* grandParent = this->getParent(x);
        if (grandParent) {
            if (y == this->getLeft(grandParent))
                this->setLeft(grandParent, x);
            else
                this->setRight(grandParent, x);
        } else {
            this->setRoot(x); // x is the new root
        }

        return x;
    }

    // Helper function to rotate left (iterative)
    typename BinarySearchTree<Key,Data>::Node* rotateLeft(typename BinarySearchTree<Key,Data>::Node* x) {
        typename BinarySearchTree<Key,Data>::Node* y = this->getRight(x);
        typename BinarySearchTree<Key,Data>::Node* T2 = this->getLeft(y);

        // Perform rotation
        this->setLeft(y, x);
        this->setRight(x, T2);

        // Update parents
        if (T2) this->setParent(T2, x);
        this->setParent(y, this->getParent(x));
        this->setParent(x, y);
        typename BinarySearchTree<Key,Data>::Node* grandParent = this->getParent(y);
        if (grandParent) {
            if (x == this->getLeft(grandParent))
                this->setLeft(grandParent, y);
            else
                this->setRight(grandParent, y);
        } else {
            this->setRoot(y); // y is the new root
        }
        return y;
    }

    //Find the minimum node (iterative)
    typename BinarySearchTree<Key,Data>::Node* findMin(typename BinarySearchTree<Key,Data>::Node* node) {
        this->nodesVisited = 0;
        while (node && this->getLeft(node)) {
            ++this->nodesVisited;
            node = this->getLeft(node);
        }
        return node;
    }

public:
    // Constructor
    RandomizedBinarySearchTree() : gen(rd()), dis(0.0, 1.0) {}

    // Constructor (with seed for testing)
    RandomizedBinarySearchTree(unsigned int seed) : gen(seed), dis(0.0, 1.0) {}

    // Copy constructor
    RandomizedBinarySearchTree(const RandomizedBinarySearchTree& other) : BinarySearchTree<Key, Data>(other) , gen(rd()), dis(0.0, 1.0) {}

    // Assignment operator (optional, but good practice)
    RandomizedBinarySearchTree& operator=(const RandomizedBinarySearchTree& other) {
        if (this != &other) {
            // Clear existing tree
            clear();
            // Copy from other
            BinarySearchTree<Key, Data>::operator=(other); // Use base class assignment
             gen = std::mt19937(rd()); // Re-seed random number generator to be different
             dis = std::uniform_real_distribution<>(0.0, 1.0);
        }
        return *this;
    }

    // Destructor
    ~RandomizedBinarySearchTree() {
        clear(); // Use the inherited clear() method
    }

    // Size of the tree
    size_t getSize() const {
        return BinarySearchTree<Key, Data>::getSize();  // Access inherited size member
    }

    // Clear the tree
    void clear() {
        BinarySearchTree<Key, Data>::clear(); // Use inherited clear method
    }

    // Check if the tree is empty
    bool isEmpty() const {
        return BinarySearchTree<Key, Data>::isEmpty();
    }

    // Access data by key (read/write)
    Data& operator[](const Key& key) {
        try {
            return BinarySearchTree<Key,Data>::operator[](key);
        }
        catch(const std::runtime_error&){
            insert(key, Data()); // Default-constructed Data
            return BinarySearchTree<Key,Data>::operator[](key);
        }

    }

     // Insert data with the given key (iterative)
    bool insert(const Key& key, const Data& data) {
        typename BinarySearchTree<Key,Data>::Node* y = nullptr;
        typename BinarySearchTree<Key,Data>::Node* x = this->getRoot();
        typename BinarySearchTree<Key,Data>::Node* newNode = new typename BinarySearchTree<Key,Data>::Node(key, data);

        // 1. Find where to insert the new node
        while (x != nullptr) {
            y = x;
            if (key < this->getKey(x)) {
                x = this->getLeft(x);
                this->nodesVisited++;
            } else if (key > this->getKey(x)){
                x = this->getRight(x);
                this->nodesVisited++;
            } else {
                // Key already exists, return false
                delete newNode;
                return false;
            }
        }

        this->setParent(newNode, y);

        // 2. Insert the new node
        if (y == nullptr) {
            this->setRoot(newNode); // Tree was empty
        } else if (key < this->getKey(y)) {
            this->setLeft(y, newNode);
        } else {
            this->setRight(y,newNode);
        }

        ++this->size;

        // 3. Randomized Balancing:  "Treap" property - higher priority nodes stay closer to the root
        while (newNode != this->getRoot() && dis(gen) < 1.0 / (1.0 + this->size)) {
            if (newNode == this->getLeft(this->getParent(newNode))) {
                newNode = rotateRight(this->getParent(newNode));
            } else {
                newNode = rotateLeft(this->getParent(newNode));
            }
        }
        return true;
    }

    // Remove data with the given key (iterative)
    bool remove(const Key& key) {
        typename BinarySearchTree<Key,Data>::Node* node = this->findNode(key);
        if (node) {
            this->removeNode(node);
            return true;
        } else {
            return false;
        }
    }

private:
    // Helper function to transplant (iterative) -  Same as base class, but in derived class for access.
    void transplant(typename BinarySearchTree<Key,Data>::Node* u, typename BinarySearchTree<Key,Data>::Node* v) {
        if (!this->getParent(u)) {
            this->setRoot(v);
        } else if (u == this->getLeft(this->getParent(u))) {
            this->setLeft(this->getParent(u),v);
        } else {
            this->setRight(this->getParent(u),v);
        }
        if (v) {
            this->setParent(v,this->getParent(u));
        }
    }


    // Find node by key (iterative) -  Same as base class, but in derived class for access.
    typename BinarySearchTree<Key,Data>::Node* findNode(const Key& key) const {
        this->nodesVisited = 0;
        typename BinarySearchTree<Key,Data>::Node* current = this->getRoot();
        while (current) {
            ++this->nodesVisited;
            if (key == this->getKey(current)) {
                return current;
            } else if (key < this->getKey(current)) {
                current = this->getLeft(current);
            } else {
                current = this->getRight(current);
            }
        }
        return nullptr;
    }

        void removeNode(typename BinarySearchTree<Key,Data>::Node* node) {
        if (!this->getLeft(node)) {
            transplant(node, this->getRight(node));
        } else if (!this->getRight(node)) {
            transplant(node, this->getLeft(node));
        } else {
            typename BinarySearchTree<Key,Data>::Node* y = findMin(this->getRight(node));
            if (this->getParent(y) != node) {
                transplant(y, this->getRight(y));
                this->setRight(y,this->getRight(node));
                this->setParent(this->getRight(node),y);
            }
            transplant(node, y);
            this->setLeft(y,this->getLeft(node));
            this->setParent(this->getLeft(node),y);
        }
        delete node;
        --this->size;
    }

};