/*
 * Author: Jason Wong
 * Project: 
 * Purpose: 
 * Notes:
 */
#include "Parser/Parser.h"
#include <iostream>

using namespace std;

int main(int argc, char const* argv[]) {
    Parser p("");
    string command;
    while (true) {
        cout << "Enter an sql command: ";
        getline(cin, command);
        try {
            auto parse_tree = p.parse(command);
            cout << parse_tree;
        }
        catch (std::runtime_error e) {
            cerr << e.what() << endl;
        }

    }
    cout << endl;
    system("pause");
    return 0;
}