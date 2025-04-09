#pragma once

#include "BinarySearchTree.h"
#include <random>
#include <queue> // Для вывода дерева
#include <stack>

template <typename Key, typename Data>
class RandomizedBinarySearchTree : public BinarySearchTree<Key, Data> {
private:
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_real_distribution<> dis;

    struct JoinResult {
        typename BinarySearchTree<Key, Data>::Node* root;
    };

    // RND_Join (a, b)
    JoinResult RND_Join(typename BinarySearchTree<Key, Data>::Node* a, typename BinarySearchTree<Key, Data>::Node* b) {
        JoinResult result;
        if (a == nullptr) {
            result.root = b;
            return result;
        }
        if (b == nullptr) {
            result.root = a;
            return result;
        }

        // Используем размер поддерева, если он доступен. Иначе считаем за 1.
        size_t size_a = (a) ? countNodes(a) : 1;
        size_t size_b = (b) ? countNodes(b) : 1;
        this->nodesVisited+=2;

        if (dis(gen) < (double)size_a / (size_a + size_b)) {
            JoinResult joinResult = RND_Join(a->right, b);
            a->right = joinResult.root;
            if (a->right) {
                a->right->parent = a;
                this->nodesVisited++;
            }
            result.root = a;
            return result;
        } else {
            JoinResult joinResult = RND_Join(a, b->left);
            b->left = joinResult.root;
            if (b->left) {
                b->left->parent = b;
                 this->nodesVisited++;
            }
            result.root = b;
            return result;
        }
    }

    size_t countNodes(typename BinarySearchTree<Key,Data>::Node* node){
        if (!node) return 0;
        return 1 + countNodes(node->left) + countNodes(node->right);
    }

    // RND_Delete (t, k, deleted) - Iterative
    typename BinarySearchTree<Key, Data>::Node* RND_Delete(typename BinarySearchTree<Key, Data>::Node* root, const Key& k, bool& deleted) {
        typename BinarySearchTree<Key, Data>::Node* current = root;
        typename BinarySearchTree<Key, Data>::Node* parent = nullptr;
        this->nodesVisited = 0; // Сброс счетчика посещенных узлов

        // Поиск удаляемого узла
        while (current != nullptr && current->key != k) {
            this->nodesVisited++;
            parent = current;
            if (k < current->key) {
                current = current->left;
            } else {
                current = current->right;
            }
        }

        // Если узел не найден
        if (current == nullptr) {
            deleted = false;
            return root;
        }

        // Узел найден
        typename BinarySearchTree<Key, Data>::Node* replacement;

        JoinResult joinResult = RND_Join(current->left, current->right);
        replacement = joinResult.root;

        // Замена узла в дереве
        if (parent == nullptr) {
            // Удаляем корень
            root = replacement;
        } else if (current == parent->left) {
            parent->left = replacement;
        } else {
            parent->right = replacement;
        }

        if(replacement){
            replacement->parent = parent;
            this->nodesVisited++; // Учитываем посещение при обновлении родителя
        }

        delete current;
        this->size--;
        deleted = true;

        return root;
    }

    // L(t) - Left Rotation
    typename BinarySearchTree<Key, Data>::Node* L(typename BinarySearchTree<Key, Data>::Node* t) {
        if (t == nullptr) {
            return nullptr;
        }
        typename BinarySearchTree<Key, Data>::Node* x = t->right;
        if (x == nullptr) return t;

        t->right = x->left;
        if (x->left != nullptr) {
            x->left->parent = t;
             this->nodesVisited++;
        }
        x->left = t;
        x->parent = t->parent;
        t->parent = x;
         this->nodesVisited+=4;

        return x;
    }

    // R(t) - Right Rotation
    typename BinarySearchTree<Key, Data>::Node* R(typename BinarySearchTree<Key, Data>::Node* t) {
        if (t == nullptr) {
            return nullptr;
        }
        typename BinarySearchTree<Key, Data>::Node* x = t->left;
         if (x == nullptr) return t;

        t->left = x->right;
        if (x->right != nullptr) {
            x->right->parent = t;
             this->nodesVisited++;
        }
        x->right = t;
        x->parent = t->parent;
        t->parent = x;
         this->nodesVisited+=4;

        return x;
    }

