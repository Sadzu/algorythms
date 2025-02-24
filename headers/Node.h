#pragma once

#include <cstddef>

template <typename T>
class Node {
private:
    T _data;
    Node<T> *_next;
    size_t _id;
public:
    Node() {
        _data = nullptr;
        _next = nullptr;
        _id = 0;
    }
    Node(T data) {
        _data = data;
        _next = nullptr;
        _id = 0;
    }
    Node(const Node<T> &node) {
        _data = node->_data;
        _next = node->_next;
        _id = node->_id;
    }
    ~Node() {
        _next = nullptr;
        _id = 0;
    }

    void setData(T data) {
        _data = data;
    }
    void setNext(Node<T> *node) {
        _next = node;
    }
    void setId(const size_t id) {
        _id = id;
    }

    T getData() {
        return _data;
    }
    size_t getId() {
        return _id;
    }
    Node<T>* getNext() {
        return _next;
    }
};
