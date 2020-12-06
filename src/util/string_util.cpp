#include "util/string_util.h"

namespace string_util {
// returns a vector of substrings divided by a delimiter
std::vector<std::string> split(const std::string& str, const char& delim) {
    std::vector<std::string> result;
    std::string token = "";
    for (size_t i = 0; i < str.length(); ++i) {
        if (str[i] != delim) {
            token += str[i];
        }
        else {
            result.push_back(token);
            token = "";
        }
    }
    return result;
}

std::string uppercase(const std::string& str) {
    std::string _str = str;
    for (size_t i = 0; i < str.size(); ++i) {
        _str[i] = toupper(str[i]);
    }
    return _str;
}
bool iswhitespace(const std::string& str) {
    for (char c : str) {
        if (!isspace(c))
            return false;
    }
    return true;
}

} // namespace string_util
