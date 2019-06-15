#pragma once
#include "Array.h"
#include "util/level.h"
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

namespace b_array {
// return the larger of the two items
template <typename T>
T maximal(const T& a, const T& b) {
    return (a > b) ? a : b;
}

// swap the two items
template <typename T>
void swap(T& a, T& b) {
    T t = a;
    a = b;
    a = t;
}
// return index of the largest item in data
template <typename T>
int index_of_maximal(b_array::Array<T>& data, size_t size) {
    int largest = 0;
    for (size_t i = 0; i < size; i++) {
        if (data[i] > data[largest]) {
            largest = i;
        }
    }
    return largest;
}

// insert entry at index i in data
template <typename T>
void insert_item(b_array::Array<T>& data, size_t pos, size_t& size, const T& entry) {
    for (size_t i = size++; i > pos; i--) {
        data[i] = data[i - 1];
    }
    data[pos] = entry;
}

// insert entry into the sorted array data with length n
template <typename T>
void ordered_insert(b_array::Array<T>& data, size_t& size, const T& entry) {
    int pos = 0;
    for (size_t i = 0; i < size; i++) {
        if (entry > data[i]) {
            pos = i + 1;
        }
    }
    insert_item(data, pos, size, entry);
}

// return the first element in data that is not less than entry
template <typename T>
int first_ge(const b_array::Array<T>& data, size_t size, const T& entry) {
    for (size_t i = 0; i < size; i++) {
        if (data[i] >= entry) {
            return i;
        }
    }
    return size;
}

// append entry to the right of data
template <typename T>
void attach_item(b_array::Array<T>& data, size_t& size, const T& entry) {
    data[size++] = entry;
}

// remove the last element in data and place it in entry
template <typename T>
T detach_item(b_array::Array<T>& data, size_t& size) {
    assert(size > 0);
    T item = data[size - 1];
    data[size] = T();
    data[--size] = T();
    return item;
}

// delete item at index i and return it, shifts array
template <typename T>
T delete_item(b_array::Array<T>& data, int index, size_t& size) {
    assert(size > 0);
    T item = data[index];
    for (size_t i = index; i < size - 1; i++) {
        data[i] = data[i + 1];
    }
    data[size] = T();
    data[--size] = T();
    return item;
}

// append data2 to the right of data1
template <typename T>
void merge(b_array::Array<T>& data1, b_array::Array<T>& data2, size_t& size1, size_t& size2) {
    for (size_t i = 0; i < size2; i++) {
        attach_item(data1, size1, data2[i]);
    }
}

// move n / 2 elements from the right of data1 and move to data2
template <typename T>
void split(b_array::Array<T>& data1, size_t& size1, b_array::Array<T>& data2, size_t& size2) {
    size_t size = size1;
    for (size_t i = ((size + 1) / 2); i < size; i++) {
        data2[i - ((size + 1) / 2)] = data1[i];
    }
    size1 = (size + 1) / 2;
    size2 = size - size1;
}

// copy src[] into dest[]
template <typename T>
void copy_array(const b_array::Array<T>& src, b_array::Array<T>& dest, size_t src_size,
                size_t& dest_size) {
    for (size_t i = 0; i < src_size; i++) {
        dest[i] = src[i];
    }
    dest_size = src_size;
}

// print array data
template <typename T>
void print_array(const b_array::Array<T>& data, size_t size, int level = 0,
                 std::ostream& outs = std::cout, size_t spacing_size = 8) {
    outs << level::create_space(level, spacing_size);
    outs << '[' << ' ';
    for (size_t i = 0; i < size; i++) {
        outs << data[i] << " ";
    }
    outs << ']';
}

// template <typename T>
// int binary_search(const b_array::Array<T>& data, size_t size, const T& item) {
//     if (size >= 1)
// }

// item > all data[i]
template <typename T>
bool is_gt(const b_array::Array<T>& data, size_t size, const T& item) {
    for (size_t i = 0; i < size; i++) {
        if (item < data[i] || item == data[i]) {
            return false;
        }
    }
    return true;
}

// item <= all data[i]
template <typename T>
bool is_le(const b_array::Array<T>& data, size_t size, const T& item) {
    for (size_t i = 0; i < size; i++) {
        if (item > data[i]) {
            return false;
        }
    }
    return true;
}

} // namespace b_array