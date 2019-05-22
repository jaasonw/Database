#pragma once
#include "Map/Map.h"
#include "MultiMap/MultiMap.h"
#include "Queue/Queue.h"
#include "StringTokenizer/STokenizer.h"
#include "Parser/keywords.h"
#include <cstring>
#include <string>
#include <iostream>

class Parser {
private:
    // the token queue
    Queue<string_tokenizer::Token> tokens;
    // the tokenizer
    STokenizer tokenizer;
    // the parse tree
    MultiMap::MultiMap<std::string, std::string> parse_tree;
    // the keyword map
    Map::Map<std::string, sql_parser::Command> keywords;

    // state table stuff
    int state_table[state_machine::NUM_ROWS][state_machine::MAX_COLUMNS];

    // init
    void init();
    void set_string(std::string input);
public:
    Parser(std::string input);
    // converts the command into a parse tree
    void parse(std::string input);
};

Parser::Parser(std::string input) {
    set_string(input);
    init();
}
void Parser::set_string(std::string input) {
    tokens.clear();
    tokenizer.set_string(input.c_str());
    string_tokenizer::Token t;
    while (!tokenizer.done()) {
        tokenizer >> t;
        if (t.type_string() != "SPACE")
            tokens.push(t);
    }
}

void Parser::init() {
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
    state_machine::mark_fail(state_table, 11);

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
}
void Parser::parse(std::string input) {
    set_string(input);
    for (auto it = tokens.it_begin(); it != tokens.it_end(); it++) {
        std::cout << *it << std::endl;
    }
    // std::cout << tokens;
}
