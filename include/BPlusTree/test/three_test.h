#pragma once
#include "BPlusTree.h"
#include <cassert>
#include <iostream>
#include <sstream>

// big 3 unit tests
namespace b_plus_tree_big_three {

// dynamically allocates and deletes trees on an infinite loop
// ram usage should steadilly climb if memory leak
void test_delete() {
    std::cout << "Open task manager and watch ram usage for this" << std::endl;
    std::cout << "Program should not crash during this" << std::endl;
    while (true) {
        auto tree = new BPlusTree<int>();
        for (int j = 0; j < 500000; j++) {
            tree->insert(j);
        }
        delete tree;
    }
}


void test_assignment_operator() {
    std::stringstream s1;
    std::stringstream s2;

    BPlusTree<int> tree1(true);
    for (int i = 0; i <= 5000; i++) {
        tree1.insert(i);
    }
    BPlusTree<int> tree2 = tree1;
    
    // test duplicate mode (insert behavior needs to match)
    tree1.insert(5);
    tree1.insert(5);
    tree1.insert(5);
    tree2.insert(5);
    tree2.insert(5);
    tree2.insert(5);

    // print tree into stringstreams
    s1 << tree1;
    s1 << std::endl;
    tree1.print_as_linked(s1);
    
    s2 << tree2;
    s2 << std::endl;
    tree2.print_as_linked(s2);

    // tree outputs should be the same
    assert(s1.str() == s2.str());
    std::cout << "Assignment operator test passed" << std::endl;
}

void test_copy_constructor() {
    std::stringstream s1;
    std::stringstream s2;
    BPlusTree<int> tree1(true);
    for (int i = 0; i <= 5000; i++) {
        tree1.insert(i / 2);
    }
    BPlusTree<int> tree2(tree1);

    // test duplicate mode (insert behavior needs to match)
    tree1.insert(5);
    tree1.insert(5);
    tree1.insert(5);
    tree2.insert(5);
    tree2.insert(5);
    tree2.insert(5);

    // print tree into stringstreams
    s1 << tree1;
    s1 << std::endl;
    tree1.print_as_linked(s1);

    s2 << tree2;
    s2 << std::endl;
    tree2.print_as_linked(s2);

    assert(s1.str() == s2.str());
    std::cout << "Copy constructor test passed" << std::endl;
}

} // namespace big_three