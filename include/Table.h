#pragma once
#include "Index.h"
#include "Map/Map.h"
#include "MultiMap/MultiMap.h"
#include "constants.h"
#include "rpn/shunting_yard.h"
#include "util/io/Record.h"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

class Table {
private:
    std::string name;
    
    std::vector<std::string> columns;
    Map::Map<std::string, size_t> column_map;
    Index index;

    // returns a string with the filename
    std::string get_filename();

    // reads the field data from the database
    bool db_read();

    // iterates through the entire file and indexes the database
    // should only be run once, when the table is initialized
    void reindex();

    // just a bunch of error message constants, don't mind these
    const char* CANNOT_FIND_TABLE = "Error: cannot find specified table";
    const char* MAX_FIELD_EXCEEDED = "Error: Max number of fields exceeded";
    const char* MAX_FIELD_SIZE_EXCEEDED = "Error: Max field size exceeded at: ";
    const char* MISMATCHED_COL_NUM = "Error: mismatched column number";
    const char* INVALID_NAME = "Error: Invalid name: ";

public:
    // default constructor so map doesnt flip
    Table();
    // for opening an existing table
    Table(std::string name);
    // for initializing a new table with new fields
    Table(std::string name, const std::vector<std::string>& columns);


    // inserts a row at the end of the file
    void insert_into(const std::vector<std::string>& fields);

    // querys the table and places the results into temp.db
    void select(const std::vector<std::string>& fields, const std::vector<std::string>& where);

    // initilizes an empty file with the columns
    void init_file();

    // returns a vector of entries satisfying the query
    friend std::ostream& operator<<(std::ostream& outs, Table& table);
};