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

    void set_string(std::string input);
    // combines words in between quotes into 1 token
    void combine_quotes();
public:
    Parser(std::string input);
    // converts the command into a parse tree
    MultiMap::MultiMap<std::string, std::string> parse(std::string input);
};