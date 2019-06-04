#include "Parser/SQLStateMachine.h"

SQLStateMachine::SQLStateMachine() {
    // init keyword map
    keywords["SELECT"] = sql_parser::SELECT;
    keywords["INSERT"] = sql_parser::INSERT;
    keywords["CREATE"] = sql_parser::CREATE;
    keywords["INTO"] = sql_parser::INTO;
    keywords["FROM"] = sql_parser::FROM;
    keywords["WHERE"] = sql_parser::WHERE;
    keywords["TABLE"] = sql_parser::TABLE;
    keywords["VALUES"] = sql_parser::VALUES;
    keywords["*"] = sql_parser::ASTERISK;
    keywords[","] = sql_parser::COMMA;
    keywords["("] = sql_parser::OPEN_PARENTH;
    keywords[")"] = sql_parser::CLOSE_PARENTH;
    keywords["="] = sql_parser::RELATIONAL_OPERATOR;
    keywords[">"] = sql_parser::RELATIONAL_OPERATOR;
    keywords["<"] = sql_parser::RELATIONAL_OPERATOR;
    keywords["<="] = sql_parser::RELATIONAL_OPERATOR;
    keywords[">="] = sql_parser::RELATIONAL_OPERATOR;
    keywords["AND"] = sql_parser::LOGICAL_OPERATOR;
    keywords["OR"] = sql_parser::LOGICAL_OPERATOR;
    #ifdef ENABLE_NON_STANDARD_SQL
        keywords["MAKE"] = sql_parser::CREATE;
        keywords["FIELDS"] = sql_parser::FIELDS;
    #endif

    // init state table
    state_machine::init_table(state_table);
    state_machine::mark_fail(state_table, 0);
    state_machine::mark_fail(state_table, 1);
    state_machine::mark_fail(state_table, 2);
    state_machine::mark_fail(state_table, 3);
    state_machine::mark_fail(state_table, 4);
    state_machine::mark_fail(state_table, 5);
    state_machine::mark_success(state_table, 6);
    state_machine::mark_fail(state_table, 7);
    state_machine::mark_fail(state_table, 8);
    state_machine::mark_fail(state_table, 9);
    state_machine::mark_fail(state_table, 10);
    state_machine::mark_success(state_table, 11);
    state_machine::mark_fail(state_table, 12);
    state_machine::mark_fail(state_table, 13);
    state_machine::mark_fail(state_table, 14);
    state_machine::mark_fail(state_table, 15);
    state_machine::mark_fail(state_table, 16);
    state_machine::mark_fail(state_table, 17);
    state_machine::mark_fail(state_table, 18);
    state_machine::mark_success(state_table, 19);
    state_machine::mark_fail(state_table, 20);
    state_machine::mark_fail(state_table, 21);
    state_machine::mark_fail(state_table, 22);
    state_machine::mark_fail(state_table, 23);
    state_machine::mark_fail(state_table, 24);
    state_machine::mark_fail(state_table, 25);
    state_machine::mark_fail(state_table, 26);
    state_machine::mark_success(state_table, 27);
    state_machine::mark_fail(state_table, 28);
    #ifdef ENABLE_NON_STANDARD_SQL
        // insert command parenthesis skipping
        state_machine::mark_success(state_table, 25);
        // create command parenthesis skipping
        state_machine::mark_success(state_table, 17);
    #endif

    // mark cells
    // command: select
    state_machine::mark_cell(0, state_table, sql_parser::SELECT, 1);

    state_machine::mark_cell(1, state_table, sql_parser::ASTERISK, 2);

    state_machine::mark_cell(1, state_table, sql_parser::STRING, 3);
    state_machine::mark_cell(4, state_table, sql_parser::STRING, 3);
    state_machine::mark_cell(5, state_table, sql_parser::STRING, 6);
    state_machine::mark_cell(8, state_table, sql_parser::STRING, 9);
    state_machine::mark_cell(10, state_table, sql_parser::STRING, 11);
    state_machine::mark_cell(12, state_table, sql_parser::STRING, 9);

    state_machine::mark_cell(3, state_table, sql_parser::COMMA, 4);

    state_machine::mark_cell(2, state_table, sql_parser::FROM, 5);
    state_machine::mark_cell(3, state_table, sql_parser::FROM, 5);

    state_machine::mark_cell(6, state_table, sql_parser::WHERE, 8);

    state_machine::mark_cell(9, state_table, sql_parser::RELATIONAL_OPERATOR, 10);
    state_machine::mark_cell(11, state_table, sql_parser::LOGICAL_OPERATOR, 12);

    // command: create
    state_machine::mark_cell(0, state_table, sql_parser::CREATE, 13);

    state_machine::mark_cell(13, state_table, sql_parser::TABLE, 14);

    state_machine::mark_cell(15, state_table, sql_parser::OPEN_PARENTH, 16);
    state_machine::mark_cell(17, state_table, sql_parser::CLOSE_PARENTH, 19);

    state_machine::mark_cell(14, state_table, sql_parser::STRING, 15);
    state_machine::mark_cell(16, state_table, sql_parser::STRING, 17);
    state_machine::mark_cell(18, state_table, sql_parser::STRING, 17);

    state_machine::mark_cell(17, state_table, sql_parser::COMMA, 18);

    // command: insert
    state_machine::mark_cell(0, state_table, sql_parser::INSERT, 20);

    state_machine::mark_cell(20, state_table, sql_parser::INTO, 21);

    state_machine::mark_cell(21, state_table, sql_parser::STRING, 22);
    state_machine::mark_cell(24, state_table, sql_parser::STRING, 25);
    state_machine::mark_cell(26, state_table, sql_parser::STRING, 25);
    // state_machine::mark_cell(27, state_table, sql_parser::STRING, 26);

    state_machine::mark_cell(23, state_table, sql_parser::OPEN_PARENTH, 24);
    state_machine::mark_cell(24, state_table, sql_parser::CLOSE_PARENTH, 27);
    state_machine::mark_cell(25, state_table, sql_parser::CLOSE_PARENTH, 27);

    state_machine::mark_cell(25, state_table, sql_parser::COMMA, 26);

    state_machine::mark_cell(22, state_table, sql_parser::VALUES, 23);

    #ifdef ENABLE_NON_STANDARD_SQL
        // insert command parenthesis skipping
        state_machine::mark_cell(23, state_table, sql_parser::STRING, 25);
        // create command field keyword
        state_machine::mark_cell(15, state_table, sql_parser::FIELDS, 28);
        state_machine::mark_cell(28, state_table, sql_parser::STRING, 17);
    #endif
}

int SQLStateMachine::update_state(std::string token) {
    sql_parser::Keyword type = sql_parser::STRING;
    // uppercase the token
    for (size_t i = 0; i < token.size(); i++) {
        token[i] = toupper(token[i]);
    }
    if (keywords.contains(token)) {
        type = keywords[token];
    }
    last_state = state_table[last_state][type];
    return last_state;
}
void SQLStateMachine::reset_state() { last_state = 0; }
bool SQLStateMachine::is_success() {
    return state_machine::is_success(state_table, last_state);
}
bool SQLStateMachine::is_fail() {
    return !state_machine::is_success(state_table, last_state);
}
bool SQLStateMachine::is_invalid() { return last_state == -1; }