#pragma once
#include "BPlusTree/BPlusTree.h"
#include "MultiMap/MultiPair.h"

namespace MultiMap {

template <typename Key, typename T>
class MultiMap {
private:
    BPlusTree<Pair<Key, T>> tree;
public:
    class Iterator {
    private:
        typename BPlusTree<Pair<Key, T>>::Iterator tree_iter;

    public:
        friend class Multimap;

        Iterator(typename BPlusTree<Pair<Key, T>>::Iterator it)
            : tree_iter(it) {}

        std::vector<T> operator*() { return tree_iter->values; }
        std::vector<T>* operator->() { return &tree_iter.operator->()->values; }

        // i++
        Iterator operator++(int) {
            Iterator temp = tree_iter;
            tree_iter++;
            return temp;
        }

        // ++i
        Iterator operator++() {
            tree_iter++;
            return *this;
        }
        // compare to self
        friend bool operator==(const Iterator& left, const Iterator& right) {
            return left.tree_iter == right.tree_iter;
        }
        friend bool operator!=(const Iterator& left, const Iterator& right) {
            return left.tree_iter != right.iter;
        }
        // compare to items
        friend bool operator==(const Iterator& left, const T& right) {
            return *(left.tree_iter) == right;
        }
        friend bool operator!=(const Iterator& left, const T& right) {
            return *(left.tree_iter) != right;
        }
        // compare to null
        friend bool operator==(const Iterator& left, std::nullptr_t) {
            return left.tree_iter == nullptr;
        }
        friend bool operator!=(const Iterator& left, std::nullptr_t) {
            return left.tree_iter != nullptr;
        }
        bool is_null() { return !tree_iter; }

        Key key() { return tree_iter->key; }
    };

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

    // Iterators
    Iterator begin() const;
    Iterator end() const;
    Iterator first_ge(const Key& entry) const;

    // operations
    bool contains(const Key& key);
    std::vector<T>& get(const Key& key);

    friend std::ostream& operator<<(std::ostream& outs,
                                    const MultiMap<Key, T>& map) {
        map.tree.print_as_list();
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
    return at(key);
}

template <typename Key, typename T>
const std::vector<T>& MultiMap<Key, T>::operator[](const Key& key) const {
    try {
        return tree.search(key);
    } catch (std::out_of_range()) {
        throw std::out_of_range("Key not found");
    }
}
template <typename Key, typename T>
std::vector<T>& MultiMap<Key, T>::at(const Key& key) {
    if (tree.search(Pair<Key, T>(key, T())) == nullptr)
        tree.insert(Pair<Key, T>(key));
    return tree.search(Pair<Key, T>(key, T()))->values;
}

template <typename Key, typename T>
const std::vector<T>& MultiMap<Key, T>::at(const Key& key) const {
    try {
        return tree.search(key);
    } catch (std::out_of_range()) {
        throw std::out_of_range("Key not found");
    }
}
template <typename Key, typename T>
bool MultiMap<Key, T>::contains(const Key& key) {
    return tree.search(Pair<Key, T>(key)) != nullptr;
}

template <typename Key, typename T>
std::vector<T>& MultiMap<Key, T>::get(const Key& key) {
    return tree.search(Pair<Key, T>(key, T()))->values;
}

template <typename Key, typename T>
void MultiMap<Key, T>::erase(const Key& key) {
    tree.remove(key);
}

template <typename Key, typename T>
typename MultiMap<Key, T>::Iterator MultiMap<Key, T>::begin() const {
    return Iterator(tree.begin());
}

template <typename Key, typename T>
typename MultiMap<Key, T>::Iterator MultiMap<Key, T>::end() const {
    return Iterator(tree.end());
}

template <typename Key, typename T>
typename MultiMap<Key, T>::Iterator MultiMap<Key, T>::first_ge(const Key& entry) const {
    return Iterator(tree.first_ge(entry));
}

} // namespace MultiMap
