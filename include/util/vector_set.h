#pragma once
#include "Map/Map.h"
#include <vector>

// set operations for vectors
namespace vset {

template <typename T>
std::vector<T> set_intersect(const std::vector<T>& v1, const std::vector<T>& v2) {
    std::vector<T> result;
    Map::Map<T, int> map;
    for (T e : v1)
        map[e] = 0x2;
    for (T e : v2)
        map[e] |= 0x1;

    for (auto it = map.begin(); it != nullptr; ++it) {
        if (*it == 0x3)
            result.push_back(it.key());
    }
    return result;
}

template <typename T>
std::vector<T> set_union(const std::vector<T>& v1, const std::vector<T>& v2) {
    std::vector<T> result;
    BPlusTree<T> tree;
    for (T e : v1)
        tree.insert(e);

    for (T e : v2)
        tree.insert(e);

    for (auto it = tree.begin(); it != nullptr; ++it) {
        result.push_back(*it);
    }
    return result;
}

} // namespace vset