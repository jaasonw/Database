#include "Parser/Parser.h"

void Parser::set_string(std::string input) {
    tokens.clear();
    tokenizer.set_string(input.c_str());
    string_tokenizer::Token t;
    while (!tokenizer.done()) {
        tokenizer >> t;
        tokens.push(t);
    }
    combine_quotes();
    std::cout << tokens << std::endl;
}

MultiMap::MultiMap<std::string, std::string> Parser::parse(std::string input) {
    MultiMap::MultiMap<std::string, std::string> parse_tree;
    set_string(input);
    sql_state.reset_state();
    int last_state = sql_state.get_state();
    while (!tokens.empty()) {
        string_tokenizer::Token t = tokens.pop();
        while (isspace(t.token_str()[0])) {
            t = tokens.pop();
        }
        std::string token_string = "";
        token_string = t.token_str();
        last_state = sql_state.update_state(token_string);
        // this wont make any sense unless you look at the state diagram or the
        // state table spreadsheet
        switch (last_state) {
            case -1:
                throw std::runtime_error("Error unexpected token: " +
                                         token_string);
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
    return parse_tree;
}

void Parser::combine_quotes() {
    Queue<string_tokenizer::Token> _tokens;
    // combine words between quotation marks
    while (!tokens.empty()) {
        auto current_token = tokens.pop();
        if (current_token.token_str() == "\"") {
            std::string token_string = "";
            while (!tokens.empty() && tokens.front().token_str() != "\"") {
                token_string += tokens.pop().token_str();
            }
            // we ran out of tokens without ever hitting another quote
            if (tokens.empty())
                throw std::runtime_error("Syntax error: no closing quote");
            // get rid of the closing quote
            if (tokens.front().token_str() == "\"")
                tokens.pop();

            _tokens.push(string_tokenizer::Token(token_string, 1));
        } else {
            _tokens.push(current_token);
        }
    }
    tokens = _tokens;
}
