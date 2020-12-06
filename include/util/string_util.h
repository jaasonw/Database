#pragma once
#include <string>
#include <vector>

namespace string_util {
    // returns a vector of substrings divided by a delimiter
    std::vector<std::string> split(const std::string& str, const char& delim);

    // returns an uppercased version of the string
    std::string uppercase(const std::string& str);

    // returns true of the string consists of whitespace
    bool iswhitespace(const std::string& str);
} // namespace string_util
