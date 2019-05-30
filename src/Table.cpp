#include "Table.h"

Table::Table(std::string name) : name(name) {}

bool Table::db_open() {
    const char* filename = (name + ".db").c_str();
    // const char* filename_fields = (name + "_fields.db").c_str();
    bin_io::open_fileRW(file_stream, filename);
    // if file is not empty
    if (file_stream.peek() != std::ifstream::traits_type::eof()) {
        Record index_block;
        index_block.read(file_stream, 0);
        fields = index_block.to_vector();
    }
    return file_stream.fail();
}

void Table::insert_into(std::string items) {}

std::vector<std::vector<std::string>> Table::select(std::string query) {
    std::vector<std::vector<std::string>> container;
    // this might need to be replaced with something else later
    if (query == "*") {
        Record block;
        // start at 1 because the first block is for fields
        for (int i = 1; block.read(file_stream, i) > 0; i++) {
            container.push_back(block.to_vector());
        }
    }
    return container;
}