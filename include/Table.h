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
    Table(const char* name);

    // opens the db into the file stream, loads the fields and indexes the db
    bool db_open();
    void db_close();
    void insert_into(const std::vector<std::string>& items);
    void select(const char* fields);
};