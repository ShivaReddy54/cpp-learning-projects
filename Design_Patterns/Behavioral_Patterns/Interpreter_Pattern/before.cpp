/*

Intent: Given a language, define a representation for its grammar along with an interpreter 
        that uses the representation to interpret sentences in the language.

EX: A calculator evaluates mathematical expressions. When you type 5 + 3 - 2, 
    the calculator interprets this as a language: numbers and operators follow grammar rules. 
    The interpreter parses and evaluates each part recursively.

*/

#include <iostream>
#include <string>

// ===== Evaluating expressions manually — hardcoded parsing =====
int evaluateExpression(const std::string& expr){
    // PROBLEM: Must manually parse the string each time
    // PROBLEM: Only handles one specific format
    // PROBLEM: Hard to add new operators or rules
    // PROBLEM: No grammar representation
    // PROBLEM: Complex nested expressions are impossible to handle

    // Very fragile, hardcoded parsing for "A + B" format only!
    size_t plusPos = expr.find('+');
    size_t minusPos = expr.find('-');

    if(plusPos != std::string::npos){
        int left = std::stoi(expr.substr(0, plusPos));
        int right = std::stoi(expr.substr(plusPos + 1));
        return left + right;
    }
    else if(minusPos != std::string::npos){
        int left = std::stoi(expr.substr(0, minusPos));
        int right = std::stoi(expr.substr(minusPos + 1));
        return left - right;
    }
    else return std::stoi(expr);

    // What about: (5 + 3) - 2? Can't handle parentheses!
    // What about: 5 * 3? Can't handle multiplication!
    // What about: nested expressions? Impossible!
    // Adding any new operator requires rewriting this function!
}

int main() {
    std::cout << "5 + 3 = " << evaluateExpression("5+3") << std::endl;
    std::cout << "10 - 4 = " << evaluateExpression("10-4") << std::endl;

    // PROBLEM: Can't do (5 + 3) - 2
    // PROBLEM: Can't do 5 + 3 + 2
    // PROBLEM: Adding * / requires rewriting entire function
    // PROBLEM: No extensibility
    // PROBLEM: No grammar structure — just ad-hoc string parsing

}

/*
===============================================================================
                Problems Without the Interpreter Pattern
===============================================================================

Example:

    evaluateExpression("5 + 3")
        │
        ├── Find '+'
        ├── Parse left operand
        ├── Parse right operand
        └── Return left + right

This approach only works for a very simple expression format such as:

    A + B


Problems:

❌ No Grammar Representation
   The language syntax exists only as parsing code, making it difficult
   to understand, extend, or reuse.

❌ Cannot Handle Nested Expressions
   Expressions such as:

       (5 + 3) - 2

   require much more complex parsing logic.

❌ Cannot Handle Multiple Operators
   Expressions like:

       5 + 3 + 2

   or

       5 + 3 * 2

   quickly become difficult to parse correctly.

❌ Difficult to Add New Operators
   Supporting operators such as '*', '/', '%', or '^' requires
   rewriting or extending the parser logic.

❌ Poor Extensibility
   Every new grammar rule increases the complexity of the parser,
   making future changes more difficult.

❌ Hard to Maintain
   As the language grows, parsing code becomes larger, harder to read,
   and more error-prone.

===============================================================================
*/