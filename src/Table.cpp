#include "Table.h"

Table::Table(const char* name) : name(name) {}

bool Table::db_open() {
    const char* filename = (name + ".db").c_str();
    // const char* filename_fields = (name + "_fields.db").c_str();
    bin_io::open_fileRW(file_stream, filename);
    // if file is not empty
    if (file_stream.peek() != std::ifstream::traits_type::eof()) {
        Record index_block;
        index_block.read(file_stream, 0);
        for (int i = 0; i < index_block.ROWS && index_block.buffer[i][0] != '\0'; i++) {
            char field[index_block.COLS] = "";
            strcpy(field, index_block.buffer[i]);
            fields.push_back(field);
        }
    }
    std::cout << fields << '\n';
    return file_stream.fail();
}

void Table::insert_into(const std::vector<std::string>& items) {}