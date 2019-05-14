#pragma once
#include "fake_array/FakeArray.h"
#include "fake_array/fake_array_util.h"
// #include "array_util.h"
#include <cassert>
#include <exception>
#include <iostream>
#include <string>

template <typename T>
class BPlusTree {
private:
    static const int MINIMUM = 1;
    static const int MAXIMUM = 2 * MINIMUM;

    // number of data elements
    size_t data_size = 0;

    // number of children
    size_t subset_size = 0;

    // max data capacity
    static const size_t DATA_CAPACITY = MAXIMUM + 1;
    // max subset capacity
    static const size_t SUBSET_CAPACITY = MAXIMUM + 2;

    // holds the keys
    FakeArray<T> data = FakeArray<T>(DATA_CAPACITY);

    // subtrees
    FakeArray<BPlusTree*> subset = FakeArray<BPlusTree*>(SUBSET_CAPACITY);

    // // holds the keys
    // T data[DATA_CAPACITY] = {T()};
    // // subtrees
    // BPlusTree* subset[SUBSET_CAPACITY] = {nullptr};
    
    
    // true if duplicate keys may be inserted
    bool duplicates_allowed;

    // true if this is a leaf node
    bool is_leaf() const { return subset_size == 0; }

    // insert element functions
    // allows MAXIMUM+1 data elements in the root
    void loose_insert(const T& entry);
    // fix excess of data elements in child i
    void fix_excess(int i);

    // remove element functions:
    // allows MINIMUM-1 data elements in the root
    void loose_remove(const T& entry);
    // fix shortage of data elements in child i
    void fix_shortage(int i);

    // remove the biggest child of this tree->entry
    T remove_biggest();
    // transfer one element LEFT from child i
    void rotate_left(int i);
    // transfer one element RIGHT from child i
    void rotate_right(int i);
    // merge subset i with subset i+1
    void merge_with_next_subset(int i);
    // zeros out the node
    void erase_node();

public:
    BPlusTree(bool duplicates_allowed = false);
    BPlusTree(const BPlusTree<T>& other);
    ~BPlusTree();
    BPlusTree<T>& operator=(const BPlusTree<T>& other);

    // insert entry into the tree
    void insert(const T& entry);
    // remove entry from the tree
    void remove(const T& entry);

    // clear this object (delete all nodes etc.)
    void clear_tree();
    // copy other into this object
    void copy_tree(const BPlusTree<T>& other);

    // true if entry can be found in the array
    bool contains(const T& entry) const;
    // return a const reference to entry in the tree
    const T& get(const T& entry) const;
    // return a reference to entry in the tree
    T& get(const T& entry);
    // return a pointer to this key. NULL if not there.
    T* find(const T& entry);

    // count the number of elements in the tree
    int size() const;
    // true if the tree is empty
    bool empty() const;

    //print a readable version of the tree
    void print_tree(std::ostream& outs = std::cout, int level = 0) const;

    friend std::ostream& operator<<(std::ostream& outs, const BPlusTree<T>& tree){
        tree.print_tree(outs);
        return outs;
    }
};

template <typename T>
BPlusTree<T>::BPlusTree(bool dupes_allowed) : duplicates_allowed(dupes_allowed) {}

template <typename T>
BPlusTree<T>::BPlusTree(const BPlusTree<T>& other) {
    copy_tree(other);
}

template <typename T>
BPlusTree<T>& BPlusTree<T>::operator=(const BPlusTree<T>& other) {
    if (this == &other)
        return *this;
    clear_tree();
    copy_tree(other);
    return *this;
}

template<typename T>
BPlusTree<T>::~BPlusTree() {
    clear_tree();
}

template <typename T>
void BPlusTree<T>::clear_tree() {
    for (size_t i = 0; i < subset_size; i++) {
        if (subset[i] != nullptr) {
            subset[i]->clear_tree();
            delete subset[i];
            subset[i] = nullptr;
        }
    }
    erase_node();
}

template <typename T>
void BPlusTree<T>::copy_tree(const BPlusTree<T>& other) {
    // copy data
    f_array::copy_array(other.data, data, other.data_size, data_size);
    duplicates_allowed = other.duplicates_allowed;
    subset_size = other.subset_size;
    for (size_t i = 0; i < other.subset_size; i++) {
        if (other.subset[i] != nullptr) {
            subset[i] = new BPlusTree<T>(other.duplicates_allowed);
            subset[i]->copy_tree(*(other.subset[i]));
        }
    }
}

