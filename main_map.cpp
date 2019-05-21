/*
 * Author: Jason Wong
 * Project: Map/Multimap 
 * Purpose: 
 * Notes:
 */
#include "Map/Map.h"
#include "Multimap/MultiMap.h"
#include "util/tests/map/map_test.h"
#include <iostream>
#include <string>

using namespace std;

void test(const Map::Map<int, int> test) {
    cout << test[75];
}

int main(int argc, char const* argv[]) {
    // Map::Map<std::string, int> test1;
    MultiMap::MultiMap<std::string, int> test2;
    // map_test::interactive_test_map(test1);
    map_test::interactive_test_multimap(test2);



    // Map Test
    // Map::Map<int, int> test1;
    // test1[75] = 10;
    // test1[100] = 10;
    // test1[25] = 10;
    // test1[50] = 10;
    // test1[110] = 10;
    // test1[80] = 10;
    // test1[30] = 10;
    // test1[60] = 10;
    // test1[12] = 10;


    // cout << test1 << endl;

    // // MultiMap Test
    // MultiMap::MultiMap<int, int> test2;

    // test2.insert(5, 7);
    // test2[5] += 9;
    // test2[5].push_back(10);
    // test2.insert(9, 10);
    // test2.insert(7, 20);

    // cout << test2[5] << endl;
    // cout << test2 << endl;

    cout << endl;
    system("pause");
    return 0;
}