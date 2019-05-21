#pragma once
#include "List/linked_list_functions.h"
#include <cassert>
#include <iostream>

template <typename T>
class Queue {
private:
    Node<T>* _data;
    Node<T>* _data_back;
    int _size;

public:
    class Iterator {
    private:
        Node<T>* _node;

    public:
        friend class Queue;
        Iterator(Node<T>* node = NULL) { this->_node = node; }
        T& operator*() { return _node->_item; }

        friend Iterator operator++(Iterator& it, int unused) {
            Iterator hold;
            hold = it;
            it._node = it._node->_next;
            return hold;
        }
        Iterator& operator++() {
            this->_node = this->_node->_next;
            return *this;
        }
        friend bool operator!=(const Iterator& left, const Iterator& right) {
            return left._node != right._node;
        }
        friend bool operator==(const Iterator& left, const Iterator& right) {
            return left._node == right._node;
        }
    };

    Queue() : _data(NULL), _data_back(NULL), _size(0) {}

    ~Queue();
    Queue(const Queue<T>& other);
    Queue<T>& operator=(const Queue<T>& other);

    Iterator find(const T& item) const;

    void push(const T& item);
    T pop();

    bool empty() const { return _size == 0; };
    int size() const { return _size; };
    void clear();

    T front() { return this->_data->_item; }
    T back() { return this->_data_back->_item; }

    Iterator it_begin() const;
    Iterator it_end() const;

    friend std::ostream& operator<<(std::ostream& outs, const Queue& s) {
        return LinkedList::print_list(s._data, outs);
    }
};

// copy constructor
template <typename T>
Queue<T>::Queue(const Queue<T>& other) {
    this->_data = NULL;
    this->_data_back = LinkedList::copy_list(other._data, this->_data);
    this->_size = other._size;
}

// destructor
template <typename T>
Queue<T>::~Queue() {
    LinkedList::delete_all(this->_data);
}

// assignment operator
template <typename T>
Queue<T>& Queue<T>::operator=(const Queue<T>& other) {
    // self assignment
    if (this == &other)
        return *this;
    // clean up
    LinkedList::delete_all(this->_data);
    LinkedList::init_head(this->_data);
    // copy stuff over
    this->_data_back = LinkedList::copy_list(other._data, this->_data);
    this->_size = other._size;
    // return
    return *this;
}

// pushes an item to the back of the queue
template <typename T>
void Queue<T>::push(const T& item) {
    this->_data_back =
        LinkedList::insert_after(this->_data, this->_data_back, item);
    this->_size++;
}

// pops an item from the front of the queue
template <typename T>
T Queue<T>::pop() {
    if (this->_size > 0) {
        this->_size--;
        return LinkedList::delete_head(this->_data);
    }
    assert("cannot pop from an empty Queue" == "");
    return NULL;
}

// returns an iterator to the front of queue
template <typename T>
typename Queue<T>::Iterator Queue<T>::it_begin() const {
    return Queue<T>::Iterator(this->_data);
}

// returns a null iterator
template <typename T>
typename Queue<T>::Iterator Queue<T>::it_end() const {
    return Queue<T>::Iterator(NULL);
}

// clears the queue
template <typename T>
void Queue<T>::clear() {
    LinkedList::delete_all(this->_data);
    this->_data_back = NULL;
    this->_size = 0;
}