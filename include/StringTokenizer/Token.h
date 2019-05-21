#pragma once
#include <iostream>
#include <string>

namespace string_tokenizer {
class Token {
private:
    std::string _token;
    int _type;

public:
    Token();
    Token(std::string str, int type);
    int type() const { return _type; };
    std::string type_string() const;
    std::string token_str() const { return _token; };
    void set_string(std::string str) { _token = str; }
    void set_type(int type) { _type = type; }
    friend std::ostream& operator<<(std::ostream& outs, const Token& t) {
         return outs << t._token;
    }
};
} // namespace string_tokenizer