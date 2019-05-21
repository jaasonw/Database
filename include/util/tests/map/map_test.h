#pragma once
#include "Map/Map.h"
#include "Multimap/MultiMap.h"
#include <cctype>
#include <cmath>
#include <iostream>
#include <string>

// a mostly generic interactive test with tweaks for the appropriate structure
namespace map_test {
void interactive_test_map(Map::Map<std::string, int>& map) {
    while (true) {
        char input;
        std::cout << "[R]andom [G]enerate [I]nsert [D]elete [C]lear  [S]earch  "
                     " e[X]it: ";
        std::cin >> input;
        switch (tolower(input)) {
            case 'r':
                // map.set(rand() % 100);
                break;
            case 'g':
                map.clear();
                map.set("75", 0);
                map.set("100", 0);
                map.set("25", 0);
                map.set("50", 0);
                map.set("110", 0);
                map.set("112", 0);
                map.set("80", 0);
                map.set("12", 0);
                map.set("30", 0);
                map.set("60", 0);
                break;
            case 'd': {
                std::string key;
                std::cin >> key;
                try {
                    map.erase(key);
                } catch (std::out_of_range) {
                    std::cout << "Item not in map" << std::endl;
                }
                break;
            }

            case 'i': {
                std::string key;
                int value;
                std::cin >> key >> value;
                map.set(key, value);
                break;
            }
            case 'c':
                map.clear();
                break;
            case 's': {
                std::string key;
                int value;
                std::cin >> key >> value;
                if (map.contains(key, value))
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
        std::cout << map;
    }
}

void interactive_test_multimap(MultiMap::MultiMap<std::string, int>& map) {
    while (true) {
        char input;
        std::cout << "[R]andom [G]enerate [I]nsert [D]elete [C]lear  [S]earch  "
                     " e[X]it: ";
        std::cin >> input;
        switch (tolower(input)) {
            case 'r':
                // map.set(rand() % 100);
                break;
            case 'g':
                map.clear();
                map.insert("75", 0);
                map.insert("100", 0);
                map.insert("25", 0);
                map.insert("50", 0);
                map.insert("110", 0);
                map.insert("112", 0);
                map.insert("80", 0);
                map.insert("12", 0);
                map.insert("30", 0);
                map.insert("60", 0);
                break;
            case 'd': {
                std::string key;
                std::cin >> key;
                try {
                    map.erase(key);
                } catch (std::out_of_range) {
                    std::cout << "Item not in map" << std::endl;
                }
                break;
            }

            case 'i': {
                std::string key;
                int value;
                std::cin >> key >> value;
                map.insert(key, value);
                break;
            }
            case 'c':
                map.clear();
                break;
            case 's': {
                std::string key;
                int value;
                std::cin >> key;
                if (map.contains(key))
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
        std::cout << map;
    }
}
} // namespace BTree_test