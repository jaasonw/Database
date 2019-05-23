#pragma once
#include "Map/Map.h"
#include "MultiMap/MultiMap.h"
#include "Parser/SQLStateMachine.h"
#include "Parser/keywords.h"
#include "Queue/Queue.h"
#include "StringTokenizer/STokenizer.h"
#include <cstring>
#include <exception>
#include <iostream>
#include <string>

class Parser {
private:
    // the token queue
    Queue<string_tokenizer::Token> tokens;
    // the tokenizer
    STokenizer tokenizer;
    // the parse tree
    MultiMap::MultiMap<std::string, std::string> parse_tree;

    SQLStateMachine state;

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

}
void Parser::parse(std::string input) {
    set_string(input);
    for (auto it = tokens.it_begin(); it != tokens.it_end(); it++) {
        std::cout << *it << std::endl;
    }
    while (!tokens.empty()) {
        string_tokenizer::Token t = tokens.pop();
        std::string token_string = "";
        // if there's an opening quotation then interpret as full word;
        if (t.token_str() == "\"") {
            while (tokens.it_begin()->token_str != "\"") {
                // uh oh, we never found the closing quotation mark
                if (tokens.size() == 0)
                    throw std::invalid_argument("Error: no closing quote");

                if (token_string.size() > 0)
                    token_string += " ";
                token_string += tokens.pop().token_str();
            }
            // remove the closing quotation mark
            tokens.pop();
        }
    }
}
