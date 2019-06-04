#pragma once
#include "LinkedList/List.h"
#include <iostream>

template <typename T>
class Stack {
private:
    List<T> _list;
public:
    // pushes an item to the top of the stack
    void push(const T& item) { this->_list.push_front(item); }
    // pops an item off the top of the stack
    T pop() { return this->_list.pop_front(); }
    // returns the item at the top of the stack
    T top() { return this->_list.front(); }
    // returns whether the stack is empty
    bool empty() const { return this->_list.empty(); }
    int size() const { return this->_list.size(); }
    // clears the stack
    void clear() { this->_list.clear(); }

    friend std::ostream& operator<<(std::ostream& outs, const Stack& s) {
        return outs << s._list;
    }
};