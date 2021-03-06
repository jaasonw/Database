#pragma once
#include "Map/Map.h"
#include "MultiMap/MultiMap.h"
#include "Parser/SQLStateMachine.h"
#include "Parser/keywords.h"
#include "Queue/Queue.h"
#include "StringTokenizer/STokenizer.h"
#include <cctype>
#include <exception>
#include <iostream>
#include <string>

class Parser {
private:
    // the token queue
    Queue<string_tokenizer::Token> tokens;
    // the tokenizer
    STokenizer tokenizer;

    SQLStateMachine sql_state;

    // tokenizes the input string into the token queue
    void set_string(const std::string& input);

    // syntax error message constants
    const char* NO_CLOSING_QUOTE = "Syntax Error: no closing quote";
    const char* UNEXPECTED_TOKEN = "Syntax Error: unexpected token: ";
    const char* END_OF_INPUT = "Syntax Error: unexpected end of input";
public:
    Parser() { set_string(""); };
    Parser(const std::string& input) { set_string(input); };
    // converts the command into a parse tree, uppercases commands
    MultiMap::MultiMap<std::string, std::string> parse(const std::string& input);
};