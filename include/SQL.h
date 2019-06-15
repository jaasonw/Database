#pragma once
#include "Parser/Parser.h"
#include "Table.h"
#include <iostream>
#include <string>

class SQL {
private:
    const char* TABLES_FILE = "tables.txt";
    Parser parser;
    Map::Map<std::string, Table> tables;
    void get_tables();
    void print_table_list();
    void print_welcome();
    const char* UNKNOWN_TABLE =
        "Error: Unknown table: , see \"tables\" for available tables";

public:
    SQL();

    void interactive();
    // opens a text file and runs the commands
    void execute_file(std::string filename);
    // executes a singular command, returns true if command was successful
    bool execute_string(std::string str, bool verbose = false);
};