#pragma once
#include "Map/Map.h"
#include <string>

namespace sql_parser {

enum Keyword {
    ZERO,
    SELECT,
    INSERT,
    CREATE,
    STRING,
    INTO,
    FROM,
    WHERE,
    TABLE,
    VALUES,
    ASTERISK,
    COMMA,
    OPEN_PARENTH,
    CLOSE_PARENTH,
    RELATIONAL_OPERATOR,
    LOGICAL_OPERATOR
};

}