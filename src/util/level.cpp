#include "util/level.h"

namespace level {
std::string create_space(size_t size, size_t space) {
    return std::string(size * space, ' ').c_str();
}

} // namespace level
