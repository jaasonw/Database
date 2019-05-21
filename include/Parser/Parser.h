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
}
void Parser::parse(std::string input) {
    set_string(input);
    for (auto it = tokens.it_begin(); it != tokens.it_end(); it++) {
        std::cout << *it << std::endl;
    }
    // std::cout << tokens;
}
