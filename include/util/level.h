#pragma once
#include <string>

namespace level {

// used for printing a tree, returns a string containing size * space number of
// spaces
std::string create_space(size_t size, size_t space = 8);

} // namespace level
