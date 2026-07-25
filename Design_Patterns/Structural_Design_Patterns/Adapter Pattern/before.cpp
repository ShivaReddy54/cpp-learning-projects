/*

Intent: Convert the interface of a class into another interface that clients expect. 
    Lets classes work together that couldn't otherwise because of incompatible interfaces.

EX: Your laptop charger has a US plug (flat pins), but European outlets use round pins. 
    You buy a travel adapter — it doesn't change your charger or the wall, 
    it just converts one interface to the other so they can work together.

*/

#include <iostream>
#include <string>

// ===== Existing class (OLD interface) =====
class EuropeanPrinter{
public:
    void printInEuros(double amount){
        std::cout << "Printing receipt: " << amount << " EUR" << std::endl;
    }
};

// ===== New class (DIFFERENT interface) =====
class AmericanPrinter{
public:
    void printInDollars(double amount){
        std::cout << "Printing receipt: $" << amount << " USD" << std::endl;
    }
};

// ===== Another incompatible class =====
class JapanesePrinter{
public:
    void printInYen(double amount){
        std::cout << "Printing receipt: " << amount << " JPY" << std::endl;
    }
};

// ===== Client expects a SPECIFIC interface =====
class PaymentSystem {
public:
    // Client can only call printInDollars()
    // It CANNOT work with EuropeanPrinter or JapanesePrinter
    void processPayment(AmericanPrinter* printer, double amount){
        std::cout << "Processing payment of $" << amount << std::endl;
        printer->printInDollars(amount);
    }
};

int main() {
    AmericanPrinter usPrinter;
    PaymentSystem system;

    // This works — matching interface
    system.processPayment(&usPrinter, 100.0);

    // PROBLEM: EuropeanPrinter has printInEuros(), NOT printInDollars()
    // We CANNOT pass it to PaymentSystem!
    EuropeanPrinter euPrinter;
    // system.processPayment(&euPrinter, 100.0);  // ERROR! Incompatible interface!

    // PROBLEM: JapanesePrinter has printInYen(), NOT printInDollars()
    // We CANNOT pass it to PaymentSystem!
    JapanesePrinter jpPrinter;
    // system.processPayment(&jpPrinter, 100.0);  // ERROR! Incompatible interface!

    // The only "solution" without a pattern is to modify PaymentSystem
    // with if-else for each printer type — BAD!
    // OR modify EuropeanPrinter — but we might not OWN that class!

}


/*
===============================================================================
                   Problems Without the Adapter Pattern
===============================================================================

                         PaymentSystem
                                │
                   Expects: printInDollars()
                                │
          ┌─────────────────────┼─────────────────────┐
          │                     │                     │
          ▼                     ▼                     ▼
  AmericanPrinter      EuropeanPrinter      JapanesePrinter
  printInDollars()     printInEuros()       printInYen()
         ✅                  ❌                   ❌

The client expects every printer to expose the same interface, but each
implementation provides a different method name.


Problems:

❌ Incompatible Interfaces
   Existing classes cannot work together because they expose different
   interfaces.

❌ Cannot Modify Existing Classes
   Third-party libraries, legacy systems, or external APIs often cannot
   be changed to match the required interface.

❌ Risky Client Changes
   The client must either be modified to understand every implementation
   or existing classes must be rewritten.

❌ Violates Open/Closed Principle
   Adding support for another printer type usually requires more if-else
   statements or modifications to existing client code.

===============================================================================
*/