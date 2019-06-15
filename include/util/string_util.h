#pragma once
#include <string>
#include <vector>

namespace string_util {
    // maybe write this later
    std::vector<std::string> split(std::string str, char delim);

    // returns an uppercased version of the string
    std::string uppercase(std::string str);

    // returns true of the string consists of whitespace
    bool iswhitespace(std::string str);
} // namespace string_util
