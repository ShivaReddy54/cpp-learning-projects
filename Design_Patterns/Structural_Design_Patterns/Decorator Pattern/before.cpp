/*

Intent: Attach additional responsibilities to an object dynamically.
    Decorators provide a flexible alternative to subclassing for extending functionality.

EX: You order a coffee. You start with a basic espresso.
    Then you wrap it with milk → becomes latte. Then you wrap it with whipped cream → becomes latte with cream.
    Each wrapper adds behavior. You don't create a new LatteWithCreamCoffee class — you stack decorators on the base object.
*/

#include <iostream>
#include <string>

// ===== Base class =====
class Coffee{
public:
    virtual ~Coffee() = default;
    virtual double getCost() = 0;
    virtual std::string getDescription() = 0;
};

// ===== Plain coffee =====
class SimpleCoffee : public Coffee{
public:
    double getCost() override { return 10.0; }
    std::string getDescription() override { return "Simple Coffee"; }
};

// ===== PROBLEM: Subclass explosion to add features! =====

// Option 1: One class per combination — EXPLOSION!
class MilkCoffee : public Coffee {
public:
    double getCost() override { return 12.0; }
    std::string getDescription() override { return "Coffee with Milk"; }
};

class WhipCoffee : public Coffee {
public:
    double getCost() override { return 14.0; }
    std::string getDescription() override { return "Coffee with Whip Cream"; }
};

class MilkAndWhipCoffee : public Coffee {
public:
    double getCost() override { return 16.0; }
    std::string getDescription() override { return "Coffee with Milk and Whip Cream"; }
};

class SprinkleCoffee : public Coffee {
public:
    double getCost() override { return 15.0; }
    std::string getDescription() override { return "Coffee with Sprinkles"; }
};

class MilkAndSprinkleCoffee : public Coffee {
    // More duplication...
};

class WhipAndSprinkleCoffee : public Coffee {
    // More duplication...
};

class MilkWhipAndSprinkleCoffee : public Coffee {
    // Even more duplication!
};

// For 3 decorators: 2^3 = 8 combinations = 8 classes!
// For 5 decorators: 2^5 = 32 combinations = 32 classes!
// Add a new decorator? ALL combinations must be recreated!

int main() {
    Coffee* coffee = new SimpleCoffee();
    std::cout << coffee->getDescription() << " : $" << coffee->getCost() << std::endl;

    Coffee* milkCoffee = new MilkCoffee();
    std::cout << milkCoffee->getDescription() << " : $" << milkCoffee->getCost() << std::endl;

    // PROBLEM: What if I want Milk + Whip + Sprinkles?
    // I need a specific MilkWhipAndSprinkleCoffee class!
    // I can't dynamically add/remove features
    // Every combination needs its own class

    delete coffee;
    delete milkCoffee;

}


/*
===============================================================================
                  Problems Without the Decorator Pattern
===============================================================================

Suppose an object can have N optional features (decorators).

Without the Decorator Pattern, every possible combination requires its own
subclass.

Example:

3 Decorators
    → 2³ = 8 Classes

5 Decorators
    → 2⁵ = 32 Classes

10 Decorators
    → 2¹⁰ = 1024 Classes

As the number of optional features grows, the number of required subclasses
grows exponentially. This is known as the **Subclass Explosion** problem.


Problems:

❌ Class Explosion
   Every combination of features requires a separate subclass,
   resulting in an exponential increase in the number of classes.

❌ No Runtime Flexibility
   Features are fixed when the object is created and cannot be
   added dynamically during execution.

❌ Cannot Remove Features
   Once a subclass is chosen, individual features cannot be
   removed without creating another subclass.

❌ Massive Code Duplication
   Similar logic is repeated across many combination classes,
   making maintenance difficult.

❌ Difficult to Extend
   Adding a new optional feature requires creating many new
   subclasses to represent every possible combination.

❌ Violates Open/Closed Principle
   Existing code often needs to be modified whenever a new
   feature or feature combination is introduced.

===============================================================================
*/