template <typename T>
void BPlusTree<T>::print_tree(std::ostream& outs, int level) const {\
    // if not leaf keep recursioning down until it is
    if (subset_size > 1) {
        // print half of the subset backwards
        for (size_t i = subset_size - 1; i >= subset_size / 2; i--) {
            if (subset[i] != nullptr)
                subset[i]->print_tree(outs, level + 1);
            else {
                f_array::print_array(data, 0, level + 1, outs);
                outs << std::endl;
            }
        }
    }

    // print the parent dataset
    f_array::print_array(data, data_size, level, outs);
    outs << std::endl;

    if (subset_size > 1) {
        // print the other half
        for (int i = (subset_size / 2) - 1; i >= 0; i--) {
            if (subset[i] != nullptr)
                subset[i]->print_tree(outs, level + 1);
            else {
                f_array::print_array(data, 0, level + 1, outs);
                outs << std::endl;
            }
        }
    }
}
template <typename T>
void BPlusTree<T>::loose_insert(const T& entry) {
    // check if thing is already here
    T* item = find(entry);
    // assuming the duplicates allowed flag is for multimap to call +=
    if (item != nullptr) {
        if (!duplicates_allowed)
            *item = entry;
        else
            *item += entry;
        return;
    }
    
    // it's not here, put it somewhere
    int index = f_array::first_ge(data, data_size, entry);
    if (!is_leaf() && data[index] != entry)
        subset[index]->loose_insert(entry);
    else {
        if (data[index] != entry)
            f_array::insert_item(data, index, data_size, entry);
        return;
    }
    if (subset[index]->data_size > MAXIMUM) {
        fix_excess(index);
    }
}
template <typename T>
void BPlusTree<T>::insert(const T& entry) {
    // the function that called this has to be root
    loose_insert(entry);
    // so do root checks here
    if (data_size > MAXIMUM) {
        // create new temp node
        BPlusTree<T>* temp = new BPlusTree<T>(duplicates_allowed);
        // copy data to temp node
        f_array::copy_array(data, temp->data, data_size, temp->data_size);
        f_array::copy_array(subset, temp->subset, subset_size, temp->subset_size);
        // wipe root data
        erase_node();
        // point root to temp
        subset[0] = temp;
        subset_size = 1;
        // call fix_excess
        fix_excess(0);
    }
}

template <typename T>
void BPlusTree<T>::fix_excess(int index) {
    BPlusTree<T>* node = subset[index];
    // dont do anything, why are we even here to begin with?
    if (node->data_size <= MAXIMUM)
        return;
    BPlusTree<T>* split = new BPlusTree<T>(duplicates_allowed);
    // split data and child
    f_array::split(node->data, node->data_size, split->data, split->data_size);
    f_array::split(node->subset, node->subset_size, split->subset,
                 split->subset_size);
    // insert the last item into self
    T item = f_array::detach_item(node->data, node->data_size);
    f_array::ordered_insert(data, data_size, item);
    f_array::insert_item(subset, index + 1, subset_size, split); 
}

template <typename T>
const T& BPlusTree<T>::get(const T& entry) const {
    int index = f_array::first_ge(data, data_size, entry);
    if (data[index] == entry)
        return data[index];
    if (data[index] != entry && subset[index] == nullptr)
        throw std::out_of_range("Item not in tree");
    else {
        return subset[index]->get(entry);
    }
}

template <typename T>
T& BPlusTree<T>::get(const T& entry) {
    T* item = find(entry);
    if (item == nullptr)
        insert(entry);
    return *find(entry);
}

template <typename T>
T* BPlusTree<T>::find(const T& entry) {
    size_t index = f_array::first_ge(data, data_size, entry);

    size_t found = (index < data_size && data[index] == entry);

    // Returns the pointer to the item
    if (found)
        return &data[index];

    // Returns a null pointer if it cant be found
    if (is_leaf() || subset[index] == nullptr) {
        return nullptr;
    }
    if (!found) // yet...
        return subset[index]->find(entry);
    return nullptr;
}
template <typename T>
bool BPlusTree<T>::contains(const T& entry) const {
    return data[f_array::first_ge(data, data_size, entry)] == entry;
}
template <typename T>
void BPlusTree<T>::rotate_left(int i) {
    if (subset[i] == nullptr) {
        subset[i] = new BPlusTree<T>(duplicates_allowed);
        subset_size++;
    }
    f_array::attach_item(subset[i]->data, subset[i]->data_size, data[i]);
    
    // guard rails to make sure im not rotating the wrong way
    assert(subset[i + 1] != nullptr);
    data[i] = f_array::delete_item(subset[i + 1]->data, 0, subset[i + 1]->data_size);

    // rotate subset pointers, if there actually are any pointers to rotate
    if (subset[i + 1]->subset_size > 0) {
        if (subset[i + 1]->subset[0] != nullptr) {
            f_array::attach_item(
                subset[i]->subset,
                subset[i]->subset_size,
                f_array::delete_item(subset[i + 1]->subset, 0, subset[i + 1]->subset_size)
            );

        }
    }
}

