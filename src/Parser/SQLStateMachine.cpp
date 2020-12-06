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
    keywords["DROP"] = sql_parser::DROP;
    keywords["TABLES"] = sql_parser::LIST_TABLES;
    keywords["EXIT"] = sql_parser::EXIT;
    keywords["EXEC"] = sql_parser::EXEC;
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
    keywords["\""] = sql_parser::QUOTE;
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
    state_machine::mark_fail(state_table, 29);
    state_machine::mark_fail(state_table, 30);
    state_machine::mark_success(state_table, 31);
    state_machine::mark_success(state_table, 32);
    state_machine::mark_fail(state_table, 33);
    state_machine::mark_fail(state_table, 34);
    state_machine::mark_fail(state_table, 35);
    state_machine::mark_fail(state_table, 36);
    state_machine::mark_fail(state_table, 37);
    state_machine::mark_fail(state_table, 38);
    state_machine::mark_fail(state_table, 39);
    state_machine::mark_fail(state_table, 40);
    state_machine::mark_fail(state_table, 41);
    state_machine::mark_fail(state_table, 42);
    state_machine::mark_fail(state_table, 43);
    state_machine::mark_fail(state_table, 44);
    state_machine::mark_fail(state_table, 45);
    state_machine::mark_fail(state_table, 46);
    state_machine::mark_fail(state_table, 47);
    state_machine::mark_fail(state_table, 48);
    state_machine::mark_fail(state_table, 49);
    state_machine::mark_fail(state_table, 50);
    state_machine::mark_success(state_table, 51);

    // this wont make any sense unless you look at the state diagram or the
    // state table spreadsheet

    // define states
    // command states
    // select
    parse_states[1] ="command";
    // create
    parse_states[13] = "command";
    // insert
    parse_states[20] = "command";
    // drop
    parse_states[29] = "command";
    // any 1 word commands i might want to add
    parse_states[32] = "command";
    // exec
    parse_states[50] = "command";

    // field states
    // select: asterisk
    parse_states[2] = "fields";
    // select: string
    parse_states[3] = "fields";
    // create: field name
    parse_states[17] = "fields";
    // insert: entry name
    parse_states[25] = "fields";
    // exec: filename
    parse_states[51] = "fields";

    // table name states
    // select: table name
    parse_states[6] = "table_name";
    // create: table name
    parse_states[15] = "table_name";
    // insert: table name
    parse_states[22] = "table_name";
    // drop: table name
    parse_states[31] = "table_name";

    // where states
    // field name
    parse_states[9] = "where";
    // relational
    parse_states[10] = "where";
    // entry name
    parse_states[11] = "where";
    // logical
    parse_states[12] = "where";

    // quote states
    quote_states.insert(33);
    quote_states.insert(34);
    quote_states.insert(35);
    quote_states.insert(36);
    quote_states.insert(37);
    quote_states.insert(38);
    quote_states.insert(39);
    quote_states.insert(40);
    quote_states.insert(41);
    quote_states.insert(42);
    quote_states.insert(43);
    quote_states.insert(44);
    quote_states.insert(45);
    quote_states.insert(46);
    quote_states.insert(47);
    quote_states.insert(48);
    quote_states.insert(49);

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

    // handle all these quotes
    state_machine::mark_cell(5, state_table, sql_parser::QUOTE, 33);

    state_machine::mark_all(33, state_table, 34);
    state_machine::unmark_cell(33, state_table, sql_parser::QUOTE);

    state_machine::mark_all(34, state_table, 34);
    state_machine::unmark_cell(34, state_table, sql_parser::QUOTE);

    state_machine::mark_cell(34, state_table, sql_parser::QUOTE, 6);


    state_machine::mark_cell(8, state_table, sql_parser::QUOTE, 35);
    state_machine::mark_cell(12, state_table, sql_parser::QUOTE, 35);

    state_machine::mark_all(35, state_table, 36);
    state_machine::unmark_cell(35, state_table, sql_parser::QUOTE);

    state_machine::mark_all(36, state_table, 36);
    state_machine::unmark_cell(36, state_table, sql_parser::QUOTE);

    state_machine::mark_cell(36, state_table, sql_parser::QUOTE, 9);


    state_machine::mark_cell(10, state_table, sql_parser::QUOTE, 37);

    state_machine::mark_all(37, state_table, 38);
    state_machine::unmark_cell(37, state_table, sql_parser::QUOTE);

    state_machine::mark_all(38, state_table, 38);
    state_machine::unmark_cell(38, state_table, sql_parser::QUOTE);

    state_machine::mark_cell(38, state_table, sql_parser::QUOTE, 11);


    // command: create
    state_machine::mark_cell(0, state_table, sql_parser::CREATE, 13);

    state_machine::mark_cell(13, state_table, sql_parser::TABLE, 14);

    state_machine::mark_cell(15, state_table, sql_parser::OPEN_PARENTH, 16);
    state_machine::mark_cell(17, state_table, sql_parser::CLOSE_PARENTH, 19);

    state_machine::mark_cell(14, state_table, sql_parser::STRING, 15);
    state_machine::mark_cell(16, state_table, sql_parser::STRING, 17);
    state_machine::mark_cell(18, state_table, sql_parser::STRING, 17);

    state_machine::mark_cell(17, state_table, sql_parser::COMMA, 18);

    // quotes
    state_machine::mark_cell(13, state_table, sql_parser::QUOTE, 40);

    state_machine::mark_all(40, state_table, 41);
    state_machine::unmark_cell(40, state_table, sql_parser::QUOTE);

    state_machine::mark_all(41, state_table, 41);
    state_machine::unmark_cell(41, state_table, sql_parser::QUOTE);

    state_machine::mark_cell(41, state_table, sql_parser::QUOTE, 15);


    state_machine::mark_cell(15, state_table, sql_parser::QUOTE, 42);
    state_machine::mark_cell(16, state_table, sql_parser::QUOTE, 42);
    state_machine::mark_cell(18, state_table, sql_parser::QUOTE, 42);

    state_machine::mark_all(42, state_table, 43);
    state_machine::unmark_cell(42, state_table, sql_parser::QUOTE);

    state_machine::mark_all(43, state_table, 43);
    state_machine::unmark_cell(43, state_table, sql_parser::QUOTE);

    state_machine::mark_cell(43, state_table, sql_parser::QUOTE, 17);

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

    // quotes
    state_machine::mark_cell(20, state_table, sql_parser::QUOTE, 44);

    state_machine::mark_all(44, state_table, 45);
    state_machine::unmark_cell(44, state_table, sql_parser::QUOTE);

    state_machine::mark_all(45, state_table, 45);
    state_machine::unmark_cell(45, state_table, sql_parser::QUOTE);

    state_machine::mark_cell(45, state_table, sql_parser::QUOTE, 22);


    state_machine::mark_cell(23, state_table, sql_parser::QUOTE, 46);
    state_machine::mark_cell(24, state_table, sql_parser::QUOTE, 46);
    state_machine::mark_cell(26, state_table, sql_parser::QUOTE, 46);

    state_machine::mark_all(46, state_table, 47);
    state_machine::unmark_cell(46, state_table, sql_parser::QUOTE);

    state_machine::mark_all(47, state_table, 47);
    state_machine::unmark_cell(47, state_table, sql_parser::QUOTE);

    state_machine::mark_cell(47, state_table, sql_parser::QUOTE, 25);

    // command: drop
    state_machine::mark_cell(0, state_table, sql_parser::DROP, 29);
    state_machine::mark_cell(29, state_table, sql_parser::TABLE, 30);
    state_machine::mark_cell(30, state_table, sql_parser::STRING, 31);

    state_machine::mark_cell(20, state_table, sql_parser::QUOTE, 48);

    state_machine::mark_all(48, state_table, 49);
    state_machine::unmark_cell(48, state_table, sql_parser::QUOTE);

    state_machine::mark_all(49, state_table, 49);
    state_machine::unmark_cell(49, state_table, sql_parser::QUOTE);

    state_machine::mark_cell(49, state_table, sql_parser::QUOTE, 31);

    // list tables
    state_machine::mark_cell(0, state_table, sql_parser::LIST_TABLES, 32);
    // exit
    state_machine::mark_cell(0, state_table, sql_parser::EXIT, 32);

    // execute file
    state_machine::mark_cell(0, state_table, sql_parser::EXEC, 50);
    state_machine::mark_all(50, state_table, 51);
    state_machine::mark_all(51, state_table, 51);


    #ifdef ENABLE_NON_STANDARD_SQL
        // insert command parenthesis skipping
        state_machine::mark_cell(23, state_table, sql_parser::STRING, 25);
        // create command field keyword
        state_machine::mark_cell(15, state_table, sql_parser::FIELDS, 28);
        state_machine::mark_cell(28, state_table, sql_parser::STRING, 17);
    #endif
}

int SQLStateMachine::update_state(const std::string& token) {
    sql_parser::Keyword type = sql_parser::STRING;
    // uppercase the token
    std::string _token = string_util::uppercase(token);
    if (keywords.contains(_token)) {
        type = keywords[_token];
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

bool SQLStateMachine::is_quote_state(int state) {
    return quote_states.search(state) != nullptr;
}
bool SQLStateMachine::is_parse_state(int state) {
    return parse_states.contains(state);
}
std::string SQLStateMachine::get_parse_key(int state) {
    if (parse_states.contains(state))
        return parse_states[state];
    else
        return "";
}