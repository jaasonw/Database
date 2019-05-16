#pragma once
#include <iostream>
#include <vector>

// print vector list
template <typename T>
std::ostream& operator<<(std::ostream& outs, const std::vector<T>& list) {
    b_array::print_array(&list[0], list.size(), 0, outs);
    outs << '[' << ' ';
    for (size_t i = 0; i < list.size(); i++) {
        outs << list[i] << " ";
    }
    outs << ']';
    return outs;
}

// list.push_back item
template <typename T>
std::vector<T>& operator+=(std::vector<T>& list, const T& item) {
    list.push_back(item);
}