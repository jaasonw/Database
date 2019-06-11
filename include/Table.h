#pragma once
#include "Index.h"
#include "Map/Map.h"
#include "MultiMap/MultiMap.h"
#include "constants.h"
#include "rpn/shunting_yard.h"
#include "util/io/Record.h"
#include "util/vector_set.h"
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

    // reads the field data from the database
    bool db_read();

    // iterates through the entire file and indexes the database
    // should only be run once, when the table is initialized
    void reindex();

    // returns a vector of indices that satisfy the where clause
    std::vector<long> evaluate_where(Queue<std::string>& where);

    // returns the indicies of all entries of arg1 that are greater than arg2
    std::vector<long> get_greater(std::string arg1, std::string arg2);
    // returns the indicies of all entries of arg1 that are less than arg2
    std::vector<long> get_less(std::string arg1, std::string arg2);
    // returns the indicies of all entries of arg1 that are equal than arg2
    std::vector<long> get_equal(std::string arg1, std::string arg2);


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
    // returns the temp table
    Table select(const std::vector<std::string>& fields, const std::vector<std::string>& where);

    // initilizes an empty file with the columns
    void init_file();

    // returns a string with the filename
    std::string get_filename() const;

    // prints the memory address of the table
    friend std::ostream& operator<<(std::ostream& outs, const Table& table);

    // prints the contents of the table
    std::ostream& print_table(std::ostream& outs = std::cout);
};