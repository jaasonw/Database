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
};

Table::Table(const char* name) : name(name) {}

bool Table::db_open() {
    const char* filename = (name + ".db").c_str();
    // const char* filename_fields = (name + "_fields.db").c_str();
    bin_io::open_fileRW(file_stream, filename);
    // if file is not empty
    if (file_stream.peek() != std::ifstream::traits_type::eof()) {
        Record index_block;
        index_block.read(file_stream, 0);
    }
    return file_stream.fail();
}

void Table::insert_into(const std::vector<std::string>& items) {}