#pragma once
#include <string>
#include <vector>

namespace string_util {
    // maybe write this later
    std::vector<std::string> split(std::string str, char delim);
    std::string uppercase(std::string str);
    bool iswhitespace(std::string str);
} // namespace string_util
