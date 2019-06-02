#pragma once
#include "MultiMap/MultiMap.h"
#include "constants.h"
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
    //std::vector<MultiMap::MultiMap<std::string, long>> indices;

    // returns a cstring with the filename
    const char* get_filename();

    // opens the db into the file stream, loads the fields and indexes the db
    bool db_read();
public:
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