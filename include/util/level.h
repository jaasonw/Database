#pragma once
#include <string>

namespace level {
const int SPACING = 32;
std::string create_space(size_t size) {
    return std::string(size * SPACING, ' ').c_str();
}
}
