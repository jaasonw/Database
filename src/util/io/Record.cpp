#include "util/io/Record.h"

long Record::write(std::fstream& outs) {
    // write to the end of the file.
    long pos = outs.tellp();

    for (int i = 0; i < ROWS; ++i) {
        outs.write(buffer[i], sizeof(buffer));
    }
    return pos;
}
long Record::read(std::fstream& ins, long recno) {
    ins.seekg(ROWS * COLS * recno);
    for (int i = 0; i < ROWS; ++i) {
        ins.read(buffer[i], sizeof(buffer));
    }
    return ins.gcount();
}
std::ostream& operator<<(std::ostream& outs, const Record& r) {
    for (int i = 0; i < r.ROWS; ++i) {
        for (int j = 0; j < r.COLS; j++) {
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