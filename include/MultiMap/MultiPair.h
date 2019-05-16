#pragma once
#include "util/vector_util.h"
#include <iostream>
#include <vector>

namespace MultiMap {

template <typename Key, typename T>
struct Pair {
    Key key;
    std::vector<T> values;
    Pair(const Key& key = Key()) : key(key) {}
    Pair(const Key& key, const T& value) : key(key) {
        values.push_back(value);
    }
    Pair(const Key& key, const std::vector<T>& values) : key(key) {
        this->values = values;
    }
    void operator+=(const T& item) { values.push_back(item); }
    void operator+=(const Pair& other) {
        for (size_t i = 0; i < other.values.size(); i++) {
            values.push_back(other.values.at(i));
        }
    }
};

template <typename Key, typename T>
std::ostream& operator<<(std::ostream& outs, const Pair<Key, T>& pair) {
    outs << "{ " << pair.key << ": ";
    b_array::array::print_array(&pair.values[0], pair.values.size());
    outs << " }";
    return outs;
}

template <typename Key, typename T>
bool operator==(const Pair<Key, T>& left, const Pair<Key, T>& right) {
    return left.key == right.key;
}

template <typename Key, typename T>
bool operator!=(const Pair<Key, T>& left, const Pair<Key, T>& right) {
    return left.key != right.key;
}

template <typename Key, typename T>
bool operator<(const Pair<Key, T>& left, const Pair<Key, T>& right) {
    return left.key < right.key;
}

template <typename Key, typename T>
bool operator>(const Pair<Key, T>& left, const Pair<Key, T>& right) {
    return left.key > right.key;
}

template <typename Key, typename T>
bool operator<=(const Pair<Key, T>& left, const Pair<Key, T>& right) {
    return left.key <= right.key;
}

template <typename Key, typename T>
bool operator>=(const Pair<Key, T>& left, const Pair<Key, T>& right) {
    return left.key >= right.key;
}

// not really sure what this does
// template <typename Key, typename T>
// Pair<Key, T> operator+(const Pair<Key, T>& left,
//                           const Pair<Key, T>& right) {
// }

} // namespace MultiMap