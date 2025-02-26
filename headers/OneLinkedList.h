#pragma once

#include "../headers/ListIterator.h"
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
    OneLinkedList() {
        _size = 0;
        _head = nullptr;
        _tail = nullptr;
    }
    OneLinkedList(OneLinkedList<T> &list) {
        if (_head != nullptr) {
            clear();
        }
        _size = 0;
        ListIterator<T> iterator = list.begin();
        do {
            pushBack((*iterator)->getData());
            ++iterator;
        } while (iterator != ++list.end());
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
        ListIterator<T> iterator = begin();
        do {
            if ((*iterator)->getData() == data) {
                return true;
            }
            ++iterator;
        } while (iterator != ++end());
        return false;
    }
    T getById(size_t id) {
        if (id >= _size) {
            return NULL;
        }
        ListIterator<T> iterator = begin();
        while (id != (*iterator)->getId()) {
            ++iterator;
        }

        return (*iterator)->getData();
    }
    void changeById(size_t id, T data) {
        if (id >= _size) {
            return;
        }
        ListIterator<T> iterator = begin();
        while (id != (*iterator)->getId()) {
            ++iterator;
        }
        (*iterator)->setData(data);
    }
    size_t getId(T data) {
        ListIterator<T> iterator = begin();
        do {
            if ((*iterator)->getData() == data) {
                return (*iterator)->getId();
            }
            ++iterator;
        } while (iterator != ++end());

        return NULL;
    }
    void addToPosition(size_t id, T data) {
        if (id >= _size) {
            return;
        }
        ListIterator<T> iterator = begin();
        while (id != (*iterator)->getId()) {
            ++iterator;
        }

        Node<T> *temp = new Node<T>(data);
        temp->setNext(*iterator);
        (*--iterator)->setNext(temp);
        temp->setId(id);
        ++iterator;
        do {
            ++iterator;
            (*iterator)->setId((*iterator)->getId() + 1);
        } while (iterator != end());
        _size++;
    }
    void removeFromPosition(size_t id) {
        if (id >= _size) {
            return;
        }
        ListIterator<T> iterator = begin();
        while (id != (*iterator)->getId()) {
            ++iterator;
        }

        Node<T> *temp = *iterator;
        (*--iterator)->setNext(temp->getNext());
        delete temp;
        _size--;

        ++iterator;
        do {
            (*iterator)->setId((*iterator)->getId() - 1);
            ++iterator;
        } while (++end() != iterator);
    }
    void removeByValue(T data) {
        bool flag = false;
        ListIterator<T> iterator = begin();
        do {
            if ((*iterator)->getData() == data) {
                flag = true;
                break;
            }
            ++iterator;
        } while (iterator != ++end());
        if (!flag) {
            return;
        }

        Node<T> *temp = *iterator;
        (*--iterator)->setNext(temp->getNext());
        delete temp;
        _size--;

        ++iterator;
        do {
            (*iterator)->setId((*iterator)->getId() - 1);
            ++iterator;
        } while (++end() != iterator);
    }
    ListIterator<T> begin() {
        return ListIterator<T>(_head);
    }
    ListIterator<T> end() {
        return ListIterator<T>(_tail);
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
        ListIterator<T> iterator = begin();
        do {
            ++iterator;
            (*iterator)->setId((*iterator)->getId() + 1);
        } while (iterator != end());
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
        ListIterator<T> iterator = end();
        _tail = *(--iterator);
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
        ListIterator<T> iterator = begin();
        do {
            (*iterator)->setId((*iterator)->getId() - 1);
            ++iterator;
        } while (iterator != ++end());
        --_size;
    }
    void print() {
        if (_size == 0) {
            std::cout << "Empty list" << std::endl;
            return;
        }
        ListIterator<T> iterator = begin();
        do {
            std::cout << "id: " << iterator.operator*()->getId() << " data: " << iterator.operator*()->getData() << std::endl;
            ++iterator;
        } while (iterator != ++end());
    }
};
