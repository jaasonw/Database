#pragma once
#include "util/io/binary_util.h"
#include <cstring>

class Record {
public:
    Record() {
        record[0] = NULL;
        recno = -1;
    }

    Record(const char* str) { strcpy(record, str); }
    long write(std::fstream& outs);
    long read(std::fstream& ins, long recno);

    friend std::ostream& operator<<(std::ostream& outs, const Record& r);

private:
    static const int MAX = 10;
    int recno;
    char record[MAX];
};
long Record::write(std::fstream& outs) {
    // write to the end of the file.
    long pos = outs.tellp();

    outs.write(record, sizeof(record));

    return pos;
}
long Record::read(std::fstream& ins, long recno) {
    long pos = recno * sizeof(record);
    ins.seekg(pos, std::ios_base::beg);

    ins.read(record, sizeof(record));
    return ins.gcount();
}
std::ostream& operator<<(std::ostream& outs, const Record& r) {
    outs << r.record;
    return outs;
}

void save_list(Record list[], int count) {
    std::fstream f;
    open_fileW(f, "record_list.bin");
    for (int i = 0; i < count; i++) {
        list[i].write(f);
    }
    f.close();
}