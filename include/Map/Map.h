#pragma once
#include "BPlusTree/BPlusTree.h"
#include "Map/Pair.h"

namespace Map {

template <typename Key, typename T>
class Map {
private:
    BPlusTree<Pair<Key, T>> tree;
public:
    class Iterator {
    private:
        typename BPlusTree<Pair<Key, T>>::Iterator tree_iter;

    public:
        friend class Map;

        Iterator(typename BPlusTree<Pair<Key, T>>::Iterator it)
            : tree_iter(it) {}

        T operator*() { return tree_iter->value; }
        T* operator->() { return &tree_iter.operator->()->value; }

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
            return left.tree_iter != right.tree_iter;
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

    Map();
    // access
    T get(const Key& key) const;
    T& at(const Key& key);
    T& operator[](const Key& key);
    const T& operator[](const Key& key) const;
    const T& at(const Key& key) const;

    //  Modifiers
    void set(const Key& key, const T& value);
    void erase(const Key& key);
    void clear() { tree.clear_tree(); }

    // Iterators
    Iterator begin() const;
    Iterator end() const;
    Iterator first_ge(const Key& entry) const;

    // operations
    bool contains(const Key& key, const T& value);
    bool contains(const Key& key);

    friend std::ostream& operator<<(std::ostream& outs, const Map<Key,T>& map) {
        map.tree.print_as_list();
        return outs;
    }
};

template <typename Key, typename T>
Map<Key, T>::Map() {}

template <typename Key, typename T>
void Map<Key, T>::set(const Key& key, const T& value) {
    tree.insert(Pair<Key, T>(key, value));
}

template <typename Key, typename T>
T& Map<Key, T>::operator[](const Key& key) {
    if (tree.search(Pair<Key, T>(key, T())) == nullptr)
        tree.insert(Pair<Key, T>(key, T()));
    return tree.search(Pair<Key, T>(key, T()))->value;
}

template <typename Key, typename T>
const T& Map<Key, T>::operator[](const Key& key) const {
    return tree.search(Pair<Key, T>(key, T()))->value;
}

template <typename Key, typename T>
T& Map<Key, T>::at(const Key& key) {
    return tree.search(Pair<Key, T>(key, T()))->value;
}

template <typename Key, typename T>
T Map<Key, T>::get(const Key& key) const {
    return tree.search(Pair<Key, T>(key, T()))->value;
}

template <typename Key, typename T>
const T& Map<Key, T>::at(const Key& key) const {
    try {
        return tree.search(key);
    } catch (std::out_of_range()) {
        throw std::out_of_range("Key not found");
    }
}

template <typename Key, typename T>
void Map<Key, T>::erase(const Key& key) {
    tree.remove(key);
}
template <typename Key, typename T>
bool Map<Key, T>::contains(const Key& key, const T& value) {
    auto search = tree.search(key);
    return search != nullptr && search->value == value;
}

template <typename Key, typename T>
bool Map<Key, T>::contains(const Key& key) {
    auto search = tree.search(key);
    return search != nullptr;
}

template <typename Key, typename T>
typename Map<Key, T>::Iterator Map<Key, T>::begin() const {
    return Iterator(tree.begin());
}

template <typename Key, typename T>
typename Map<Key, T>::Iterator Map<Key, T>::end() const {
    return Iterator(tree.end());
}

template <typename Key, typename T>
typename Map<Key, T>::Iterator Map<Key, T>::first_ge(const Key& entry) const {
    return Iterator(tree.first_ge(entry));
}

} // namespace Map
