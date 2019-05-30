#pragma once
#include "util/io/binary_util.h"
#include <cstring>

// represents a block of data
class Record {
public:
    Record() {
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                buffer[i][j] = '\0';
            }
        }
        record_number = -1;
    }

    long write(std::fstream& outs);
    long read(std::fstream& ins, long record_number);

    // writes a row to the first null row in the buffer
    // returns true if successfully written, false if buffer is full
    bool write_row(const char* str);

    friend std::ostream& operator<<(std::ostream& outs, const Record& r);

    static const int ROWS = 20;
    static const int COLS = 20;
// private:
    int record_number;
    char buffer[ROWS][COLS];
};

// void save_list(Record* list, int count) {
//     std::fstream f;
//     bin_io::open_fileW(f, "record_list.bin");
//     for (int i = 0; i < count; i++) {
//         list[i].write(f);
//     }
//     f.close();
// }