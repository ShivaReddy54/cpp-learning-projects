#include <iostream>
#include <string>

// ===== STEP 1: Component — base interface =====
class Coffee{
public:
    virtual ~Coffee() = default;
    virtual double getCost() = 0;
    virtual std::string getDescription() = 0;
};

// ===== STEP 2: Concrete Component — the base object =====
class SimpleCoffee : public Coffee {
public:
    double getCost() override { return 10.0; }
    std::string getDescription() override { return "Simple Coffee"; }
};

// ===== STEP 3: Base Decorator — wraps a Coffee =====
class CoffeeDecorator : public Coffee {
protected:
    Coffee* wrappedCoffee;  // The object being decorated

public:
    CoffeeDecorator(Coffee* coffee) : wrappedCoffee(coffee) {}
    virtual ~CoffeeDecorator() { delete wrappedCoffee; }

    double getCost() override { return wrappedCoffee->getCost(); }
    std::string getDescription() override { return wrappedCoffee->getDescription(); }
};

// ===== STEP 4: Concrete Decorators — each adds ONE feature =====

class MilkDecorator : public CoffeeDecorator {
public:
    MilkDecorator(Coffee* coffee) : CoffeeDecorator(coffee) {}

    double getCost() override {
        return wrappedCoffee->getCost() + 2.0;  // Add milk cost
    }

    std::string getDescription() override {
        return wrappedCoffee->getDescription() + " + Milk";  // Append to existing
    }
};

class WhipDecorator : public CoffeeDecorator {
public:
    WhipDecorator(Coffee* coffee) : CoffeeDecorator(coffee) {}

    double getCost() override {
        return wrappedCoffee->getCost() + 4.0;  // Add whip cost
    }

    std::string getDescription() override {
        return wrappedCoffee->getDescription() + " + Whip Cream";
    }
};

class SprinkleDecorator : public CoffeeDecorator {
public:
    SprinkleDecorator(Coffee* coffee) : CoffeeDecorator(coffee) {}

    double getCost() override {
        return wrappedCoffee->getCost() + 5.0;  // Add sprinkle cost
    }

    std::string getDescription() override {
        return wrappedCoffee->getDescription() + " + Sprinkles";
    }
};

class CaramelDecorator : public CoffeeDecorator {
public:
    CaramelDecorator(Coffee* coffee) : CoffeeDecorator(coffee) {}

    double getCost() override {
        return wrappedCoffee->getCost() + 3.0;  // Add caramel cost
    }

    std::string getDescription() override {
        return wrappedCoffee->getDescription() + " + Caramel Syrup";
    }
};

// ===== CLIENT CODE =====
int main() {
    // ===== Simple coffee =====
    Coffee* myCoffee = new SimpleCoffee();
    std::cout << "=== Just Simple Coffee ===" << std::endl;
    std::cout << myCoffee->getDescription() << " : $" << myCoffee->getCost() << std::endl;

    std::cout << std::endl;

    // ===== Add milk — WRAP the existing coffee =====
    myCoffee = new MilkDecorator(myCoffee);
    std::cout << "=== + Milk ===" << std::endl;
    std::cout << myCoffee->getDescription() << " : $" << myCoffee->getCost() << std::endl;

    std::cout << std::endl;

    // ===== Add whip cream — WRAP again =====
    myCoffee = new WhipDecorator(myCoffee);
    std::cout << "=== + Whip Cream ===" << std::endl;
    std::cout << myCoffee->getDescription() << " : $" << myCoffee->getCost() << std::endl;

    std::cout << std::endl;

    // ===== Add sprinkles — WRAP again =====
    myCoffee = new SprinkleDecorator(myCoffee);
    std::cout << "=== + Sprinkles ===" << std::endl;
    std::cout << myCoffee->getDescription() << " : $" << myCoffee->getCost() << std::endl;

    std::cout << std::endl;

    // ===== Different combination: Simple + Caramel + Milk =====
    Coffee* anotherCoffee = new SimpleCoffee();
    anotherCoffee = new CaramelDecorator(anotherCoffee);
    anotherCoffee = new MilkDecorator(anotherCoffee);
    std::cout << "=== Another Coffee: Caramel + Milk ===" << std::endl;
    std::cout << anotherCoffee->getDescription() << " : $" << anotherCoffee->getCost() << std::endl;

    // ===== No class explosion! 4 decorators = 4 classes (not 16!) =====
    // Any combination is possible by stacking decorators!

    // Cleanup — only delete the outermost decorator
    // It will cascade delete through the chain
    delete myCoffee;
    delete anotherCoffee;

}


/*
===============================================================================
                    Decorator Pattern: Before vs After
===============================================================================

BEFORE (Without Decorator)

Every combination of features requires a separate subclass.

    MilkCoffee
    WhipCoffee
    SprinkleCoffee
    MilkAndWhipCoffee
    MilkAndSprinkleCoffee
    WhipAndSprinkleCoffee
    MilkWhipAndSprinkleCoffee
    ...
    (Many more combinations)

Example:
    4 Optional Decorators
        → 2⁴ = 16 Classes


Problems:
  ❌ Large number of subclasses.
  ❌ Features are fixed at compile time.
  ❌ Difficult to support new combinations.
  ❌ Poor scalability as more decorators are added.


===============================================================================

AFTER (Using Decorator Pattern)

                Coffee
            (Component Interface)
                     │
                     ▼
              SimpleCoffee
             (Concrete Component)
                     │
        ┌────────────┼────────────┬────────────┐
        ▼            ▼            ▼            ▼
MilkDecorator  WhipDecorator  SprinkleDecorator  CaramelDecorator
                     │
                     ▼
        Decorators can be wrapped in ANY order.

Example:

SimpleCoffee
      │
      ▼
MilkDecorator
      │
      ▼
WhipDecorator
      │
      ▼
CaramelDecorator


Total:
    1 Component
  + 1 Concrete Component
  + 4 Decorator Classes
  = 6 Classes

Instead of:
    16 Combination Classes


Result:
  ✅ Only 6 classes instead of 16.
  ✅ Features can be added dynamically at runtime.
  ✅ Any combination of decorators is possible.
  ✅ Easy to add new decorators without changing existing code.

===============================================================================
*/