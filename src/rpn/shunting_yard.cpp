#include "rpn/shunting_yard.h"

Queue<std::string> infix_to_postfix(std::vector<std::string> infix) {
    // operator -> precedence
    Map::Map<std::string, int> operators;
    operators["AND"] = 0;
    operators["OR"] = 0;
    operators[">"] = 2;
    operators[">="] = 2;
    operators["<"] = 2;
    operators["<="] = 2;
    operators["="] = 2;

    Stack<std::string> stack;
    Queue<std::string> queue;
    for (auto token : infix) {
        if (!operators.contains(string_util::uppercase(token)))
            queue.push(token);
        else {
            while (!stack.empty() &&
                   operators[string_util::uppercase(stack.top())] >=
                       operators[string_util::uppercase(token)]) {
                queue.push(stack.pop());
            }
            stack.push(token);
        }
    }
    while (!stack.empty()) {
        queue.push(stack.pop());
    }
    return queue;
}