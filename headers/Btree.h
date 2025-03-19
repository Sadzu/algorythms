#pragma once

#include <iostream>

template <typename K, typename V>
class Btree {
 private:
   struct Node {
     K _key;
     V _data;
     Node *left;
     Node *right;
     Node *parent;
     Node() {
       _key = 0;
       _data = 0;
       left = nullptr;
       right = nullptr;
       parent = nullptr;
     }
   };
   Node *root;
 public:
   Btree() {
     root = nullptr;
   }
   Btree(Btree &&other) {
     if (root != nullptr) {
       clear();
     }
     root = new Node();
     root->_key = other.root->_key;
     root->_data = other.root->_data;
     root->left = nullptr;
     root->right = nullptr;
     root->parent = nullptr;
   }
   ~Btree() {
     clear();
   }

   void add(K key, V data) {
       if (root == nullptr) {
         root = new Node();
         root->_key = key;
         root->_data = data;
         return;
       }
       Node *cur = root;
       while (true) {
           if (key < cur->_key) {
             if (cur->left == nullptr) {
               cur->left = new Node();
               cur->left->_key = key;
               cur->left->_data = data;
               cur->left->parent = cur;
               return;
             } else {
               cur = cur->left;
             }
           } else if (key > cur->_key) {
             if (cur->right == nullptr) {
               cur->right = new Node();
               cur->right->_key = key;
               cur->right->_data = data;
               cur->right->parent = cur;
               return;
             } else {
               cur = cur->right;
             }
           } else {
             break;
           }
       }
   }
   void clear() {
     while (root != nullptr) {
       delete findMinimum();
     }
   }
   Node* findMinimum() {
     if (root == nullptr) {
       return nullptr;
     }
     Node *cur = root;
     while (true) {
       if (cur->left == nullptr && cur->right == nullptr) {
         return cur;
       } else if (cur->left != nullptr) {
         cur = cur->left;
       } else {
         cur = cur->right;
       }
     }
   }
   Node* findMaximum() {
     if (root == nullptr) {
       return nullptr;
     }
     Node *cur = root;
     while (true) {
       if (cur->right == nullptr && cur->left == nullptr) {
         return cur;
       } else if (cur->right != nullptr) {
         cur = cur->right;
       } else {
         cur = cur->left;
       }
     }
   }
   bool isEmpty() {
     return root == nullptr;
   }
   V& get(K key) {
     Node *cur = root;
     while (true) {
       if (cur->_key == key) {
         return cur->_data;
       } else if (cur->_key < key) {
         if (cur->left == nullptr) {
           throw std::out_of_range("Key not found");
         } else {
           cur = cur->left;
         }
       } else if (cur->_key > key) {
         if (cur->right == nullptr) {
           throw std::out_of_range("Key not found");
         } else {
           cur = cur->right;
         }
       }
     }
   }
   Node* getNode(K key) {
     Node *cur = root;
     while (true) {
       if (cur->_key == key) {
         return cur;
       } else if (cur->_key < key) {
         if (cur->left == nullptr) {
           throw std::out_of_range("Key not found");
         } else {
           cur = cur->left;
         }
       } else if (cur->_key > key) {
         if (cur->right == nullptr) {
           throw std::out_of_range("Key not found");
         } else {
           cur = cur->right;
         }
       }
     }
   }
   // void deleteNode(K key) {
   //   Node *cur = getNode(key);
   //   Node *parent = cur->parent;
   //   if (cur->left == nullptr && cur->right == nullptr) {
   //     if (parent == )
   //   } else if (cur->left == nullptr || cur->right == nullptr) {
   //     if (cur->left == nullptr) {
   //       if (parent == nullptr) {}
   //     }
   //   }
   // }
};