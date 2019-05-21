#pragma once
#include "Node.h"
#include <cassert>
#include <iostream>

namespace LinkedList {
    // initializes head to NULL
    template <typename T>
    Node<T>* init_head(Node<T>*& head) {
        head = NULL;
        return head;
    }

    // deletes all the Nodes in the list
    template <typename T>
    void delete_all(Node<T>*& head) {
        if (head == NULL)
            return;
        if (head->_next != NULL)
            delete_all(head->_next);
        delete head;
        head = NULL;
    }

    // true if head is NULL, false otherwise.
    template <typename T>
    bool empty(const Node<T>* head) {
        return head == NULL;
    }

    // insert at the beginning of the list:
    template <typename T>
    Node<T>* insert_head(Node<T>*& head, const T& item) {
        Node<T>* node;
        if (head == NULL) {
            node = new Node<T>(item);
            head = node;
        } else {
            node = new Node<T>(item, head);
            head = node;
        }
        return node;
    }
    
    // makes a copy of the list, returns a pointer to the last Node:
    template <typename T>
    Node<T>* copy_list(const Node<T>* src, Node<T>*& dest) {
        // somehow, copy was called on a null
        if (src == NULL)
            return NULL;
        dest = insert_head(dest, src->_item);
        if (src->_next == NULL)
            return dest;
        return copy_list(src->_next, dest->_next);
    }

    // insert_after: if after is NULL, inserts at head
    template <typename T>
    Node<T>* insert_after(Node<T>*& head, Node<T>* after, const T& item) {
        if (head == NULL)
            return insert_head(head, item);
        return insert_head(after->_next, item);
    }

    // delete the Node at the head of the list, return the item:
    template <typename T>
    T delete_head(Node<T>*& head) {
        if (head == NULL) {
            return NULL;
        }
        Node<T>* hold = head;
        T hold_item = head->_item;
        head = head->_next;
        delete hold;
        return hold_item;
    }

    template <typename T>
    Node<T>* previous_node(Node<T>* head, Node<T>* node) {
        if (head == NULL)
            assert("head node is null somehow");
        if (head == node)
            return NULL;
        if (head->next == node)
            return head;
        else
            return previous_node(head->next, node);
    }

    template <typename T>
    Node<T>* find(Node<T>* head, const T& item) {
        if (head == NULL)
            return NULL;
        if (head->_item == item)
            return head;
        else
            return find(head->_next, item);
    }

    // print the list and return outs
    template <typename T>
    std::ostream& print_list(const Node<T>* head, std::ostream& outs = std::cout) {
        if (head == NULL)
            return outs;
        else {
            std::cout << head->_item << " ";
            return print_list(head->_next);
        }
        return outs;
    }
}