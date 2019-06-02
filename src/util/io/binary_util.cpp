#include "util/io/binary_util.h"

namespace bin_io {

bool file_exists(const char* filename) {
    const bool debug = false;
    std::fstream ff;
    ff.open(filename, std::fstream::in | std::fstream::binary);
    if (ff.fail()) {
        if (debug)
            std::cout << "file_exists(): File does NOT exist: " << filename
                      << std::endl;
        return false;
    }
    if (debug)
        std::cout << "file_exists(): File DOES exist: " << filename
                  << std::endl;
    ff.close();
    return true;
}

void open_fileRW(std::fstream& f, const char* filename) {
    const bool debug = false;
    // openning a nonexistent file for in|out|app causes a fail()
    //  so, if the file does not exist, create it by openning it for
    //  output:
    if (!file_exists(filename)) {
        f.open(filename, std::fstream::out | std::fstream::binary | std::ios::app);
        if (f.fail()) {
            std::cout << "file open (RW) failed: with out|" << filename << "]"
                      << std::endl;
            throw std::runtime_error("file RW failed  ");
        } else {
            if (debug)
                std::cout << "open_fileRW: file created successfully: "
                          << filename << std::endl;
        }
    } else {
        f.open(filename,
               std::fstream::in | std::fstream::out | std::fstream::binary | std::ios::app);
        if (f.fail()) {
            std::cout << "file open (RW) failed. [" << filename << "]"
                      << std::endl;
            throw std::runtime_error("file failed to open.");
        }
    }
}
void open_fileW(std::fstream& f, const char filename[]) {
    f.open(filename, std::fstream::out | std::fstream::binary | std::ios::app);
    if (f.fail()) {
        std::cout << "file open failed: " << filename << std::endl;
        throw std::runtime_error("file failed to open.");
    }
}

} // namespace bin_io