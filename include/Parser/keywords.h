#pragma once
#include "constants.h"

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
    #ifdef ENABLE_NON_STANDARD_SQL
        ,FIELDS
    #endif
};

}