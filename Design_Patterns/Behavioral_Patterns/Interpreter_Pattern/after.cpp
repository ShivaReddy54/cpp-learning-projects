#include <iostream>
#include <string>
#include <map>

// ===== STEP 1: Abstract Expression =====
class Expression{
public:
    virtual ~Expression() = default;
    virtual int interpret(const std::map<std::string, int>& context) = 0;
};

// ===== STEP 2: Terminal Expression — leaf nodes (numbers, variables) =====
class NumberExpression : public Expression{
private:
    int number;

public:
    NumberExpression(int n) : number(n) {}
    int interpret(const std::map<std::string, int>& context) override{
        return number;
    }
};

class VariableExpression : public Expression{
private:
    std::string name;

public:
    VariableExpression(const std::string& n) : name(n) {}
    int interpret(const std::map<std::string, int>& context) override{
        // Look up variable value in context
        auto it = context.find(name);
        if(it != context.end()) return it->second;

        std::cout << "Variable '" << name << "' not found!" << std::endl;
        return 0;
    }
};

// ===== STEP 3: Non-terminal Expressions — operator nodes (combine sub-expressions) =====
class AddExpression : public Expression{
private:
    Expression* left;
    Expression* right;

public:
    AddExpression(Expression* l, Expression* r) : left(l), right(r) {}
    int interpret(const std::map<std::string, int>& context) override{
        return left->interpret(context) + right->interpret(context);
    }
    ~AddExpression() { delete left; delete right; }
};

class SubtractExpression : public Expression{
private:
    Expression* left;
    Expression* right;

public:
    SubtractExpression(Expression* l, Expression* r) : left(l), right(r) {}
    int interpret(const std::map<std::string, int>& context) override{
        return left->interpret(context) - right->interpret(context);
    }
    ~SubtractExpression() { delete left; delete right; }
};

class MultiplyExpression : public Expression{
private:
    Expression* left;
    Expression* right;

public:
    MultiplyExpression(Expression* l, Expression* r) : left(l), right(r) {}
    int interpret(const std::map<std::string, int>& context) override{
        return left->interpret(context) * right->interpret(context);
    }
    ~MultiplyExpression() { delete left; delete right; }
};

// ===== CLIENT CODE =====
int main() {
    // Context: variable values
    std::map<std::string, int> context;
    context["x"] = 5;
    context["y"] = 3;
    context["z"] = 2;

    // ===== Simple expression: 5 + 3 =====
    Expression* simple = new AddExpression(
        new NumberExpression(5),
        new NumberExpression(3)
    );
    std::cout << "5 + 3 = " << simple->interpret(context) << std::endl;
    delete simple;

    // ===== Variable expression: x + y =====
    Expression* withVars = new AddExpression(
        new VariableExpression("x"),
        new VariableExpression("y")
    );
    std::cout << "x + y = " << withVars->interpret(context) << std::endl;
    delete withVars;

    // ===== NESTED expression: (x + y) - z  =====
    // This was IMPOSSIBLE before! Now easy:
    Expression* nested = new SubtractExpression(
        new AddExpression(             // (x + y) is a sub-expression!
            new VariableExpression("x"),
            new VariableExpression("y")
        ),
        new VariableExpression("z")   // - z
    );
    std::cout << "(x + y) - z = (5 + 3) - 2 = " << nested->interpret(context) << std::endl;
    delete nested;

    // ===== Complex nested: ((x + y) * z) - 5 =====
    Expression* complex = new SubtractExpression(
        new MultiplyExpression(
            new AddExpression(
                new VariableExpression("x"),
                new VariableExpression("y")
            ),
            new VariableExpression("z")
        ),
        new NumberExpression(5)
    );
    std::cout << "(x + y) * z - 5 = (5 + 3) * 2 - 5 = " << complex->interpret(context) << std::endl;
    delete complex;

    // ===== Adding a NEW operator? Just create a new Expression class! =====
    // No modification to existing classes needed!

}

/*
===============================================================================
                  Interpreter Pattern: Before vs After
===============================================================================

BEFORE (Without Interpreter)

evaluateExpression("5 + 3")
    │
    ├── Find '+'
    ├── Parse left operand
    ├── Parse right operand
    └── Return result

Characteristics:

  • Ad-hoc string parsing.
  • Works only for simple expressions such as "A + B".
  • No support for nested expressions.
  • Difficult to extend with new operators.


Problems:
  ❌ No grammar representation.
  ❌ No support for nested expressions.
  ❌ Poor extensibility.
  ❌ Parser becomes increasingly complex.


===============================================================================

AFTER (Using Interpreter)

Expression Tree

                 AddExpression
                 /           \
                /             \
   NumberExpression(5)   NumberExpression(3)

Evaluation happens recursively:

    AddExpression.interpret()
        = left.interpret() + right.interpret()
        = 5 + 3
        = 8


===============================================================================

Nested Expressions Become Natural

                 SubtractExpression
                  /              \
                 /                \
        AddExpression       Variable("z")
          /       \                │
         /         \               ▼
 Variable("x")  Variable("y")      2
      │               │
      ▼               ▼
      5               3

Evaluation:

    (x + y) - z
    = (5 + 3) - 2
    = 6


Result:
  ✅ Grammar is represented as an expression tree.
  ✅ Nested expressions are handled naturally through recursion.
  ✅ New operators can be added by creating new Expression classes.
  ✅ Existing interpreter code remains unchanged.

===============================================================================
*/