#pragma once

#include "../headers/Node.h"

#include <iostream>

template <typename T>
class OneLinkedList {
    friend class Node<T>;
private:
    size_t _size;
    Node<T> *_head;
    Node<T> *_tail;
public:
    class ListIterator {
    private:
        Node<T> *_current;
    public:
        explicit ListIterator(Node<T> *node) {
            _current = node;
        }
        ListIterator(const ListIterator &other) {
            _current = other._current;
        }
        ~ListIterator() {
            _current = nullptr;
        }

        Node<T>* getCurrent() {
            return _current;
        }
        T& operator*() {
            if (_current == nullptr) {
                throw std::runtime_error("Operator * to nullptr");
            }
            T& n = _current->_data;
            return n;
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
        ListIterator operator--() {
            Node<T> *temp = _current;
            _current = _current->getNext();
            while (_current->getNext() != temp) {
                _current = _current->getNext();
            }

            return *this;
        }
        ListIterator operator--(int) {
            ListIterator tmp = *this;
            ListIterator tmp2 = _current;
            while (_current->getNext() != tmp2) {
                _current = _current->getNext();
            }

            return tmp;
        }
    };
    OneLinkedList() {
        _size = 0;
        _head = nullptr;
        _tail = nullptr;
    }
    OneLinkedList(OneLinkedList &list) {
        if (_head != nullptr) {
            clear();
        }
        _size = 0;
        ListIterator iterator = list.begin();
        do {
            pushBack(iterator.getCurrent()->getData());
            ++iterator;
        } while (iterator != ++list.tail());
    }
    ~OneLinkedList() {
        clear();
    }

    size_t getSize() {
        return _size;
    }
    void clear() {
        while (_head != nullptr) {
            popFront();
        }
    }
    bool isEmpty() {
        return _size == 0;
    }
    bool contains(T data) {
        ListIterator iterator = begin();
        do {
            if (iterator.getCurrent()->getData() == data) {
                return true;
            }
            ++iterator;
        } while (iterator != ++tail());
        return false;
    }
    T getById(size_t id) {
        if (id >= _size) {
            throw std::out_of_range("Index is out of range");
        }
        ListIterator iterator = begin();
        while (id != iterator.getCurrent()->getId()) {
            ++iterator;
        }

        return iterator.getCurrent()->getData();
    }
    bool changeById(size_t id, T data) {
        if (id >= _size) {
            return false;
        }
        ListIterator iterator = begin();
        while (id != iterator.getCurrent()->getId()) {
            ++iterator;
        }
        iterator.getCurrent()->setData(data);

        return true;
    }
    int getId(T data) {
        ListIterator iterator = begin();
        do {
            if (iterator.getCurrent()->getData() == data) {
                return static_cast<int>(iterator.getCurrent()->getId());
            }
            ++iterator;
        } while (iterator != ++tail());

        return -1;
    }
    void addToPosition(size_t id, T data) {
        if (id >= _size) {
            throw std::out_of_range("Index is out of range");
            return;
        } else if (id == 0) {
            pushFront(data);
        }
        ListIterator iterator = begin();
        while (id != iterator.getCurrent()->getId()) {
            ++iterator;
        }

        Node<T> *temp = new Node<T>(data);
        temp->setNext(iterator.getCurrent());
        (--iterator).getCurrent()->setNext(temp);
        temp->setId(id);
        ++iterator;
        do {
            ++iterator;
            iterator.getCurrent()->setId(iterator.getCurrent()->getId() + 1);
        } while (iterator != tail());
        _size++;
    }
    void removeFromPosition(size_t id) {
        if (id >= _size) {
            throw std::out_of_range("Index is out of range");
            return;
        }
        if (id == _size - 1) {
            popBack();
            return;
        }
        if (id == 0) {
            popFront();
            return;
        }
        ListIterator iterator = begin();
        while (id != iterator.getCurrent()->getId()) {
            ++iterator;
        }

        Node<T> *temp = iterator.getCurrent();
        (--iterator).getCurrent()->setNext(temp->getNext());
        delete temp;
        _size--;

        ++iterator;
        do {
            iterator.getCurrent()->setId(iterator.getCurrent()->getId() - 1);
            ++iterator;
        } while (++tail() != iterator);
    }
    void removeByValue(T data) {
        bool flag = false;
        ListIterator iterator = begin();
        do {
            if (iterator.getCurrent()->getData() == data) {
                flag = true;
                break;
            }
            ++iterator;
        } while (iterator != ++tail());
        if (!flag) {
            throw std::invalid_argument("Data not found");
        }
        if (iterator.getCurrent()->getId() == 0) {
            popFront();
            return;
        }
        if (iterator.getCurrent()->getId() == _size - 1) {
            popBack();
            return;
        }
        Node<T> *temp = iterator.getCurrent();
        (--iterator).getCurrent()->setNext(temp->getNext());
        delete temp;
        _size--;

        ++iterator;
        do {
            iterator.getCurrent()->setId(iterator.getCurrent()->getId() - 1);
            ++iterator;
        } while (++tail() != iterator);
    }
    ListIterator begin() {
        return ListIterator(_head);
    }
    ListIterator tail() {
        return ListIterator(_tail);
    }
    ListIterator end() {
        return ListIterator(nullptr);
    }
    void pushBack(T data) {
        Node<T> *temp = new Node<T>(data);
        if (isEmpty()) {
            _head = temp;
            _head->setNext(_head);
            _tail = _head;
            _head->setId(0);
            _size++;
            return;
        }

        _tail->setNext(temp);
        size_t new_id = _tail->getId() + 1;
        _tail = _tail->getNext();
        _tail->setId(new_id);
        _size++;
        _tail->setNext(_head);
    }
    void pushFront(T data) {
        Node<T> *temp = new Node<T>(data);
        if (isEmpty()) {
            _head = temp;
            _head->setId(0);
            _size++;
            _head->setNext(_head);
            _tail = _head;
            return;
        }

        temp->setNext(_head);
        _tail->setNext(temp);
        _head = temp;
        ListIterator iterator = begin();
        do {
            ++iterator;
            iterator.getCurrent()->setId(iterator.getCurrent()->getId() + 1);
        } while (iterator != tail());
        _size++;
    }
    void popBack() {
        if (_tail == nullptr) {
            return;
        }
        if (_tail == _head) {
            delete _head;
            _head = _tail = nullptr;
            _size = 0;
            return;
        }
        Node<T> *temp = _tail;
        ListIterator iterator = tail();
        _tail = (--iterator).getCurrent();
        _tail->setNext(_head);
        delete temp;
        --_size;
    }
    void popFront() {
        if (_head == nullptr) {
            return;
        }
        if (_head == _tail) {
            delete _tail;
            _head = _tail = nullptr;
            _size = 0;
            return;
        }
        Node<T> *temp = _head;
        _head = _head->getNext();
        _tail->setNext(_head);
        delete temp;
        ListIterator iterator = begin();
        do {
            iterator.getCurrent()->setId(iterator.getCurrent()->getId() - 1);
            ++iterator;
        } while (iterator != ++tail());
        --_size;
    }
    void print() {
        if (_size == 0) {
            std::cout << "Empty list" << std::endl;
            return;
        }
        ListIterator iterator = begin();
        do {
            std::cout << iterator.getCurrent()->getData() << " ";
            ++iterator;
        } while (iterator != ++tail());

        std::cout << std::endl;
    }
};
