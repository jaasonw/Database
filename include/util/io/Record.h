#pragma once
#include "util/io/binary_util.h"
#include <cstring>
#include <vector>

// represents a block of data
struct Record {
    static const int ROWS = 20;
    static const int COLS = 20;

    int record_number;
    char buffer[ROWS][COLS];

    Record();
    long write(std::fstream& outs);
    long read(std::fstream& ins, long record_number);

    // writes a row to the first null row in the buffer
    // returns true if successfully written, false if buffer is full
    bool write_row(const char* str);

    std::vector<std::string> to_vector();

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