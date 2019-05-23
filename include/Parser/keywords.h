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
    RELATIONAL_OPERATOR,
    LOGICAL_OPERATOR,
    ASTERISK,
    INTO,
    VALUES,
    FROM,
    COMMA,
    WHERE,
    TABLE,
    OPEN_PARENTH,
    CLOSE_PARENTH
};

}