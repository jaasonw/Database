#pragma once
#include "Map/Map.h"
#include "Parser/keywords.h"
#include "StringTokenizer/Token.h"
#include "StringTokenizer/state_machine/state_table.h"
#include "constants.h"
#include "util/string_util.h"
#include <cctype>
#include <string>

class SQLStateMachine {
private:
    // the keyword map
    Map::Map<std::string, sql_parser::Keyword> keywords;
    // the state table
    int state_table[state_machine::NUM_ROWS][state_machine::MAX_COLUMNS];
    int last_state = 0;
    Map::Map<int, std::string> parse_states;
    BPlusTree<int> quote_states;
public:
    SQLStateMachine();
    int update_state(std::string token);
    int get_state() { return last_state; };
    void reset_state();
    bool is_success();
    bool is_fail();
    bool is_invalid();
    bool is_quote_state(int state);
    bool is_parse_state(int state);
    std::string get_parse_key(int state);
};