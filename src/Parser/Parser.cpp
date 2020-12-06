#include "Parser/Parser.h"

void Parser::set_string(const std::string& input) {
    tokens.clear();
    tokenizer.set_string(input.c_str());
    string_tokenizer::Token t;
    while (!tokenizer.done()) {
        tokenizer >> t;
        tokens.push(t);
    }
    #ifdef DEBUG
        std::cout << tokens << std::endl;
    #endif
}

MultiMap::MultiMap<std::string, std::string> Parser::parse(const std::string& input) {
    MultiMap::MultiMap<std::string, std::string> parse_tree;
    set_string(input);
    sql_state.reset_state();
    int last_state = sql_state.get_state();
    while (!tokens.empty()) {
        string_tokenizer::Token t = tokens.pop();

        while (!tokens.empty() && isspace(t.token_str()[0])) {
            t = tokens.pop();
        }
        // we somehow exited the while loop with spaces, it's probably not
        // supposed to be there
        if (isspace(t.token_str()[0]))
            break;

        std::string token_string = t.token_str();
        last_state = sql_state.update_state(token_string);

        if (sql_state.is_quote_state(last_state)) {
            token_string = "";
            while (!tokens.empty() && sql_state.is_quote_state(last_state)) {
                std::string _token_string = tokens.pop().token_str();
                last_state = sql_state.update_state(_token_string);
                if (_token_string != "\"")
                    token_string += _token_string;
            }
            // we exited this loop while never finding a closing quote
            if (sql_state.is_quote_state(last_state))
                throw std::runtime_error(NO_CLOSING_QUOTE);
        }


        if (sql_state.is_invalid())
            throw std::runtime_error(UNEXPECTED_TOKEN + token_string);
        else {
            std::string key = sql_state.get_parse_key(last_state);
            if (key != "") {
                if (key == "command")
                    parse_tree[key] += string_util::uppercase(token_string);
                else
                    parse_tree[key] += token_string;
            }
        }
    }
    if (sql_state.is_invalid() || !sql_state.is_success()) {
        throw std::runtime_error(END_OF_INPUT);
    }
    #ifdef DEBUG
        std::cout << parse_tree << std::endl;
    #endif
    return parse_tree;
}