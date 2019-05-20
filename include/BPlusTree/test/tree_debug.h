#pragma once
#include "BPlusTree/BPlusTree.h"
#include <fstream>
#include <string>

namespace tree_debug {

// dump a detailed tree with pointers and addresses labeled into a file
template <typename T>
void dump_tree(std::string filename, const BPlusTree<T>& tree) {
    std::ofstream tree_dump;
    tree_dump.open(filename);
    tree.print_as_tree_debug(tree_dump);
    tree_dump.close();
}

} // namespace tree_debug
