#pragma once
#include <exception>
#include <fstream>
#include <iostream>
#include <stdexcept>

namespace binary_fio {

bool file_exists(const char* filename);
void open_fileRW(std::fstream& f, const char* filename) throw(char*);
void open_fileW(std::fstream& f, const char* filename);

}