#pragma once
#include "LinkedList/linked_list_functions.h"
#include <cassert>
#include <iostream>

template <typename T>
class List {
private:
    LinkedList::Node<T>* _data;
    LinkedList::Node<T>* _data_back;
    int _size;

public:
    class Iterator {
    private:
        LinkedList::Node<T>* _node;
    public:
        friend class List;
        Iterator(LinkedList::Node<T>* node = NULL) { this->_node = node; }
        T& operator*() { return _node->_item; }

        friend Iterator operator++(Iterator& it, int) {
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

    List(): _data(NULL), _data_back(NULL), _size(0) {}

    ~List();
    List(const List<T>& other);
    List<T>& operator=(const List<T>& other);

    Iterator find(const T& item) const;

    void push_front(const T& item);
    void push_back(const T& item);
    T pop_front();

    void insert(int pos, const T& item);
    void insert_after(Iterator pos, const T& item);
    void insert_after(LinkedList::Node<T>* pos, const T& item);

    bool empty() const { return _size == 0; };
    int size() const { return _size; };
    void clear();

    T front() { return this->_data->_item; }
    T back() { return this->_data_back->_item; }

    Iterator it_begin() const;
    Iterator it_end() const;

    friend std::ostream& operator<<(std::ostream& outs, const List& s) {
        for (auto it = s.it_begin(); it != s.it_end(); it++) {
            outs << *it << " ";
        }
        return outs;
    }
};

// copy constructor
template <typename T>
List<T>::List(const List<T>& other) {
    this->_data = NULL;
    this->_data_back = LinkedList::copy_list(other._data, this->_data);
    this->_size = other._size;
}

// destructor
template <typename T>
List<T>::~List() {
    LinkedList::delete_all(this->_data);
}

// assignment operator
template <typename T>
List<T>& List<T>::operator=(const List<T>& other) {
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

// insert item at front of list
template <typename T>
void List<T>::push_front(const T& item) {
    LinkedList::insert_head(this->_data, item);
    if (this->_data_back == NULL)
        this->_data_back = this->_data;
    this->_size++;
}

// insert item at back of list
template <typename T>
void List<T>::push_back(const T& item) {
    this->_data_back = LinkedList::insert_after(this->_data, this->_data_back, item);
    this->_size++;
}

// removes and returns the item at the front of the list
template <typename T>
T List<T>::pop_front() {
    if (this->_size > 0) {
        this->_size--;
        return LinkedList::delete_head(this->_data);
    }
    assert(false && "cannot pop from an empty list");
    return T();
}

// returns an iterator to the 1st occurance of item
template <typename T>
typename List<T>::Iterator List<T>::find(const T& item) const {
    return List<T>::Iterator(LinkedList::find(this->_data, item));
}

// returns an iterator to the beginning of the list
template <typename T>
typename List<T>::Iterator List<T>::it_begin() const {
    return List<T>::Iterator(this->_data);
}

// returns a null iterator
template <typename T>
typename List<T>::Iterator List<T>::it_end() const {
    return List<T>::Iterator(NULL);
}

// inserts an item at index
template <typename T>
void List<T>::insert(int pos, const T& item) {
    // insert at beginning edge case
    if (pos == 0) {
        this->push_front(item);
        return;
    }
    LinkedList::Node<T>*walk = this->_data;
    for (int i = 0; i < pos; i++, walk = walk->_next) {
        if (walk == NULL && i != pos - 1) {
            assert(false && "inserted element at invalid position");
        }
        // insert at end edge case
        if (walk == NULL && i == pos - 1) {
            this->push_back(item);
        }
        else {
            if (i == pos - 1) {
                LinkedList::insert_after(this->_data, walk, item);
            }
        }
    }
}

// insert an item after an interator
template <typename T>
void List<T>::insert_after(Iterator pos, const T& item) {
    this->insert_after(pos._node, item);
}

// insert an item after a node
template <typename T>
void List<T>::insert_after(LinkedList::Node<T>* pos, const T& item) {
    LinkedList::insert_after(this->_data, pos, item);
}

// clears the list
template <typename T>
void List<T>::clear() {
    LinkedList::delete_all(this->_data);
    this->_data_back = NULL;
    this->_size = 0;
}