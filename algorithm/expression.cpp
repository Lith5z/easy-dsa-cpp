// 前缀、后缀表达式求值，中缀转换为后缀
// 表达式约定为每个数/操作符之间有一个空格，括号仅有(，表达式合法，不使用负数

#include <cassert>
#include <cctype>
#include <iostream>
#include <sstream>
#include <stack>
#include <string>
#include <vector>

constexpr int operate(char op, int left, int right) {
    switch (op) {
        case '+':
            return (left + right);
        case '-':
            return (left - right);
        case '*':
            return (left * right);
        case '/':
            assert(right != 0);
            return (left / right);
        default:
            assert(false);
            return -1;
    }
}

int cal_postfix(const std::string& s) {
    std::istringstream input(s);
    std::stack<int> values;
    std::string token;

    while (input >> token) {
        // 是数字，入栈
        if (std::isdigit(token[0])) {
            values.push(std::stoi(token));
            continue;
        }

        // 不是数字，出栈两次，取出两个操作数
        // 先出栈的是后进入的数字，所以先取出right操作数
        char op = token[0];
        int right = values.top();
        values.pop();
        int left = values.top();
        values.pop();

        // 计算并将结果入栈
        values.push(operate(op, left, right));
    }

    return values.top();
}

int cal_prefix(const std::string& s) {
    // 先按 token
    // 分割成vector，再逆序，直接翻转字符串会导致12->21这样多位数的错误
    std::istringstream input(s);
    std::vector<std::string> tokens;
    std::stack<int> values;
    std::string token;

    while (input >> token) {
        tokens.push_back(token);
    }

    for (auto it = tokens.rbegin(); it != tokens.rend(); ++it) {
        token = *it;
        if (std::isdigit(token[0])) {
            values.push(std::stoi(token));
            continue;
        }

        // 逆序处理后，先出栈的是原表达式中的left操作数
        char op = token[0];
        int left = values.top();
        values.pop();
        int right = values.top();
        values.pop();

        values.push(operate(op, left, right));
    }

    return values.top();
}

constexpr int precedence(char op) {
    switch (op) {
        case '*':
        case '/':
            return 2;
        case '+':
        case '-':
            return 1;
        default:
            // 括号
            return -1;
    }
}
std::string infix_to_postfix(const std::string& s) {
    std::istringstream input(s);
    std::string res;
    std::stack<char> ops;
    std::string token;

    while (input >> token) {
        if (std::isdigit(token[0])) {
            res += token;
            res += " ";
        }
        else if (precedence(token[0]) > 0) {
            // 第二个条件说明ops优先级由高到低，应当有个式子可以计算了
            // 等于是因为，从左到右计算，如果有个同级的运算符，也应视作一个式子的边界
            // 第三个条件，左括号和当前op是一个式子，等待右括号封闭
            while (!ops.empty() &&
                   precedence(token[0]) <= precedence(ops.top()) &&
                   (ops.top() != '(')) {
                res += ops.top();
                res += " ";
                ops.pop();
            }
            ops.push(token[0]);  // 压入当前的op
        }
        else if (token[0] == '(') {
            ops.push(token[0]);
        }
        else {  // is ) 一直出栈直到匹配的(
            while (!ops.empty() && ops.top() != '(') {
                res += ops.top();
                res += " ";
                ops.pop();
            }
            ops.pop();  // 弹出上个循环忽略的(本身
        }
    }
    while (!ops.empty()) {
        res += ops.top();
        res += " ";
        ops.pop();
    }
    return res;
}

int main() {
    assert(cal_prefix("+ 12 3") == 15);
    assert(cal_prefix("- * 12 3 4") == 32);
    assert(cal_prefix("/ + 20 4 3") == 8);

    assert(cal_postfix("12 3 +") == 15);
    assert(cal_postfix("12 3 * 4 -") == 32);
    assert(cal_postfix("20 4 + 3 /") == 8);

    std::string postfix = infix_to_postfix("3 + 4 * 2");
    assert(postfix == "3 4 2 * + ");
    assert(cal_postfix(postfix) == 11);

    postfix = infix_to_postfix("( 3 + 4 ) * 2 - 8 / 4");
    assert(postfix == "3 4 + 2 * 8 4 / - ");
    assert(cal_postfix(postfix) == 12);

    std::cout << "All expression tests passed.\n";
    return 0;
}