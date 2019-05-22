#pragma once
#include "Map/Map.h"
#include <string>

namespace sql_parser {

enum Command {
    ZERO,
    SELECT,
    INSERT,
    CREATE,
    VALUE,
    RELATIONAL_OPERATOR,
    LOGICAL_OPERATOR,
    ASTERISK
};

}