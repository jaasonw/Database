/*
 * Author: Jason Wong
 * Project: BTree
 * Purpose: Implementing a BTree
 * Notes:
 */
#include "BPlusTree.h"
#include "test/three_test.h"
#include "test/tree_debug.h"
#include "test/tree_test.h"
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char const* argv[]) {
    // b_plus_tree_big_three::test_assignment_operator();
    // b_plus_tree_big_three::test_copy_constructor();
    // b_plus_tree_big_three::test_delete();

    BPlusTree<int> tree;
    try {
        b_plus_tree_test::interactive_test(tree);

    } catch (const std::exception& e) {
        // some sort of exception was called, catch it and dump it in a file
        string filename = "tree_dump.txt";
        cerr << "Uh oh, something wasn't right, tree has been dumped in: "
             << filename << endl;
        cerr << "Error: ";
        cerr << e.what() << endl;
        tree_debug::dump_tree(filename, tree);
        cerr << "Events: " << endl;
        tree.print_event_log(cerr);
    }

    cout << endl;
    system("pause");
    return 0;
}