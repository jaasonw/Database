#include "BPlusTree/BPlusTree.h"
#include "BPlusTree/test/three_test.h"
#include "BPlusTree/test/tree_debug.h"
#include "BPlusTree/test/tree_test.h"
#include "util/Random.h"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

using namespace std;

bool test_bplustree_auto(size_t how_many, bool report);
void test_iterator() {
    const bool debug = false;
    BPlusTree<int> bpt;
    for (int i = 0; i < 125; i++)
        bpt.insert(Random(0, 100));
    cout << bpt << endl;

    cout << "------------------------------------------------------------"
         << endl;
    for (BPlusTree<int>::Iterator it = bpt.begin(); it != bpt.end(); ++it) {
        // if (debug) it.print_Iterator();
        cout << "[" << *it << "] ";
        if (debug)
            cout << endl;
    }
    cout << endl;
    cout << "------------------------------------------------------------"
         << endl;
    cout << "test ++ operator: " << endl;
    BPlusTree<int>::Iterator it = bpt.begin();
    cout << "{" << *(it++) << "}" << endl;
    cout << "{" << *it << "}" << endl;
    for (int i = 60; i < 75; i++) {
        it = bpt.search(i);
        if (!it.is_null())
            cout << *it << " was found." << endl;
        else
            cout << i << " was not found" << endl;
    }
    cout << "=================================================================="
            "=="
         << endl;
}

void test_bplustree_auto(size_t tree_size, int how_many, bool report) {
    bool verified = true;
    for (int i = 0; i < how_many; i++) {
        if (report) {
            cout << "*********************************************************"
                 << endl;
            cout << " T E S T:    " << i << endl;
            cout << "*********************************************************"
                 << endl;
        }
        if (!test_bplustree_auto(tree_size, report)) {
            cout << "T E S T :   [" << i << "]    F A I L E D ! ! !" << endl;
            verified = false;
            return;
        }
    }
    cout << "******************************************************************"
            "********"
         << endl;
    cout << "******************************************************************"
            "********"
         << endl;
    cout << "             E N D     T E S T: " << how_many << " tests of "
         << tree_size << " items: ";
    cout << (verified ? "VERIFIED" : "VERIFICATION FAILED") << endl;
    cout << "******************************************************************"
            "********"
         << endl;
    cout << "******************************************************************"
            "********"
         << endl;
}

bool test_bplustree_auto(size_t how_many, bool report) {
    const size_t MAX = 10000;
    assert(how_many < MAX);
    BPlusTree<int> bpt;
    // int a[MAX];
    // int original[MAX];
    // int deleted_list[MAX];
    b_array::Array<int> a(MAX);
    b_array::Array<int> original(MAX);
    b_array::Array<int> deleted_list(MAX);

    size_t original_size;
    size_t size;
    size_t deleted_size = 0;

    // fill a[ ]
    for (int i = 0; i < how_many; i++) {
        a[i] = i;
    }
    // shuffle a[ ]: Put this in a function!
    for (int i = 0; i < how_many; i++) {
        int from = Random(0, how_many - 1);
        int to = Random(0, how_many - 1);
        int temp = a[to];
        a[to] = a[from];
        a[from] = temp;
    }
    // copy  a[ ] -> original[ ]:
    // b_array::copy_array(original, a, how_many, size);
    original = a;
    size = how_many;
    original_size = how_many;
    for (int i = 0; i < size; i++) {
        bpt.insert(a[i]);
    }
    if (report) {
        cout << "========================================================"
             << endl;
        cout << "  " << endl;
        cout << "========================================================"
             << endl;
        cout << bpt << endl << endl;
    }
    for (int i = 0; i < how_many; i++) {
        int r = Random(0, how_many - i - 1);
        if (report) {
            cout << "========================================================"
                 << endl;
            cout << bpt << endl;
            cout << ". . . . . . . . . . . . . . . . . . . . . . . . . . . . "
                 << endl;
            cout << "deleted: ";
            b_array::print_array(deleted_list, deleted_size);
            cout << "   from: ";
            b_array::print_array(original, original_size);
            cout << endl;
            cout << "  REMOVING [" << a[r] << "]" << endl;
            cout << "========================================================"
                 << endl;
        }
        bpt.remove(a[r]);

        
        int item = b_array::delete_item(a, r, size);
        deleted_list[deleted_size] = item;
        deleted_size++;
        // b_array::attach_item(deleted_list, deleted_list, item);
        if (!bpt.is_valid()) {
            cout << setw(6) << i << " I N V A L I D   T R E E" << endl;
            cout << "Original Array: ";
            b_array::print_array(original, original_size);
            cout << "Deleted Items : ";
            b_array::print_array(deleted_list, deleted_size);
            cout << endl << endl << bpt << endl << endl;
            return false;
        }
    }
    if (report)
        cout << " V A L I D    T R E E" << endl;

    return true;
}

using namespace std;
int main(int argc, char *argv[])
{
    cout <<endl<<endl<<endl<< "===============================" << endl<<endl<<endl<<endl;

    //------------------------------------------
    srand(time(0));
    //------------------------------------------



//    test_bplustree_insert_random();
//    test_bplustree_remove();
//    test_bplustree_interactive();
//    test_bplustree_big_three();
    test_bplustree_auto(1000,100,false);

//    test_map();
//    test_map_interactive();

//    test_multimap();
//    test_multimap_interactive();
//    test_multimap_auto();

//    test_iterator();
    cout <<endl<<endl<<endl<< "===============================" << endl;
    return 0;
}
