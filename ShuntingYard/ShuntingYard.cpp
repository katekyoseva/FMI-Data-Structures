#include <iostream>
#include <string>
#include <stack>
#include <vector>
#include <stdexcept>
#include <cmath>

/* Shunting Yard for infix expressions working with all integers and doubles*/

bool isOperator(char c)
{
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}

bool isDigit(char c)
{
    return c >= '0' && c <= '9';
}

int precedence(const std::string& op)
{
    if (op == "^") return 4;
    // includiing unary operators
    if (op == "u+" || op == "u-") return 3;
    if (op == "*" || op == "/") return 2;
    if (op == "+" || op == "-") return 1;

    return 0;
}

bool isRightAssociative(const std::string& op)
{
    return op == "^" || op == "u+" || op == "u-";
}

std::vector<std::string> tokenize(const std::string& expression)
{
    std::vector<std::string> tokens;

    for (int i = 0; i < expression.size();)
    {
        char c = expression[i];

        if (c == ' ')
        {
            i++;
            continue;
        }

        if (isDigit(c))
        {
            std::string number;
            bool hasDot = false;
            bool hasDigit = false;

            while (i < expression.size() && (isDigit(expression[i]) || expression[i] == '.'))
            {
                if (expression[i] == '.')
                {
                    if (hasDot) 
                        throw std::invalid_argument("Invalid number");
                    hasDot = true;
                }
                else
                    hasDigit = true;

                number.push_back(expression[i++]);
            }

            if (!hasDigit)
                throw std::invalid_argument("Invalid number");

            tokens.push_back(number);
        }
        else if (isOperator(c) || c == '(' || c == ')')
        {
            std::string op = "";
            op.push_back(c);
            tokens.push_back(op);
            i++;
        }
        else
            throw std::invalid_argument("Invalid character");
    }
    return tokens;
}

std::vector<std::string> toPostfix(const std::string& expression)
{
    std::vector<std::string> tokens = tokenize(expression);
    std::vector<std::string> output;
    std::stack<std::string> operators;

    bool expectOperand = true;

    for (const std::string& token : tokens)
    {
        if (isDigit(token[0]))
        {
            if (!expectOperand)
                throw std::invalid_argument("Invalid expression");

            output.push_back(token);
            expectOperand = false;
        }
        else if (token == "(")
        {
            if (!expectOperand)
                throw std::invalid_argument("Invalid expression");

            operators.push(token);
            expectOperand = true;
        }
        else if (token == ")")
        {
            if (expectOperand)
                throw std::invalid_argument("Invalid expression");

            while (!operators.empty() && operators.top() != "(")
            {
                output.push_back(operators.top());
                operators.pop();
            }

            if (operators.empty())
                throw std::invalid_argument("Mismatched parentheses");

            operators.pop();
            expectOperand = false;
        }
        else
        {
            std::string op = token;

            bool unary = false;
            if (expectOperand)
            {
                if (token != "+" && token != "-")
                    throw std::invalid_argument("Invalid operator");

                op = (token == "+") ? "u+" : "u-";
                unary = true;
            }
            else expectOperand = true;

            if (!unary)
            {
                while (!operators.empty() && operators.top() != "(")
                {
                    std::string top = operators.top();

                    if (precedence(top) > precedence(op) || (precedence(top) == precedence(op) && !isRightAssociative(op)))
                    {
                        output.push_back(top);
                        operators.pop();
                    }
                    else break;
                }
            }

            operators.push(op);
        }
    }

    if (expectOperand)
        throw std::invalid_argument("Invalid expression");

    while (!operators.empty())
    {
        if (operators.top() == "(")
            throw std::invalid_argument("Mismatched parentheses");

        output.push_back(operators.top());
        operators.pop();
    }

    return output;
}

void applyOperator(std::stack<double>& values, const std::string& op)
{
    if (op == "u+" || op == "u-")
    {
        if (values.empty())
            throw std::invalid_argument("Invalid expression");

        double a = values.top();
        values.pop();

        values.push(op == "u-" ? -a : a);
        return;
    }

    if (values.size() < 2)
        throw std::invalid_argument("Invalid expression");


    double b = values.top();
    values.pop();

    double a = values.top();
    values.pop();

    if (op == "+") values.push(a + b);
    else if (op == "-") values.push(a - b);
    else if (op == "*") values.push(a * b);
    else if (op == "/")
    {
        if (b == 0)
            throw std::invalid_argument("Division by zero");

        values.push(a / b);
    }
    else if (op == "^") values.push(pow(a, b));
}

double evaluatePostfix(const std::vector<std::string>& postfix)
{
    std::stack<double> numbers;

    for (const std::string& token : postfix)
    {
        if (isDigit(token[0]))
        {
            numbers.push(stod(token));
        }
        else
            applyOperator(numbers, token);
    }

    if (numbers.size() != 1)
        throw std::invalid_argument("Invalid expression");

    return numbers.top();
}

double evaluate(const std::string& expression)
{
    std::vector<std::string> tokens = tokenize(expression);
    std::stack<std::string> operators;
    std::stack<double> values;

    bool expectOperand = true;

    for (const std::string& token : tokens)
    {
        if (isDigit(token[0]))
        {
            if (!expectOperand)
                throw std::invalid_argument("Invalid expression");

            values.push(stod(token));
            expectOperand = false;
        }
        else if (token == "(")
        {
            if (!expectOperand)
                throw std::invalid_argument("Invalid expression");

            operators.push(token);
            expectOperand = true;
        }
        else if (token == ")")
        {
            if (expectOperand)
                throw std::invalid_argument("Invalid expression");

            while (!operators.empty() && operators.top() != "(")
            {
                applyOperator(values, operators.top());
                operators.pop();
            }

            if (operators.empty())
                throw std::invalid_argument("Mismatched parentheses");

            operators.pop();
            expectOperand = false;
        }
        else
        {
            std::string op = token;
            bool unary = false;

            if (expectOperand)
            {
                if (token != "+" && token != "-")
                    throw std::invalid_argument("Invalid operator");

                op = (token == "+") ? "u+" : "u-";
                unary = true;
            }
            else
                expectOperand = true;

            if (!unary)
            {
                while (!operators.empty() && operators.top() != "(")
                {
                    std::string top = operators.top();

                    if (precedence(top) > precedence(op) ||
                        (precedence(top) == precedence(op) &&
                            !isRightAssociative(op)))
                    {
                        applyOperator(values, top);
                        operators.pop();
                    }
                    else
                        break;
                }
            }
            operators.push(op);
        }
    }

    if (expectOperand)
        throw std::invalid_argument("Invalid expression");

    while (!operators.empty())
    {
        if (operators.top() == "(")
            throw std::invalid_argument("Mismatched parentheses");

        applyOperator(values, operators.top());
        operators.pop();
    }

    if (values.size() != 1)
        throw std::invalid_argument("Invalid expression");
    return values.top();
}

int main()
{
    std::string expression;

    getline(std::cin, expression);

    try
    {
        std::vector<std::string> postfix = toPostfix(expression);
        std::cout << "\nPostfix: ";

        for (const std::string& token : postfix)
            std::cout << token << ' ';

        std::cout << "\nResult: ";
        std::cout << evaluatePostfix(postfix);

        std::cout << "\n\nDiirect evaluation result: ";
        std::cout << evaluate(expression);
    }
    catch (std::exception e)
    {
        std::cout << e.what();
    }

    return 0;
}
