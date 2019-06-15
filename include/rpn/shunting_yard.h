#pragma once
#include "Map/Map.h"
#include "Queue/Queue.h"
#include "Stack/Stack.h"
#include "util/string_util.h"
#include <string>
#include <vector>

// Converts a given vector of tokens in infix notation into a queue of tokens
// in reverse polish notation (postfix)
Queue<std::string> infix_to_postfix(const std::vector<std::string>& infix);