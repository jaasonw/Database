#pragma once
#include <cctype>
#include <cmath>
#include <iostream>

// a mostly generic interactive test with tweaks for the appropriate structure
namespace b_plus_tree_test {
template <typename T>
void interactive_test(T& tree) {
    while (true) {
        char input;
        std::cout << "[R]andom [G]enerate [I]nsert [D]elete [C]lear  [S]earch  "
                     " e[X]it: ";
        std::cin >> input;
        switch (tolower(input)) {
            case 'r':
                tree.insert(rand() % 100);
                break;
            case 'g':
                tree.clear_tree();
                tree.insert(75);
                tree.insert(100);
                tree.insert(25);
                tree.insert(50);
                tree.insert(110);
                tree.insert(112);
                tree.insert(80);
                tree.insert(12);
                tree.insert(30);
                tree.insert(60);
                break;
            case 'd': {
                int input;
                std::cin >> input;
                try {
                    tree.remove(input);
                } catch (std::out_of_range) {
                    std::cout << "Item not in tree" << std::endl;
                }
                break;
            }

            case 'i': {
                int input;
                std::cin >> input;
                tree.insert(input);
                break;
            }
            case 'c':
                // tree = T();
                tree.clear_tree();
                break;
            case 's': {
                int input;
                std::cin >> input;
                if (tree.find(input))
                    std::cout << "found" << std::endl;
                else
                    std::cout << "not found" << std::endl;
                break;
            }
            case 'x':
                return;
            default:
                std::cout << "invalid input" << std::endl;
        }
        std::cout << tree;
    }
}
} // namespace BTree_test