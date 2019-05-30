#pragma once
#include <exception>
#include <fstream>
#include <iostream>
#include <stdexcept>

namespace bin_io {

bool file_exists(const char* filename);
void open_fileRW(std::fstream& f, const char* filename);
void open_fileW(std::fstream& f, const char* filename);

}