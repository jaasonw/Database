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
    cout << "Enter an sql command: ";
    // cin >> command;
    getline(cin, command);
    p.parse(command);
    cout << endl;
    system("pause");
    return 0;
}