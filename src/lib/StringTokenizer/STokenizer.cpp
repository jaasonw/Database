#include "STokenizer.h"

STokenizer::STokenizer() : pos(0), buffer(""), state(0) {
    make_table(state_table);
}

STokenizer::STokenizer(char* str) : pos(0), state(0) {
    make_table(state_table);
    strcpy(buffer, str);
}

void STokenizer::make_table(int state_table[][MAX_COLUMNS]) {
    const char* ALPHA = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    const char* NUMBERS = "1234567890";
    const char* PUNCTUATION = ",\\.?!;\"'()[]{}-/";
    const char* WHITESPACE = " \t\n\r";

    state_machine::init_table(state_table);

    // define successs states
    state_machine::mark_fail(state_table,       0);
    state_machine::mark_success(state_table,    1);
    state_machine::mark_success(state_table,    2);
    state_machine::mark_success(state_table,    3);
    state_machine::mark_fail(state_table,       4);
    state_machine::mark_success(state_table,    5);
    state_machine::mark_fail(state_table,       6);
    state_machine::mark_success(state_table,    7);
    state_machine::mark_fail(state_table,       8);
    state_machine::mark_success(state_table,    9);
    state_machine::mark_success(state_table,    10);

    // mark alphabet
    state_machine::mark_cells(0, state_table, ALPHA, 1);
    state_machine::mark_cells(1, state_table, ALPHA, 1);
    state_machine::mark_cells(2, state_table, ALPHA, -1);
    state_machine::mark_cells(3, state_table, ALPHA, -1);
    state_machine::mark_cells(4, state_table, ALPHA, -1);
    state_machine::mark_cells(5, state_table, ALPHA, -1);
    state_machine::mark_cells(6, state_table, ALPHA, 1);
    
    // mark spaces
    state_machine::mark_cells(0, state_table, WHITESPACE, 2);
    state_machine::mark_cells(1, state_table, WHITESPACE, -1);
    state_machine::mark_cells(2, state_table, WHITESPACE, 2);

    // mark numbers
    state_machine::mark_cells(0, state_table, NUMBERS, 3);
    state_machine::mark_cells(1, state_table, NUMBERS, 1);
    state_machine::mark_cells(2, state_table, NUMBERS, -1);
    state_machine::mark_cells(3, state_table, NUMBERS, 3);
    state_machine::mark_cells(4, state_table, NUMBERS, 3);
    // ...
    state_machine::mark_cells(10, state_table, NUMBERS, 10);

    // mark punctuation
    state_machine::mark_cells(0, state_table, PUNCTUATION, 5);

    // mark all cases with "." (decimals, ellipis, period)
    state_machine::mark_cell(0, state_table, '.', 5);
    state_machine::mark_cell(1, state_table, '.', -1);
    state_machine::mark_cell(2, state_table, '.', -1);
    state_machine::mark_cell(3, state_table, '.', 10);
    state_machine::mark_cell(4, state_table, '.', -1);
    state_machine::mark_cell(5, state_table, '.', 8);
    state_machine::mark_cell(6, state_table, '.', -1);
    state_machine::mark_cell(7, state_table, '.', -1);
    state_machine::mark_cell(8, state_table, '.', 9);

    // mark apostrophe cases
    state_machine::mark_cell(0, state_table, '\'', 6);
    state_machine::mark_cell(1, state_table, '\'', 6);
}

bool STokenizer::get_token(int start_state, std::string& token) {
    int last_pos = pos;
    bool has_token = state_machine::get_token(buffer, pos, state_table, start_state);
    char _token[MAX_BUFFER] = "";
    strncpy(_token, buffer + last_pos, pos + 1 - last_pos);
    token = _token;
    pos += 1;
    return has_token;
}

void STokenizer::set_string(char* str) {
    // reset pos and state
    pos = 0;
    state = 0;
    strcpy(buffer, str);
}

STokenizer& operator>>(STokenizer& s, Token& t) {
    std::string str;
    s.get_token(s.state, str);
    int _state = 0;
    int _pos = 0;
    state_machine::get_token(str.c_str(), _pos, s.state_table, _state);
    t.set_type(_state);
    t.set_string(str);
    return s;
}