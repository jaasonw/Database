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
    // updates the state according to the new token
    // returns the new state
    int update_state(std::string token);
    // returns the current state
    int get_state() { return last_state; };
    // sets the current state to 0
    void reset_state();
    // returns true if the current state is a success state
    bool is_success();
    // returns true if the current state is a not a state
    bool is_fail();
    // returns true if the current state is an invalid state
    bool is_invalid();
    // returns true if the current state is a quote state
    bool is_quote_state(int state);
    // returns true if the current state is a parse state
    bool is_parse_state(int state);
    // returns the parse tree key associated the state
    std::string get_parse_key(int state);
};