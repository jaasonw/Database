#pragma once
#include "Token.h"
#include "state_machine/state_table.h"
#include <cstring>
#include <string>

class STokenizer {
public:
    STokenizer();
    STokenizer(const char* str);
    bool done() { return pos >= strlen(buffer); }
    bool fail() { return state == -1; };

    // extract one token (very similar to the way cin >> works)
    friend STokenizer& operator>>(STokenizer& s, string_tokenizer::Token& t);

    // set a new string as the input string
    void set_string(const char* str);

    static const int MAX_BUFFER = 255;

private:
    // constants
    static const int MAX_COLUMNS = state_machine::MAX_COLUMNS;
    static const int MAX_ROWS = state_machine::NUM_ROWS;

    // state table
    int state_table[MAX_ROWS][MAX_COLUMNS];

    // current state
    int state;

    // input string
    char buffer[MAX_BUFFER];

    // current position in the string
    int pos;

    // create table for all the tokens we will recognize
    // (e.g. doubles, words, etc.)
    void make_table(int state_table[][MAX_COLUMNS]);

    // extract the longest string that match
    // one of the acceptable token types
    bool get_token(int start_state, std::string& token);
};