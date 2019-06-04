#include "util/string_util.h"

namespace string_util {
// maybe write this later
std::vector<std::string> split(std::string str, char delim) {}

std::string uppercase(std::string str) {
    for (size_t i = 0; i < str.size(); i++) {
        str[i] = toupper(str[i]);
    }
    return str;
}

} // namespace string_util
