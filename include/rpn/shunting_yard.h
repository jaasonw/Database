#pragma once
#include "Map/Map.h"
#include "Queue/Queue.h"
#include "Stack/Stack.h"
#include "util/string_util.h"
#include <string>
#include <vector>

Queue<std::string> infix_to_postfix(std::vector<std::string> infix);