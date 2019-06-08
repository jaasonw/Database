#pragma once
#include <iostream>

namespace Map {

template <typename K, typename V>
struct Pair {
    K key;
    V value;
    Pair(const K& key = K(), const V& value = V()) : key(key), value(value) {}
    void operator+=(const Pair& other);
};

template <typename K, typename V>
std::ostream& operator<<(std::ostream& outs, const Pair<K, V>& pair) {
    outs << "{ " << pair.key << ": " << pair.value << " }";
    return outs;
}

template <typename K, typename V>
bool operator==(const Pair<K, V>& left, const Pair<K, V>& right) {
    return left.key == right.key;
}

template <typename K, typename V>
bool operator!=(const Pair<K, V>& left, const Pair<K, V>& right) {
    return left.key != right.key;
}

template <typename K, typename V>
bool operator<(const Pair<K, V>& left, const Pair<K, V>& right) {
    return left.key < right.key;
}

template <typename K, typename V>
bool operator>(const Pair<K, V>& left, const Pair<K, V>& right) {
    return left.key > right.key;
}

template <typename K, typename V>
bool operator<=(const Pair<K, V>& left, const Pair<K, V>& right) {
    return left.key <= right.key;
}

template <typename K, typename V>
bool operator>=(const Pair<K, V>& left, const Pair<K, V>& right) {
    return left.key >= right.key;
}

template <typename K, typename V>
void Pair<K, V>::operator+=(const Pair&) {
    // value = value + other.value;
}

// not really sure what this does
// template <typename K, typename V>
// Pair<K, V> operator+(const Pair<K, V>& left, const Pair<K, V>& right) {
// }

} // namespace Map