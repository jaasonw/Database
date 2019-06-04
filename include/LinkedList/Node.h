#pragma once
#include <iostream>

namespace LinkedList {

template <typename T>
struct Node {
    T _item;
    Node<T>* _next;
    Node(const T& item = T(), Node<T>* next = NULL)
        : _item(item), _next(next) {}
    friend std::ostream& operator<<(std::ostream& outs, const Node<T>& print) {
        outs << "[" << print._item << "]->";
        return outs;
    }
};

} // namespace LinkedList