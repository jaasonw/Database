#pragma once
#include "Queue/Queue.h"
#include "StringTokenizer/STokenizer.h"
#include <cstring>
#include <string>

class Parser {
private:
    Queue<string_tokenizer::Token> tokens;
    STokenizer tokenizer;
    void parse();
public:
    Parser(std::string input);
    void set_string(std::string input);
};

Parser::Parser(std::string input) {
    set_string(input);
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
