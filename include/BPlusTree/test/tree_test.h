#pragma once
#include <cctype>
#include <cmath>
#include <iostream>
#include <vector>

// a mostly generic interactive test with tweaks for the appropriate structure
namespace b_plus_tree_test {

template <typename T>
std::vector<int> generate_tree(T& tree) {
    std::vector<int> nums;
    nums.push_back(75);
    nums.push_back(100);
    nums.push_back(25);
    nums.push_back(50);
    nums.push_back(110);
    nums.push_back(112);
    nums.push_back(80);
    nums.push_back(12);
    nums.push_back(30);
    nums.push_back(60);
    nums.push_back(10);
    nums.push_back(22);
    nums.push_back(10);
    nums.push_back(26);
    nums.push_back(11);
    nums.push_back(7);
    nums.push_back(8);
    nums.push_back(1);
    nums.push_back(23);
    nums.push_back(5);
    nums.push_back(1);
    nums.push_back(28);
    nums.push_back(0);
    nums.push_back(25);
    nums.push_back(10);
    nums.push_back(26);
    nums.push_back(19);
    nums.push_back(7);
    nums.push_back(22);
    nums.push_back(11);
    nums.push_back(27);
    nums.push_back(3);
    nums.push_back(27);
    nums.push_back(29);
    nums.push_back(13);
    nums.push_back(13);
    nums.push_back(30);
    nums.push_back(23);
    nums.push_back(2);
    nums.push_back(24);
    for (int i = 0; i < nums.size(); i++) {
        tree.insert(nums[i]);
    }
    return nums;
}
template <typename T>
void interactive_test(T& tree) {
    while (true) {
        char input;
        std::cout << "[R]andom [G]enerate [I]nsert [B]egin [E]nd [D]elete [C]lear  [S]earch  "
                     "du[M]p e[X]it: ";
        std::cin >> input;
        switch (tolower(input)) {
            case 'r':
                tree.insert(rand() % 100);
                break;
            case 'g':
                tree.clear_tree();
                generate_tree(tree);
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
                if (tree.search(input) != nullptr)
                    std::cout << "found" << std::endl;
                else
                    std::cout << "not found" << std::endl;
                break;
            }
            case 'b':
                std::cout<< "Smallest: " << *(tree.begin()) << std::endl;
                break;
            case 'e':
                std::cout << "Largest: " << *(tree.end()) << std::endl;
                break;
            case 'm':
                tree_debug::dump_tree("tree_dump.txt", tree);
                break;
            case 'x':
                return;
            default:
                std::cout << "invalid input" << std::endl;
        }
        std::cout << tree;
        tree.print_as_linked();
        std::cout << std::endl;
        // tree.print_as_list();
        // std::cout << std::endl;
    }
}
} // namespace b_plus_tree_test