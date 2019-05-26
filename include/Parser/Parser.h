#pragma once
#include "Map/Map.h"
#include "MultiMap/MultiMap.h"
#include "Parser/SQLStateMachine.h"
#include "Parser/keywords.h"
#include "Queue/Queue.h"
#include "StringTokenizer/STokenizer.h"
#include <cstring>
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
    // the parse tree
    MultiMap::MultiMap<std::string, std::string> parse_tree;

    SQLStateMachine sql_state;

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
        if (!isspace(t.token_str()[0]))
            tokens.push(t);
    }
}

void Parser::init() {

}
void Parser::parse(std::string input) {
    set_string(input);
    sql_state.reset_state();
    // for (auto it = tokens.it_begin(); it != tokens.it_end(); it++) {
    //     std::cout << *it << std::endl;
    // }
    // select * from student
    int last_state = sql_state.get_state();
    while (!tokens.empty()) {
        string_tokenizer::Token t = tokens.pop();
        std::string token_string = "";
        // place holder code that just takes the token and puts it in string
        token_string = t.token_str();


        // if there's an opening quotation then interpret as full word;
        // if (t.token_str() == "\"") {
        //     while (tokens.it_begin()->token_str != "\"") {
        //         // uh oh, we never found the closing quotation mark
        //         if (tokens.size() == 1)
        //             throw std::invalid_argument("Error: no closing quote");
        //         if (token_string.size() > 0)
        //             token_string += " ";
        //         token_string += tokens.pop().token_str();
        //     }
        //     // remove the closing quotation mark
        //     tokens.pop();
        // }
        
        last_state = sql_state.update_state(token_string);
        // this wont make any sense unless you look at the state diagram or the
        // state table spreadsheet
        switch (last_state) {
            case -1:
                throw std::runtime_error("Syntax Error at token: " + token_string);
                break;
            case 1:
            case 12:
            case 18:
                parse_tree["command"] += token_string;
                break;
            case 2:
            case 3:
            case 16:
                parse_tree["fields"] += token_string;
                break;
            case 6:
            case 14:
            case 20:
                parse_tree["table_name"] += token_string;
                break;
        }
    }
    if (sql_state.is_invalid() || !sql_state.is_success()) {
        throw std::runtime_error("Error: unexpected end of input");
    }
    std::cout << parse_tree << std::endl;
}
