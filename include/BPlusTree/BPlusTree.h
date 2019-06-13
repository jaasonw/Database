#pragma once
#include "util/array/Array.h"
#include "util/array/array_util.h"
#include <cassert>
#include <exception>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

template <typename T>
class BPlusTree {
private:
    // keep a log of inputs and outputs used to recreate a tree
    std::vector<std::string> event_log;

    static const int MAXIMUM = 2;
    static const int MINIMUM = MAXIMUM / 2;

    // number of data elements
    size_t data_size = 0;

    // number of children
    size_t subset_size = 0;

    // max data capacity
    static const size_t DATA_CAPACITY = MAXIMUM + 1;
    // max subset capacity
    static const size_t SUBSET_CAPACITY = MAXIMUM + 2;

    // holds the keys
    b_array::Array<T> data = b_array::Array<T>(DATA_CAPACITY);

    // subtrees
    b_array::Array<BPlusTree*> subset = b_array::Array<BPlusTree*>(SUBSET_CAPACITY);
    
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
    void loose_remove(const T& entry, BPlusTree<T>* origin = nullptr, int offset = 0);
    // fix shortage of data elements in child i
    void fix_shortage(int i);

    // remove the biggest child of this tree->entry
    T remove_biggest();

    // MIGHT NOT NEED THESE FOR B+TREE
    // transfer one element LEFT from child i
    void rotate_left(int i);
    // transfer one element RIGHT from child i
    void rotate_right(int i);
    // merge subset i with subset i+1
    void merge_with_next_subset(size_t i);
    // zeros out the node
    void erase_node();

    // we do need these though
    // transfer beginning of left subset to the end of right subset
    void transfer_left(size_t i);
    // transfer the end of the left subset to the beginning of the right
    void transfer_right(int i);

    // return a const reference to entry in the tree
    const T& get(const T& entry) const;
    // return a reference to entry in the tree
    T& get(const T& entry);
    // return a pointer to this key. NULL if not there.
    T* find(const T& entry);

    BPlusTree<T>* link_leaves(BPlusTree<T>* previous = nullptr);

    BPlusTree<T>* next = nullptr;
public:

    class Iterator {
    private:
        BPlusTree<T>* node_ptr;
        size_t offset;
    public:
        friend class BPlusTree;

        Iterator(BPlusTree<T>* ptr = nullptr, int offset = 0): node_ptr(ptr), 
                                                               offset(offset) {}

        T operator*() {
            if (offset > node_ptr->data_size)
                throw std::invalid_argument("iterator is invalid");
            if (node_ptr == nullptr)
                throw std::invalid_argument("iterator is null");
            return node_ptr->data[offset];
        }
        T* operator->() {
            assert(offset < node_ptr->data_size);
            return &node_ptr->data[offset];
        }

        // i++
        Iterator operator++(int) {
            Iterator temp = *this;
            if (offset < node_ptr->data_size - 1)
                offset++;
            else {
                node_ptr = node_ptr->next;
                offset = 0;
            }
            return temp;
        }

        // ++i
        Iterator operator++() {
            if (offset < node_ptr->data_size - 1)
                ++offset;
            else {
                node_ptr = node_ptr->next;
                offset = 0;
            }
            return *this;
        }
        // compare to self
        friend bool operator==(const Iterator& left, const Iterator& right) {
            return left.node_ptr == right.node_ptr;
        }
        friend bool operator!=(const Iterator& left, const Iterator& right) {
            return left.node_ptr != right.node_ptr;
        }
        // compare to items
        friend bool operator==(const Iterator& left, const T& right) {
            return *(left.node_ptr) == right;
        }
        friend bool operator!=(const Iterator& left, const T& right) {
            return *(left.node_ptr) != right;
        }
        // compare to null
        friend bool operator==(const Iterator& left, std::nullptr_t) {
            return left.node_ptr == nullptr;
        }
        friend bool operator!=(const Iterator& left, std::nullptr_t) {
            return left.node_ptr != nullptr;
        }
        bool is_null() { return !node_ptr; }
    };

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

