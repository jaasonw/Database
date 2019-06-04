#pragma once
#include "Parser/Parser.h"
#include "Table.h"
#include <iostream>
#include <string>

class SQL {
private:
    Parser parser;
    // um, add in the big 3 for this later, not that i'll ever need it
    Map::Map<std::string, Table*> tables;
public:
    SQL();
    void interactive();
    void execute_file(std::string filename);
    void execute_string(std::string str);
};

SQL::SQL() {
    // create file if it dont exist
    std::ofstream fout;
    fout.open("tables.txt", std::ios::app);
    fout.close();

    std::ifstream fin;
    fin.open("tables.txt");
    std::string tablename;
    while (fin >> tablename) {
        tables[tablename] = new Table(tablename);
    }
    std::cout << tables;
    fin.close();
}

void SQL::interactive() {
    std::string command;
    while (true) {
        std::cout << "sql> ";
        getline(std::cin, command);
        execute_string(command);
    }
}

void SQL::execute_string(std::string command) {
    try {
        auto parse_tree = parser.parse(command);
        #ifdef DEBUG
            std::cout << parse_tree << '\n';
        #endif
        // select
        if (parse_tree["command"][0] == "select") {
            tables[parse_tree["table_name"][0]]->select(parse_tree["fields"],
                                                        parse_tree["where"]);
            Table temp("temp");
            std::cout << temp << '\n';
        }
        // create
        if (parse_tree["command"][0] == "create") {
            tables[parse_tree["table_name"][0]] =
                new Table(parse_tree["table_name"][0], parse_tree["fields"]);
            std::ofstream fout;
            fout << '\n' <<parse_tree["table_name"][0] << '\n';
            fout.close();
        }
        // insert
        if (parse_tree["command"][0] == "insert") {
            tables[parse_tree["table_name"][0]]->insert_into(
                parse_tree["fields"]);
        }

    } catch (std::runtime_error e) {
        std::cerr << e.what() << '\n';
    }
}
