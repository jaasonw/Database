#include "util/io/Record.h"

Record::Record() {
    clear_buffer();
    record_number = -1;
}

long Record::write(std::fstream& outs) {
    // write to the end of the file.
    long pos = outs.tellp();

    for (int i = 0; i < ROWS; ++i) {
        outs.write(buffer[i], COLS);
    }
    // return the position the block was just writen to
    return pos;
}
long Record::read(std::fstream& ins, long record_number) {
    clear_buffer();
    this->record_number = record_number;
    ins.seekg(ROWS * COLS * record_number);
    for (int i = 0; i < ROWS; ++i) {
        ins.read(buffer[i], COLS);
    }
    return ins.gcount();
}
std::ostream& operator<<(std::ostream& outs, const Record& r) {
    for (int i = 0; i < r.ROWS; ++i) {
        for (int j = 0; j < r.COLS; ++j) {
            outs << r.buffer[i][j];
        }
        outs << '\n';
    }
    return outs;
}

bool Record::write_row(const char* str) {
    for (int i = 0; i < ROWS; ++i) {
        if (buffer[i][0] == '\0') {
            strcpy(buffer[i], str);
            return true;
        }
    }
    return false;
}

std::vector<std::string> Record::to_vector() {
    std::vector<std::string> row;
    for (int i = 0; i < ROWS && buffer[i][0] != '\0'; ++i) {
        char field[COLS] = "";
        strcpy(field, buffer[i]);
        row.push_back(field);
    }
    return row;
}

void Record::clear_buffer() {
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            buffer[i][j] = '\0';
        }
    }
}