    // return an iterator to entry in the tree
    Iterator search(const T& entry) const;
    // return an iterator to the first entry greater than or equal to
    Iterator lower_bound(const T& entry) const;

    // count the number of elements in the tree
    int size() const;
    // true if the tree is empty
    bool empty() const;

    // print a readable version of the tree
    void print_as_tree(std::ostream& outs = std::cout, int level = 0) const;

    BPlusTree<T>* get_smallest_node() const;
    BPlusTree<T>* get_largest_node() const;
    void print_as_linked(std::ostream& outs = std::cout) const;
    void print_as_list(std::ostream& outs = std::cout) const;

    friend std::ostream& operator<<(std::ostream& outs, const BPlusTree<T>& tree){
        tree.print_as_tree(outs);
        return outs;
    }

    // validation
    bool is_valid();

    // Iterators
    Iterator begin() const;
    Iterator end() const;

    // DEBUG STUFF
    // prints a tree with all he pointers and data 
    void print_as_tree_debug(std::ostream& outs = std::cout, int level = 0) const;
    // prints the events in the event log
    void print_event_log(std::ostream& outs = std::cout) const;
};

template <typename T>
BPlusTree<T>::BPlusTree(bool dupes_allowed) : duplicates_allowed(dupes_allowed) {}

template <typename T>
BPlusTree<T>::BPlusTree(const BPlusTree<T>& other) {
    copy_tree(other);
    link_leaves();
}

template <typename T>
BPlusTree<T>& BPlusTree<T>::operator=(const BPlusTree<T>& other) {
    if (this == &other)
        return *this;
    clear_tree();
    copy_tree(other);
    link_leaves();
    return *this;
}

template<typename T>
BPlusTree<T>::~BPlusTree() {
    clear_tree();
}

