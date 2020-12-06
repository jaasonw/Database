#include "StringTokenizer/Token.h"

using namespace string_tokenizer;

Token::Token(): _token(""), _type(0) {}
Token::Token(const std::string& str, int type) : _token(str), _type(type) {}

std::string Token::type_string() const {
    switch (_type) {
        case 1:
        case 7:
        case 11:
            return "ALPHA";
        case 2:
            return "SPACE";
        case 3:
        case 10:
            return "NUMBER";
        case 5:
        case 6:
            return "PUNCTUATION";
        case 9:
            return "ELLIPSIS";
        default:
            return "UNKNOWN";
    }
}