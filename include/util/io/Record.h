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
    long append_to_file(std::fstream& outs);
    void write(std::fstream& outs, long index);
    long read(std::fstream& ins, long record_number);

    // writes a row to the first null row in the buffer
    // returns true if successfully written, false if buffer is full
    bool write_row(const char* str);
    void clear_buffer();

    std::vector<std::string> to_vector();
    void create_from_vector(const std::vector<std::string>& items);

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