template <typename T>
void BPlusTree<T>::clear_tree() {
    for (size_t i = 0; i < subset_size; ++i) {
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
    b_array::copy_array(other.data, data, other.data_size, data_size);
    duplicates_allowed = other.duplicates_allowed;
    subset_size = other.subset_size;
    for (size_t i = 0; i < other.subset_size; ++i) {
        if (other.subset[i] != nullptr) {
            subset[i] = new BPlusTree<T>(other.duplicates_allowed);
            subset[i]->copy_tree(*(other.subset[i]));
        }
    }
}

template <typename T>
void BPlusTree<T>::print_event_log(std::ostream& outs) const {
    for (size_t i = 0; i < event_log.size(); ++i) {
        outs << event_log[i] << std::endl;
    }
}

template <typename T>
void BPlusTree<T>::print_as_tree(std::ostream& outs, int level) const {
    // if not leaf keep recursioning down until it is
    if (subset_size > 1) {
        // print half of the subset backwards
        for (size_t i = subset_size - 1; i >= subset_size / 2; i--) {
            if (subset[i] != nullptr)
                subset[i]->print_as_tree(outs, level + 1);
            else {
                b_array::print_array(data, 0, level + 1, outs);
                outs << std::endl;
            }
        }
    }

    // print the parent dataset
    b_array::print_array(data, data_size, level, outs);
    outs << std::endl;
    

    if (subset_size > 1) {
        // print the other half
        for (int i = (subset_size / 2) - 1; i >= 0; i--) {
            if (subset[i] != nullptr)
                subset[i]->print_as_tree(outs, level + 1);
            else {
                b_array::print_array(data, 0, level + 1, outs);
                outs << std::endl;
            }
        }
    }
}
template <typename T>
void BPlusTree<T>::print_as_tree_debug(std::ostream& outs, int level) const {
    // if not leaf keep recursioning down until it is
    if (subset_size > 1) {
        // print half of the subset backwards
        for (size_t i = subset_size - 1; i >= subset_size / 2; i--) {
            if (subset[i] != nullptr)
                subset[i]->print_as_tree_debug(outs, level + 1);
            else {
                outs << level::create_space(level, 32) << "[ " << this << " ]" << std::endl;
                b_array::print_array(data, data_size, level + 1, outs, 32);
                outs << std::endl;
                b_array::print_array(subset, subset_size, level + 1, outs, 32);
                outs << std::endl;
                if (is_leaf()) {
                    outs << level::create_space(level, 32) << "[ " << next << " ]" << std::endl;
                }
                outs << std::endl;
            }
        }
    }

    // print the parent dataset
    // format
    // current address
    // data
    // subset pointers
    // next pointer (if leaf)
    outs << level::create_space(level, 32) << "[ " << this << " ]" << std::endl;
    b_array::print_array(data, data_size, level, outs, 32);
    outs << std::endl;
    b_array::print_array(subset, subset_size, level, outs, 32);
    outs << std::endl;
    if (is_leaf()) {
        outs << level::create_space(level, 32) << "[ " << next << " ]" << std::endl;
    }
    outs << std::endl;

    if (subset_size > 1) {
        // print the other half
        for (int i = (subset_size / 2) - 1; i >= 0; i--) {
            if (subset[i] != nullptr)
                subset[i]->print_as_tree_debug(outs, level + 1);
            else {
                outs << level::create_space(level, 32) << "[ " << this << " ]" << std::endl;
                b_array::print_array(data, data_size, level + 1, outs, 32);
                outs << std::endl;
                b_array::print_array(subset, subset_size, level + 1, outs, 32);
                outs << std::endl;
                if (is_leaf()) {
                    outs << level::create_space(level, 32) << "[ " << next << " ]" << std::endl;
                }
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
    int index = b_array::first_ge(data, data_size, entry);
    if (!is_leaf() && data[index] != entry)
        subset[index]->loose_insert(entry);
    else {
        if (data[index] != entry)
            b_array::insert_item(data, index, data_size, entry);
        return;
    }
    if (subset[index]->data_size > MAXIMUM) {
        fix_excess(index);
    }
}
template <typename T>
void BPlusTree<T>::insert(const T& entry) {

    // Log event
    std::stringstream event;
    event << "Inserting: " << entry;
    event_log.push_back(event.str());

    // the function that called this has to be root
    loose_insert(entry);
    // so do root checks here
    if (data_size > MAXIMUM) {
        // create new temp node
        BPlusTree<T>* temp = new BPlusTree<T>(duplicates_allowed);
        // copy data to temp node
        b_array::copy_array(data, temp->data, data_size, temp->data_size);
        b_array::copy_array(subset, temp->subset, subset_size, temp->subset_size);
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
    b_array::split(node->data, node->data_size, split->data, split->data_size);
    b_array::split(node->subset, node->subset_size, split->subset,
                 split->subset_size);

    // connect the nodes together
    split->next = node->next;
    node->next = split;

    // insert the last item into self
    T item = b_array::detach_item(node->data, node->data_size);
    b_array::ordered_insert(data, data_size, item);
    b_array::insert_item(subset, index + 1, subset_size, split);

    // insert the item into the new node
    if (split->is_leaf())
        b_array::ordered_insert(split->data, split->data_size, item);
}

template <typename T>
const T& BPlusTree<T>::get(const T& entry) const {
    size_t index = b_array::first_ge(data, data_size, entry);
    bool found = index < data_size && data[index] == entry;
    if (found && is_leaf())
        return data[index];
    else if (found && !is_leaf())
        return subset[index + 1]->find(entry);
    else if (!found && !is_leaf())
        return subset[index]->find(entry);
    else if (!found && is_leaf())
        throw std::out_of_range("Item not in tree");
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
    size_t index = b_array::first_ge(data, data_size, entry);
    bool found = index < data_size && data[index] == entry;
    if (found && is_leaf())
        return &data[index];
    else if (found && !is_leaf())
        return subset[index + 1]->find(entry);
    else if (!found && !is_leaf())
        return subset[index]->find(entry);
    else if (!found && is_leaf())
        return nullptr;
    return nullptr;
}
template <typename T>
bool BPlusTree<T>::contains(const T& entry) const {
    return data[b_array::first_ge(data, data_size, entry)] == entry;
}
template <typename T>
void BPlusTree<T>::rotate_left(int i) {
    if (subset[i] == nullptr) {
        subset[i] = new BPlusTree<T>(duplicates_allowed);
        subset_size++;
    }
    b_array::attach_item(subset[i]->data, subset[i]->data_size, data[i]);
    
    // guard rails to make sure im not rotating the wrong way
    assert(subset[i + 1] != nullptr);
    data[i] = b_array::delete_item(subset[i + 1]->data, 0, subset[i + 1]->data_size);

    // rotate subset pointers, if there actually are any pointers to rotate
    if (subset[i + 1]->subset_size > 0) {
        if (subset[i + 1]->subset[0] != nullptr) {
            b_array::attach_item(
                subset[i]->subset,
                subset[i]->subset_size,
                b_array::delete_item(subset[i + 1]->subset, 0, subset[i + 1]->subset_size)
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
    b_array::insert_item(subset[i + 1]->data, 0, subset[i + 1]->data_size, data[i]);

    assert(subset[i] != nullptr);
    data[i] = b_array::detach_item(subset[i]->data, subset[i]->data_size);

    // rotate subset pointers, if there actually are any pointers to rotate
    if (subset[i]->subset_size > 0) {
        if (subset[i]->subset[subset[i]->subset_size - 1] != nullptr) {
            b_array::insert_item(
                subset[i + 1]->subset,
                0,
                subset[i + 1]->subset_size,
                b_array::detach_item(subset[i]->subset, subset[i]->subset_size)
            );
        }
    }
}

template <typename T>
void BPlusTree<T>::erase_node() {
    data_size = 0;
    next = nullptr;
    subset_size = 0;
    // null out the subset array before it causes problems
    for (size_t i = 0; i < SUBSET_CAPACITY; ++i) {
        if (i < DATA_CAPACITY)
            data[i] = T();
        subset[i] = nullptr;
    }
}

template <typename T>
void BPlusTree<T>::merge_with_next_subset(size_t i) {
    assert(i + 1 < subset_size);
    b_array::merge(subset[i]->data, subset[i + 1]->data, subset[i]->data_size,
                 subset[i + 1]->data_size);
    b_array::merge(subset[i]->subset, subset[i + 1]->subset,
                 subset[i]->subset_size, subset[i + 1]->subset_size);
    // update the next
    subset[i]->next = subset[i + 1]->next;
    // make sure we arent deleting its children too
    subset[i + 1]->erase_node();
    delete subset[i + 1];
    b_array::delete_item(subset, i + 1, subset_size);
}

template <typename T>
void BPlusTree<T>::remove(const T& entry) {

    // Log event
    std::stringstream event;
    event << "Removing: " << entry;
    event_log.push_back(event.str());

    // the function that called this has to be root
    loose_remove(entry);
    if (data_size < MINIMUM && subset_size > 0) {
        auto shrink_ptr = subset[0];
        subset[0] = nullptr;
        b_array::copy_array(shrink_ptr->data, data, shrink_ptr->data_size, data_size);
        b_array::copy_array(shrink_ptr->subset, subset, shrink_ptr->subset_size, subset_size);

        shrink_ptr->erase_node();
        delete shrink_ptr;
    }
}

template <typename T>
void BPlusTree<T>::loose_remove(const T& entry, BPlusTree<T>* origin, int offset) {
    size_t index = b_array::first_ge(data, data_size, entry);
    bool found = index < data_size && data[index] == entry;

    if (found && is_leaf()) {
        // remove the actual item
        b_array::delete_item(data, index, data_size);
        if (origin != nullptr) {
            if (data_size > 0)
                origin->data[offset] = data[0];
            else {
                if (next != nullptr)
                    origin->data[offset] = next->data[0];
            }
        }
    }
    else if (found && !is_leaf()) {
        subset[index + 1]->loose_remove(entry, this, index);
        if (index == data_size)
            index--;
        fix_shortage(index);
    }
    else if (!found && !is_leaf()) {
        subset[index]->loose_remove(entry, origin, offset);
        if (index == data_size)
            index--;
        fix_shortage(index);
    }
    else if (!found && is_leaf()) {
        throw std::out_of_range("Item not in tree");
        // return;
    }
    for (size_t i = 0; i < data_size; ++i) {
        fix_shortage(i);
    }
}

template <typename T>
T BPlusTree<T>::remove_biggest() {
    if (is_leaf()) {
        return b_array::detach_item(data, data_size);
    }
    T item = subset[subset_size - 1]->remove_biggest();
    // make sure to get rid of the node we just pulled from it if it's empty
    if (subset[subset_size - 1]->data_size <= 0) {
        delete b_array::detach_item(subset, subset_size);
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
    // do regular fix b tree fix shortage if we arent about to hit a leaf
    if (!left->is_leaf() || !right->is_leaf()) {
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
            T item = b_array::delete_item(data, index, data_size);
            if (right == nullptr) {
                auto temp = new BPlusTree<T>(duplicates_allowed);
                b_array::insert_item(subset, index + 1, subset_size, temp);
                right = subset[index + 1];
            }
            b_array::ordered_insert(right->data, right->data_size, item);
            // merge them
            merge_with_next_subset(index);
        }
    }
    // we're at a node right before the leaf, do the b+tree fix instead
    else {
        // case 1: we have a sibling to borrow from (left)
        if (left != nullptr && left->data_size > MINIMUM &&
            (right == nullptr || right->data_size <= MINIMUM)) {
            transfer_right(index);
        }
        // case 1.5: we have a sibling to borrow from (right)
        else if (right != nullptr && right->data_size > MINIMUM &&
                 (left == nullptr || left->data_size <= MINIMUM)) {
            transfer_left(index);
        }
        // case 2: we dont have siblings to borrow from
        else {
            b_array::delete_item(data, index, data_size);
            if (right == nullptr) {
                auto temp = new BPlusTree<T>(duplicates_allowed);
                b_array::insert_item(subset, index + 1, subset_size, temp);
                right = subset[index + 1];
            }
            // merge them
            merge_with_next_subset(index);
        }
    }
}

template <typename T>
BPlusTree<T>* BPlusTree<T>::get_smallest_node() const {
    BPlusTree<T>* smallest = (BPlusTree<T>*)this;
    while (!smallest->is_leaf()) {
        smallest = smallest->subset[0];
    }
    return smallest;
}

template <typename T>
BPlusTree<T>* BPlusTree<T>::get_largest_node() const {
    BPlusTree<T>* largest = (BPlusTree<T>*)this;
    while (!largest->is_leaf()) {
        largest = largest->subset[largest->subset_size - 1];
    }
    return largest;
}
template <typename T>
void BPlusTree<T>::print_as_linked(std::ostream& outs) const {
    BPlusTree<T>* current_node = get_smallest_node();
    while (current_node != nullptr) {
        b_array::print_array(current_node->data, current_node->data_size, 0, outs);
        outs << " -> ";
        current_node = current_node->next;
    }
    outs << " |||";
}
template <typename T>
void BPlusTree<T>::print_as_list(std::ostream& outs) const {
    for (auto it = begin(); it != nullptr; it++) {
        if (it != nullptr)
            outs << *it << std::endl;
    }
}

template <typename T>
typename BPlusTree<T>::Iterator BPlusTree<T>::begin() const {
    auto node = get_smallest_node();
    if (node->data_size == 0)
        return Iterator(nullptr, 0);
    return Iterator(node);
}

template <typename T>
typename BPlusTree<T>::Iterator BPlusTree<T>::end() const {
    auto node = get_largest_node();
    if (node->data_size == 0)
        return Iterator(nullptr);
    return Iterator(node);
}

template <typename T>
typename BPlusTree<T>::Iterator BPlusTree<T>::search(const T& entry) const {
    size_t index = b_array::first_ge(data, data_size, entry);
    bool found = index < data_size && data[index] == entry;
    if (found && is_leaf())
        return Iterator((BPlusTree<T>*)this, index);
    else if (found && !is_leaf())
        return subset[index + 1]->search(entry);
    else if (!found && !is_leaf())
        return subset[index]->search(entry);
    else if (!found && is_leaf())
        return Iterator(nullptr);    
    return Iterator(nullptr);
}

template <typename T>
void BPlusTree<T>::transfer_left(size_t i) {
    assert(i < subset_size);
    // should never call if sibling has nothing to borrow
    assert(subset[i + 1]->data_size > 1);
    // should never call on a non leaf
    assert(subset[i]->subset_size == 0);

    b_array::attach_item(
        subset[i]->data,
        subset[i]->data_size,
        b_array::delete_item(subset[i + 1]->data, 0, subset[i + 1]->data_size)
    );

    // update parent with right child
    data[i] = subset[i + 1]->data[0];
}

template <typename T>
void BPlusTree<T>::transfer_right(int i) {
    // assert(i > data_size);
    // should never call if sibling has nothing to borrow
    assert(subset[i]->data_size > 1);
    // should never call on a non leaf
    assert(subset[i]->subset_size == 0);

    b_array::insert_item(
        subset[i + 1]->data,
        0,
        subset[i + 1]->data_size,
        b_array::detach_item(subset[i]->data, subset[i]->data_size)
    );

    // update parent with right child
    data[i] = subset[i + 1]->data[0];
}

template <typename T>
BPlusTree<T>* BPlusTree<T>::link_leaves(BPlusTree<T>* previous) {
    if (is_leaf())
        return nullptr;
    // link if the thing below is a leaf
    if (subset_size > 0 && subset[0]->is_leaf()) {
        if (previous != nullptr)
            previous->next = subset[0];
        for (size_t i = 0; i < subset_size - 1; ++i) {
            subset[i]->next = subset[i + 1];
        }
        return subset[subset_size - 1];
    }
    // keep recursioning down
    else {
        BPlusTree<T>* last_link = previous;
        for (size_t i = 0; i < subset_size - 1; ++i) {
            BPlusTree<T>* left = subset[i];
            BPlusTree<T>* right = subset[i + 1];
            last_link = left->link_leaves(last_link);
            BPlusTree<T>* right_link = right->link_leaves(last_link);
            last_link = right_link;
        }
        return last_link;
    }
}
template <typename T>
bool BPlusTree<T>::is_valid() {
    /* is_valid():
    *  check to see if all B+ tree conditions are met with the current (sub)tree:
    * if is_leaf: return true;
    * if (data[last item] is NOT <= all subset[last subtree]->data
    *      return false;
    * for every data[i]
    *      if data[i] is NOT > all subset[i]->data
    *          return false;
    * //now, this is lame:
    * for every data[i]
    *      if data[i] is not in the leaves
    *          return false;
    *
    * for every subtree subset[i]
    *      if !subset[i]->is_valid()
    *          return false;
    * . . . . . . . . . . . . . . . .
    */
    if (is_leaf())
        return true;
    if (!b_array::is_le(subset[subset_size - 1]->data,
                        subset[subset_size - 1]->data_size,
                        data[data_size - 1]))
        return false;
    for (size_t i = 0; i < data_size; ++i) {
        if (!b_array::is_gt(subset[i]->data,
                            subset[i]->data_size,
                            data[data_size - 1]))
            return false;
        if (find(data[i]) == nullptr) {
            return false;
        }
    }
    for (size_t i = 0; i < subset_size; ++i) {
        if (!subset[i]->is_valid())
            return false;
    }
    return true;
}

template <typename T>
typename BPlusTree<T>::Iterator
BPlusTree<T>::lower_bound(const T& entry) const {
    size_t index = b_array::first_ge(data, data_size, entry);
    bool found = index < data_size && data[index] == entry;
    if (found && is_leaf())
        return Iterator((BPlusTree<T>*)this, index);
    else if (found && !is_leaf())
        return subset[index + 1]->lower_bound(entry);
    else if (!found && !is_leaf())
        return subset[index]->lower_bound(entry);
    else if (!found && is_leaf()) {
        if (index < data_size)
            return Iterator((BPlusTree<T>*)this, index);
        else
            return Iterator(next);
    }
    return Iterator(nullptr);
}