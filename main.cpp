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
    try {
        b_plus_tree_test::interactive_test(tree);

    } catch (const std::exception& e) {
        cerr << e.what() << endl;
    }
    // auto a = Array<BPlusTree<int>*>(10);
    // a[0] = new BPlusTree<int>();
    // a[1] = nullptr;
    // cout << a;
    // array::print_array(a, 10);

    cout << endl;
    system("pause");
    return 0;
}