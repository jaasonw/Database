#pragma once
#include "constants.h"
#include "util/io/binary_util.h"
#include <cstring>
#include <vector>

// represents a block of data
struct Record {
    static const int ROWS = constants::MAX_BLOCK_ROWS;
    static const int COLS = constants::MAX_BLOCK_ROWS;

    int record_number;
    char buffer[ROWS][COLS];

    Record();
    // appends the record to the end of the file at outs
    long append_to_file(std::fstream& outs);
    // writes the record to the index in the file
    void write(std::fstream& outs, long index);
    // reads a record from the index of a file
    long read(std::fstream& ins, long record_number);

    // writes a row to the first null row in the buffer
    // returns true if successfully written, false if buffer is full
    bool write_row(const char* str);
    // resets the buffer to nulls
    void clear_buffer();

    // returns a copy of the record in vector format, stops at either the first
    // null row or max row number
    std::vector<std::string> to_vector();
    
    // initializes the buffer to the values in the vector
    void create_from_vector(const std::vector<std::string>& items);

    // prints the record as a row
    friend std::ostream& operator<<(std::ostream& outs, const Record& r);

};

// void save_list(Record* list, int count) {
//     std::fstream f;
//     bin_io::open_fileW(f, "record_list.bin");
//     for (int i = 0; i < count; i++) {
//         list[i].write(f);
//     }
//     f.close();
// }