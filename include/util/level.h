#pragma once
#include <string>

namespace level {
std::string create_space(size_t size, size_t space = 8) {
    return std::string(size * space, ' ').c_str();
}
}