    // BST_Root_Insert(t, k, data, inserted) - Recursive
    typename BinarySearchTree<Key, Data>::Node* BST_Root_Insert(typename BinarySearchTree<Key, Data>::Node* t, const Key& k, const Data& data, bool& inserted) {
        this->nodesVisited++;
        if (t == nullptr) {
            t = new typename BinarySearchTree<Key, Data>::Node(k, data);
            t->left = nullptr;
            t->right = nullptr;
            t->parent = nullptr;
            inserted = true;
            return t;
        }

        if (k == t->key) {
            inserted = false;
            return t;
        }

        if (k < t->key) {
            t->left = BST_Root_Insert(t->left, k, data, inserted);
             if (t->left) t->left->parent = t;
            this->nodesVisited+=2;
            if (inserted) {
                t=R(t);
                this->nodesVisited++;
                return t;
            } else {
                return t;
            }
        } else {
            t->right = BST_Root_Insert(t->right, k, data, inserted);
             if (t->right) t->right->parent = t;
             this->nodesVisited+=2;
            if (inserted) {
                t=L(t);
                this->nodesVisited++;
                return t;
            } else {
                return t;
            }
        }
    }

    //Iterative RND_Insert with Probabilistic Root Insertion - псевдокод
    typename BinarySearchTree<Key, Data>::Node* RND_Insert(typename BinarySearchTree<Key, Data>::Node* root, const Key& k, const Data& data, bool& inserted) {
        typename BinarySearchTree<Key, Data>::Node* newNode = new typename BinarySearchTree<Key, Data>::Node(k, data);
        newNode->parent = nullptr;  // Initially no parent
        this->nodesVisited = 0;     // Reset node visit count

        // Calculate probability to insert at root
        // Рандомизация на вставку в корень
        if (rand() < RAND_MAX / (countNodes(root) + 1)) {
            // Insert at root - Rotate the tree
            newNode = BST_Root_Insert(root, k, data, inserted);
            this->root = newNode;
            inserted = true;
            this->size++;
            return newNode;
        }
        // Iteratively find the insertion position in the subtree
        else {
            // Iteratively find the insertion position in the subtree
            typename BinarySearchTree<Key, Data>::Node* current = root;
            typename BinarySearchTree<Key, Data>::Node* parent = nullptr;

            while (current != nullptr) {
                this->nodesVisited++;
                parent = current;
                if (k < current->key) {
                    current = current->left;
                } else if (k > current->key) {
                    current = current->right;
                } else {
                    // Key already exists - no insertion
                    inserted = false;
                    delete newNode;
                    return root; // Return root
                }
            }

            // Insert new node as leaf
            newNode->parent = parent;
            if (k < parent->key) {
                parent->left = newNode;
            } else {
                parent->right = newNode;
            }
            inserted = true;
            this->size++;
            return root;
        }
    }

    // Iterative Search
    typename BinarySearchTree<Key, Data>::Node* iterativeFindNode(const Key& key) const {
        this->nodesVisited = 0;
        typename BinarySearchTree<Key, Data>::Node* current = this->root;
        while (current) {
            this->nodesVisited++;
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

public:
    RandomizedBinarySearchTree() : BinarySearchTree<Key, Data>(), rd(), gen(rd()), dis(0.0, 1.0) {}

    // Override insert
    bool insert(const Key& key, const Data& data) {
        bool inserted = false;
        this->nodesVisited = 0; // Reset nodesVisited before insert
        this->root = RND_Insert(this->root, key, data, inserted);
        this->setRoot(this->root);
        return inserted;
    }


    // Override remove
    bool remove(const Key& key) {
        bool deleted = false;
        this->root = RND_Delete(this->root, key, deleted);
        return deleted;
    }

    //Override find
    bool contains(const Key& key) const {
        typename BinarySearchTree<Key, Data>::Node* node = iterativeFindNode(key);
        return node != nullptr;
    }

    void printLevelOrder() const {
        if (this->root == nullptr) {
            std::cout << "Tree is empty" << std::endl;
            return;
        }

        std::queue<typename BinarySearchTree<Key, Data>::Node*> q;
        q.push(this->root);

        while (!q.empty()) {
            typename BinarySearchTree<Key, Data>::Node* node = q.front();
            q.pop();

            std::cout << node->key << " ";

            if (node->left != nullptr) {
                q.push(node->left);
            }
            if (node->right != nullptr) {
                q.push(node->right);
            }
        }
        std::cout << std::endl;
    }
};