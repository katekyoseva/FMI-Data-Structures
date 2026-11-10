#include <stack>
#include <string>
#include <vector>
#include <stdexcept>

// Task 1
bool isValidBracet(const std::string& s) {
    std::stack<char> sck;
    for (char c : s)
    {
        if (c == '(' || c == '[' || c == '{')
        {
            sck.push(c);
        }
        else
        {
            if (sck.empty()) return false;
            char top = sck.top();
            if ((c == ')' && top == '(') ||
                (c == ']' && top == '[') ||
                (c == '}' && top == '{'))
            {
                sck.pop();
            }
            else
            {
                return false;
            }
        }
    }
    return sck.empty();
}

// Task 2
class Solution2 {
public:
    int evalRPN(std::vector<std::string>& tokens) {
        std::stack<int> res;

        for (std::string& token : tokens)
        {
            if (token == "+" || token == "-" ||
                token == "*" || token == "/")
            {
                if (res.size() < 2) throw std::invalid_argument("Invalid expression!");
                int b = res.top();
                res.pop();

                int a = res.top();
                res.pop();

                switch (token[0])
                {
                case '+': res.push(a + b); break;
                case '-': res.push(a - b); break;
                case '*': res.push(a * b); break;
                case '/': res.push(a / b); break;
                }
            }
            else
            {
                res.push(stoi(token));
            }
        }
        return res.top();
    }
};

// Task 3
class Solution3 {
public:
    int calculate(const std::string& s) {
        std::stack<int> st;
        long res = 0;
        long curr = 0;
        short sign = 1;

        for (char c : s) {
            if ((c - '0') >= 0 && (c - '0') <= 9) {
                curr = curr * 10 + (c - '0');
            }
            else if (c == '+') {
                res += sign * curr;
                sign = 1;
                curr = 0;
            }
            else if (c == '-') {
                res += sign * curr;
                sign = -1;
                curr = 0;
            }
            else if (c == '(') {
                st.push(res);
                st.push(sign);
                res = 0;
                sign = 1;
            }
            else if (c == ')') {
                res += sign * curr;
                curr = 0;
                res *= st.top();
                st.pop();
                res += st.top();
                st.pop();
            }
        }

        res += sign * curr;
        return res;
    }
};