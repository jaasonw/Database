#pragma once
#include "BTree.h"
#include "MultiPair.h"

namespace MultiMap {

template <typename Key, typename T>
class MultiMap {
private:
    BTree<Pair<Key, T>> tree;
public:
    MultiMap();

    // access
    std::vector<T>& at(const Key& key);
    const std::vector<T>& at(const Key& key) const;
    std::vector<T>& operator[](const Key& key);
    const std::vector<T>& operator[](const Key& key) const;

    // modifiers
    void insert(const Key& key, const T& value);
    void erase(const Key& key);
    void clear() { tree.clear_tree(); }

    // operations
    bool contains(const Key& key);
    std::vector<T>& get(const Key& key);

    friend std::ostream& operator<<(std::ostream& outs,
                                    const MultiMap<Key, T>& map) {
        map.tree.print_tree();
        return outs;
    }
};

template <typename Key, typename T>
MultiMap<Key, T>::MultiMap() : tree(true) {}

template <typename Key, typename T>
void MultiMap<Key, T>::insert(const Key& key, const T& value) {
    tree.insert(Pair<Key, T>(key, value));
}
template <typename Key, typename T>
std::vector<T>& MultiMap<Key, T>::operator[](const Key& key) {
    return tree.get(Pair<Key, T>(key, T())).values;
}

template <typename Key, typename T>
const std::vector<T>& MultiMap<Key, T>::operator[](const Key& key) const {
    try {
        return tree.get(key);
    } catch (std::out_of_range()) {
        throw std::out_of_range("Key not found");
    }
}
template <typename Key, typename T>
std::vector<T>& MultiMap<Key, T>::at(const Key& key) {
    return tree.get(Pair<Key, T>(key, T())).values;
}

template <typename Key, typename T>
const std::vector<T>& MultiMap<Key, T>::at(const Key& key) const {
    try {
        return tree.get(key);
    } catch (std::out_of_range()) {
        throw std::out_of_range("Key not found");
    }
}
template <typename Key, typename T>
bool MultiMap<Key, T>::contains(const Key& key) {
    return tree.find(Pair<Key, T>(key)) != nullptr;
}

template <typename Key, typename T>
std::vector<T>& MultiMap<Key, T>::get(const Key& key) {
    return tree.get(Pair<Key, T>(key, T())).values;
}

template <typename Key, typename T>
void MultiMap<Key, T>::erase(const Key& key) {
    tree.remove(key);
}

} // namespace MultiMap
