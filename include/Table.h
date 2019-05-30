#pragma once
#include "MultiMap/MultiMap.h"
#include "util/io/Record.h"
#include <fstream>
#include <string>
#include <vector>

class Table {
private:
    std::string name;
    std::vector<std::string> fields;
    //std::vector<MultiMap::MultiMap<std::string, long>> indices;
    std::fstream file_stream;

public:
    Table(std::string name);

    // opens the db into the file stream, loads the fields and indexes the db
    bool db_open();
    // closes the db
    void db_close();
    void insert_into(std::string items);
    // returns a vector of entries satisfying the query
    std::vector<std::vector<std::string>> select(std::string query);
};