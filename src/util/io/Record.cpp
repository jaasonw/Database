#include "util/io/Record.h"

Record::Record() {
    clear_buffer();
    record_number = -1;
}

long Record::append_to_file(std::fstream& outs) {
    // write to the end of the file.
    outs.seekp(0, std::ios::end);
    long pos = outs.tellp();

    for (int i = 0; i < ROWS; i++) {
        outs.write(buffer[i], COLS);
    }

    return pos;
}

void Record::write(std::fstream& outs, long index) {
    outs.seekp(ROWS * COLS * index, std::ios::beg);

    for (int i = 0; i < ROWS; ++i) {
        outs.write(buffer[i], COLS);
    }
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
void Record::create_from_vector(const std::vector<std::string>& items) {
    clear_buffer();
    if (items.size() > COLS) {
        throw std::runtime_error("Error: max number of items exceeded");
    }
    for (size_t i = 0; i < items.size(); ++i) {
        write_row(items.at(i).c_str());
    }
}