template <typename T>
void BPlusTree<T>::rotate_right(int i) {
    if (subset[i + 1] == nullptr) {
        subset[i + 1] = new BPlusTree<T>(duplicates_allowed);
        subset_size++;
    }
    f_array::insert_item(subset[i + 1]->data, 0, subset[i + 1]->data_size, data[i]);

    assert(subset[i] != nullptr);
    data[i] = f_array::detach_item(subset[i]->data, subset[i]->data_size);

    // rotate subset pointers, if there actually are any pointers to rotate
    if (subset[i]->subset_size > 0) {
        if (subset[i]->subset[subset[i]->subset_size - 1] != nullptr) {
            f_array::insert_item(
                subset[i + 1]->subset,
                0,
                subset[i + 1]->subset_size,
                f_array::detach_item(subset[i]->subset, subset[i]->subset_size)
            );
        }
    }
}

template <typename T>
void BPlusTree<T>::erase_node() {
    data_size = 0;
    
    subset_size = 0;
    // null out the subset array before it causes problems
    for (size_t i = 0; i < SUBSET_CAPACITY; i++) {
        if (i < DATA_CAPACITY)
            data[i] = T();
        subset[i] = nullptr;
    }
}

template <typename T>
void BPlusTree<T>::merge_with_next_subset(int i) {
    assert(i + 1 < subset_size);
    f_array::merge(subset[i]->data, subset[i + 1]->data, subset[i]->data_size,
                 subset[i + 1]->data_size);
    f_array::merge(subset[i]->subset, subset[i + 1]->subset,
                 subset[i]->subset_size, subset[i + 1]->subset_size);
    // make sure we arent deleting its children too
    subset[i + 1]->erase_node();
    delete subset[i + 1];
    f_array::delete_item(subset, i + 1, subset_size);
}

template <typename T>
void BPlusTree<T>::remove(const T& entry) {
    // the function that called this has to be root
    loose_remove(entry);
    if (data_size < MINIMUM && subset_size > 0) {
        auto shrink_ptr = subset[0];
        subset[0] = nullptr;
        f_array::copy_array(shrink_ptr->data, data, shrink_ptr->data_size, data_size);
        f_array::copy_array(shrink_ptr->subset, subset, shrink_ptr->subset_size, subset_size);

        shrink_ptr->erase_node();
        delete shrink_ptr;
    }
}

template <typename T>
void BPlusTree<T>::loose_remove(const T& entry) {
    size_t index = f_array::first_ge(data, data_size, entry);
    if (!is_leaf()) {
        if (index < data_size && data[index] == entry) {
            // replace item with largest child on the left
            data[index] = subset[index]->remove_biggest();
            fix_shortage(index);
        } else {
            subset[index]->loose_remove(entry);
            if (index == data_size)
                index--;
            fix_shortage(index);
        }
    } else {
        if (data[index] == entry) {
            f_array::delete_item(data, index, data_size);
            return;
        } else {
            throw std::out_of_range("Item not in tree");
        }
    }
    for (size_t i = 0; i < data_size; i++) {
        fix_shortage(i);
    }
    
}

template <typename T>
T BPlusTree<T>::remove_biggest() {
    if (is_leaf()) {
        return f_array::detach_item(data, data_size);
    }
    T item = subset[subset_size - 1]->remove_biggest();
    // make sure to get rid of the node we just pulled from it if it's empty
    if (subset[subset_size - 1]->data_size <= 0) {
        // delete subset[subset_size - 1];
        // subset[subset_size - 1] = nullptr;
        delete f_array::detach_item(subset, subset_size);
    }
    fix_shortage(data_size - 1);
    return item;
}

template <typename T>
void BPlusTree<T>::fix_shortage(int index) {
    BPlusTree<T>* left = subset[index];
    BPlusTree<T>* right = subset[index + 1];
    
    // dont do anything if we're at a leaf, why are we even at a leaf?
    if (is_leaf())
        return;

    // dont do anything, we dont have a shortage
    if (left != nullptr && right != nullptr) {
        if (left->data_size >= MINIMUM && right->data_size >= MINIMUM)
            return;
    }

    // case 1: we have a sibling to borrow from (left)
    if (left != nullptr && left->data_size > MINIMUM &&
        (right == nullptr || right->data_size <= MINIMUM)) {
            rotate_right(index);
    }
    // case 1.5: we have a sibling to borrow from (right)
    else if (right != nullptr && right->data_size > MINIMUM &&
             (left == nullptr || left->data_size <= MINIMUM)) {
        rotate_left(index);
    }
    // case 2: we dont have siblings to borrow from
    else {
        // move the root down to right
        T item = f_array::delete_item(data, index, data_size);
        if (right == nullptr) {
            auto temp = new BPlusTree<T>(duplicates_allowed);
            f_array::insert_item(subset, index + 1, subset_size, temp);
            right = subset[index + 1];
        }
        f_array::ordered_insert(right->data, right->data_size, item);
        // merge them
        merge_with_next_subset(index);
    }
}