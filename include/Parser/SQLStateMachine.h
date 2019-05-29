#pragma once
#include "Map/Map.h"
#include "Parser/keywords.h"
#include "StringTokenizer/state_machine/state_table.h"
#include "StringTokenizer/Token.h"
#include <cctype>
#include <string>

class SQLStateMachine {
private:
    // the keyword map
    Map::Map<std::string, sql_parser::Keyword> keywords;
    // the state table
    int state_table[state_machine::NUM_ROWS][state_machine::MAX_COLUMNS];
    int last_state = 0;
public:
    SQLStateMachine();
    int update_state(std::string token);
    int get_state() { return last_state; };
    void reset_state();
    bool is_success();
    bool is_fail();
    bool is_invalid();
};