#pragma once

#include <stdexcept>

#include "Node.h"

template <typename T>
class ListIterator {
private:
    Node<T> *_current;
public:
    explicit ListIterator(Node<T> *node) {
        _current = node;
    }
    ListIterator(const ListIterator<T> &other) {
        _current = other._current;
    }
    ~ListIterator() {
        _current = nullptr;
    }

    Node<T> getCurrent() {
        return _current;
    }
    T& operator*() {
        if (_current == nullptr) {
            throw std::runtime_error("Operator * to nullptr");
        }
        return _current->getData();
    }
    bool operator==(const ListIterator &other) const {
        return _current == other._current;
    }
    bool operator!=(const ListIterator &other) const {
        return _current != other._current;
    }
    ListIterator operator++() {
        _current = _current->getNext();
        return *this;
    }
    ListIterator operator++(int) {
        ListIterator tmp = *this;
        _current = _current->getNext();
        return tmp;
    }
    ListIterator<T> operator--() {
        Node<T> *temp = _current;
        _current = _current->getNext();
        while (_current->getNext() != temp) {
            _current = _current->getNext();
        }

        return *this;
    }
    ListIterator<T> operator--(int) {
        ListIterator<T> tmp = *this;
        ListIterator<T> tmp2 = _current;
        while (_current->getNext() != tmp2) {
            _current = _current->getNext();
        }

        return tmp;
    }
};
