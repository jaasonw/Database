#pragma once
#include "Map/Map.h"
#include "Parser/keywords.h"
#include "StringTokenizer/state_machine/state_table.h"
#include "StringTokenizer/Token.h"
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

SQLStateMachine::SQLStateMachine() {
    // init keyword map
    keywords["select"] = sql_parser::SELECT;
    keywords["insert"] = sql_parser::INSERT;
    keywords["create"] = sql_parser::CREATE;
    keywords["="] = sql_parser::RELATIONAL_OPERATOR;
    keywords[">"] = sql_parser::RELATIONAL_OPERATOR;
    keywords["<"] = sql_parser::RELATIONAL_OPERATOR;
    keywords["<="] = sql_parser::RELATIONAL_OPERATOR;
    keywords[">="] = sql_parser::RELATIONAL_OPERATOR;
    keywords["and"] = sql_parser::LOGICAL_OPERATOR;
    keywords["or"] = sql_parser::LOGICAL_OPERATOR;
    keywords["*"] = sql_parser::ASTERISK;
    keywords["into"] = sql_parser::INTO;
    keywords["values"] = sql_parser::VALUES;
    keywords["from"] = sql_parser::FROM;
    keywords[","] = sql_parser::COMMA;

    // init state table
    state_machine::init_table(state_table);
    state_machine::mark_fail(state_table,       0);
    state_machine::mark_fail(state_table,       1);
    state_machine::mark_fail(state_table,       2);
    state_machine::mark_fail(state_table,       3);
    state_machine::mark_fail(state_table,       4);
    state_machine::mark_fail(state_table,       5);
    state_machine::mark_success(state_table,    6);
    state_machine::mark_fail(state_table,       7);
    state_machine::mark_fail(state_table,       8);
    state_machine::mark_fail(state_table,       9);
    state_machine::mark_fail(state_table,       10);
    state_machine::mark_success(state_table,    11);
    state_machine::mark_fail(state_table,       12);
    state_machine::mark_fail(state_table,       13);
    state_machine::mark_fail(state_table,       14);
    state_machine::mark_fail(state_table,       15);
    state_machine::mark_fail(state_table,       16);
    state_machine::mark_success(state_table,    17);
    state_machine::mark_fail(state_table,       18);
    state_machine::mark_fail(state_table,       19);
    state_machine::mark_fail(state_table,       20);
    state_machine::mark_fail(state_table,       21);
    state_machine::mark_fail(state_table,       22);
    state_machine::mark_fail(state_table,       23);
    state_machine::mark_fail(state_table,       24);
    state_machine::mark_fail(state_table,       25);
    state_machine::mark_fail(state_table,       26);
    state_machine::mark_fail(state_table,       27);
    state_machine::mark_success(state_table,    28);

    // mark cells
    // command: select
    state_machine::mark_cell(0, state_table, sql_parser::SELECT, 1);

    state_machine::mark_cell(1, state_table, sql_parser::ASTERISK, 2);

    state_machine::mark_cell(1, state_table, sql_parser::STRING, 3);
    state_machine::mark_cell(4, state_table, sql_parser::STRING, 3);
    state_machine::mark_cell(5, state_table, sql_parser::STRING, 6);
    state_machine::mark_cell(7, state_table, sql_parser::STRING, 6);
    state_machine::mark_cell(8, state_table, sql_parser::STRING, 9);
    state_machine::mark_cell(10, state_table, sql_parser::STRING, 11);

    state_machine::mark_cell(3, state_table, sql_parser::COMMA, 4);
    state_machine::mark_cell(6, state_table, sql_parser::COMMA, 7);
    state_machine::mark_cell(26, state_table, sql_parser::COMMA, 27);

    state_machine::mark_cell(2, state_table, sql_parser::FROM, 5);
    state_machine::mark_cell(3, state_table, sql_parser::FROM, 5);

    state_machine::mark_cell(6, state_table, sql_parser::WHERE, 8);

    state_machine::mark_cell(9, state_table, sql_parser::RELATIONAL_OPERATOR, 10);


    // command: create
    state_machine::mark_cell(0, state_table, sql_parser::CREATE, 12);

    state_machine::mark_cell(12, state_table, sql_parser::TABLE, 13);

    state_machine::mark_cell(14, state_table, sql_parser::OPEN_PARENTH, 15);
    state_machine::mark_cell(15, state_table, sql_parser::CLOSE_PARENTH, 17);

    state_machine::mark_cell(13, state_table, sql_parser::STRING, 14);
    state_machine::mark_cell(15, state_table, sql_parser::STRING, 16);

    state_machine::mark_cell(16, state_table, sql_parser::COMMA, 15);


    // command: insert
    state_machine::mark_cell(0, state_table, sql_parser::CREATE, 18);

    state_machine::mark_cell(18, state_table, sql_parser::INTO, 19);

    state_machine::mark_cell(19, state_table, sql_parser::STRING, 20);
    state_machine::mark_cell(21, state_table, sql_parser::STRING, 22);
    state_machine::mark_cell(25, state_table, sql_parser::STRING, 26);
    state_machine::mark_cell(27, state_table, sql_parser::STRING, 26);

    state_machine::mark_cell(20, state_table, sql_parser::OPEN_PARENTH, 21);
    state_machine::mark_cell(22, state_table, sql_parser::CLOSE_PARENTH, 23);
    
    state_machine::mark_cell(24, state_table, sql_parser::OPEN_PARENTH, 25);
    state_machine::mark_cell(26, state_table, sql_parser::CLOSE_PARENTH, 28);

    state_machine::mark_cell(22, state_table, sql_parser::COMMA, 21);
    state_machine::mark_cell(22, state_table, sql_parser::COMMA, 21);
    
    state_machine::mark_cell(23, state_table, sql_parser::VALUES, 24);
}

int SQLStateMachine::update_state(std::string token) {
    sql_parser::Keyword type = sql_parser::STRING;
    // lowercase the token
    for (size_t i = 0; i < token.size(); i++) {
        token[i] = tolower(token[i]);
    }
    if (keywords.contains(token)) {
        type = keywords[token];
    }
    last_state = state_table[last_state][type];
    return last_state;
}
void SQLStateMachine::reset_state() {
    last_state = 0;
}
bool SQLStateMachine::is_success() {
    return state_machine::is_success(state_table, last_state);
}
bool SQLStateMachine::is_fail() {
    return !state_machine::is_success(state_table, last_state);
}
bool SQLStateMachine::is_invalid() {
    return last_state == -1;
}