/*
 * Author: Jason Wong
 * Project:
 * Purpose:
 * Notes:
 */
#include "Parser/Parser.h"
#include "Table.h"
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
            cout << parse_tree << '\n';
            if (parse_tree["command"][0] == "select") {
                Table t(parse_tree["table_name"][0]);
                t.db_open();
                cout << t.select(parse_tree["fields"][0]) << '\n';
                t.db_close();
            }
        }
        catch (std::runtime_error e) {
            cerr << e.what() << endl;
        }
    }

    cout << endl;
    system("pause");
    return 0;
}