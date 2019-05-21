#pragma once
#include "Queue/Queue.h"
#include "StringTokenizer/STokenizer.h"
#include <string>

class Parser {
private:
    Queue<string_tokenizer::Token> tokens;
public:
    Parser();
};

Parser::Parser() {}
