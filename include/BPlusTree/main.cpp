/*
 * Author: Jason Wong
 * Project: BTree
 * Purpose: Implementing a BTree
 * Notes:
 */
#include "BPlusTree.h"
#include "test/tree_test.h"
#include "test/three_test.h"
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char const* argv[]) {
    // b_plus_tree_big_three::test_assignment_operator();
    // b_plus_tree_big_three::test_copy_constructor();
    // b_plus_tree_big_three::test_delete();

    BPlusTree<int> tree;
    b_plus_tree_test::interactive_test(tree);

    cout << endl;
    system("pause");
    return 0;
}