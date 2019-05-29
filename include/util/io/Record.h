#pragma once
#include "util/io/binary_util.h"
#include <cstring>

// represents a block of data
class Record {
private:
    static const int ROWS = 20;
    static const int COLS = 20;
    int record_number;
    char buffer[ROWS][COLS];
public:
    Record() {
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                buffer[i][j] = '\0';
            }
        }
        record_number = -1;
    }

    // Record(char data[ROWS][COLS]) {
    //     for (int i = 0; i < ROWS; i++) {
    //         strcpy(record[i], data[i]);
    //     }
    // }
    long write(std::fstream& outs);
    long read(std::fstream& ins, long record_number);

    // writes a row to the first null row in the buffer
    // returns true if successfully written, false if buffer is full
    bool write_row(const char* str);

    friend std::ostream& operator<<(std::ostream& outs, const Record& r);

};

void save_list(Record* list, int count) {
    std::fstream f;
    binary_fio::open_fileW(f, "record_list.bin");
    for (int i = 0; i < count; i++) {
        list[i].write(f);
    }
    f.close();
}