#pragma once
#include "Parser/Parser.h"
#include "Table.h"
#include <iostream>
#include <string>

class SQL {
private:
    Parser parser;

public:
    SQL();
    void interactive();
    void execute_file(std::string filename);
    void execute_string(std::string str);
};

SQL::SQL() {}

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
            Table t(parse_tree["table_name"][0]);
            t.select(parse_tree["fields"], parse_tree["where"]);
            Table temp("temp");
            std::cout << temp << '\n';
        }
        // create
        if (parse_tree["command"][0] == "create") {
            Table t(parse_tree["table_name"][0], parse_tree["fields"]);
        }
        // insert
        if (parse_tree["command"][0] == "insert") {
            Table t(parse_tree["table_name"][0]);
            t.insert_into(parse_tree["fields"]);
        }

    } catch (std::runtime_error e) {
        std::cerr << e.what() << '\n';
    }
}
