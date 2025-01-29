#include <iostream>
#include <cctype>
#include <string>
#include <stdexcept>

using namespace std;

class SimpleCompiler {
private:
    string expression;
    size_t currentPos;

public:
    SimpleCompiler(const string& expr) : expression(expr), currentPos(0) {}

    // Function to get the current character in the expression
    char currentChar() {
        if (currentPos < expression.length()) {
            return expression[currentPos];
        }
        return '\0';  // Return null character if we're at the end of the string
    }

    // Function to move to the next character in the expression
    void nextChar() {
        if (currentPos < expression.length()) {
            currentPos++;
        }
    }

    // Function to skip whitespace
    void skipWhitespace() {
        while (isspace(currentChar())) {
            nextChar();
        }
    }

    // Function to parse a number (integer)
    int parseNumber() {
        int result = 0;
        while (isdigit(currentChar())) {
            result = result * 10 + (currentChar() - '0');
            nextChar();
        }
        return result;
    }

    // Function to parse a factor: numbers and parentheses
    int parseFactor() {
        skipWhitespace();
        int result = 0;
        
        if (currentChar() == '(') {
            nextChar();  // Consume '('
            result = parseExpression();
            if (currentChar() == ')') {
                nextChar();  // Consume ')'
            } else {
                throw runtime_error("Expected ')'");
            }
        } else if (isdigit(currentChar())) {
            result = parseNumber();
        } else {
            throw runtime_error("Unexpected character in factor");
        }

        return result;
    }

    // Function to parse a term: multiplication and division
    int parseTerm() {
        int result = parseFactor();
        while (true) {
            skipWhitespace();
            if (currentChar() == '*') {
                nextChar();  // Consume '*'
                result *= parseFactor();
            } else if (currentChar() == '/') {
                nextChar();  // Consume '/'
                int divisor = parseFactor();
                if (divisor == 0) {
                    throw runtime_error("Division by zero");
                }
                result /= divisor;
            } else {
                break;
            }
        }
        return result;
    }

    // Function to parse an expression: addition and subtraction
    int parseExpression() {
        int result = parseTerm();
        while (true) {
            skipWhitespace();
            if (currentChar() == '+') {
                nextChar();  // Consume '+'
                result += parseTerm();
            } else if (currentChar() == '-') {
                nextChar();  // Consume '-'
                result -= parseTerm();
            } else {
                break;
            }
        }
        return result;
    }

    // Main function to evaluate the expression
    int evaluate() {
        int result = parseExpression();
        skipWhitespace();
        if (currentChar() != '\0') {
            throw runtime_error("Unexpected character after expression");
        }
        return result;
    }
};

int main() {
    string expression;
    
    cout << "Enter an arithmetic expression: ";
    getline(cin, expression);

    SimpleCompiler compiler(expression);
    
    try {
        int result = compiler.evaluate();
        cout << "Result: " << result << endl;
    } catch (const runtime_error& e) {
        cout << "Error: " << e.what() << endl;
    }

    return 0;
}

