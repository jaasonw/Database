#pragma once
#include <exception>
#include <fstream>
#include <iostream>
#include <stdexcept>

namespace bin_io {

// returns true of a file exists
bool file_exists(const char* filename);
// opens f in read write binary mode
void open_fileRW(std::fstream& f, const char* filename);
// opens f in write binary mode
void open_fileW(std::fstream& f, const char* filename);

}