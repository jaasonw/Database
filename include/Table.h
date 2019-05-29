#pragma once
#include "util/io/Record.h"
#include <fstream>
#include <string>
#include <vector>

class Table {
private:
    std::string name;
    std::vector<std::string> fields;
    std::vector<long> indices;
    std::fstream file_stream;
public:
    Table(const char* name);
    bool db_open();
    void db_close();
    void insert_into(const std::vector<std::string>& items);
};

Table::Table(const char* name) : name(name) {}

bool Table::db_open() {
    // char* filename = (name + ".txt").c_str();
    // if (binary_fio::file_exists(filename)) {
    //     binary_fio::open_fileRW(file_stream, filename);
    // }

}

void Table::insert_into(const std::vector<std::string>& items) {}