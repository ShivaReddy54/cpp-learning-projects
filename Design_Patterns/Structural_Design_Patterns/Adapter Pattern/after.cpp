#include <iostream>
#include <string>

// ===== Target Interface (what the client expects) =====
class PrinterInterface{
public:
    virtual ~PrinterInterface() = default;
    virtual void printInDollars(double amount) = 0;
};

// ===== Existing Adaptee classes (incompatible interfaces) =====
class EuropeanPrinter{
public:
    void printInEuros(double amount){
        std::cout << "Printing receipt: " << amount << " EUR" << std::endl;
    }
};

class JapanesePrinter{
public:
    void printInYen(double amount){
        std::cout << "Printing receipt: " << amount << " JPY" << std::endl;
    }
};

// ===== Existing compatible class (already matches target) =====
class AmericanPrinter : public PrinterInterface {
public:
    void printInDollars(double amount) override{
        std::cout << "Printing receipt: $" << amount << " USD" << std::endl;
    }
};

// ===== STEP 1: Adapter for EuropeanPrinter =====
class EuropeanPrinterAdapter : public PrinterInterface {
private:
    EuropeanPrinter* europeanPrinter;  // Holds reference to the adaptee

public:
    EuropeanPrinterAdapter(EuropeanPrinter* printer) : europeanPrinter(printer) {}

    // ADAPT: Convert printInDollars() call → printInEuros() call
    // Also convert the value (USD to EUR)
    void printInDollars(double amountUSD) override{
        double amountEUR = amountUSD * 0.85;  // Convert USD to EUR
        europeanPrinter->printInEuros(amountEUR);  // Call the actual method
    }
};

// ===== STEP 2: Adapter for JapanesePrinter =====
class JapanesePrinterAdapter : public PrinterInterface{
private:
    JapanesePrinter* japanesePrinter;

public:
    JapanesePrinterAdapter(JapanesePrinter* printer) : japanesePrinter(printer) {}

    // ADAPT: Convert printInDollars() call → printInYen() call
    void printInDollars(double amountUSD) override{
        double amountJPY = amountUSD * 150.0;  // Convert USD to JPY
        japanesePrinter->printInYen(amountJPY);
    }
};

// ===== Client (now works with ANY printer via interface!) =====
class PaymentSystem{
public:
    void processPayment(PrinterInterface* printer, double amount) {
        std::cout << "Processing payment of $" << amount << std::endl;
        printer->printInDollars(amount);  // Works for ALL adapters!
    }
};

// ===== CLIENT CODE =====
int main() {
    PaymentSystem system;

    // American printer — no adapter needed, already compatible
    AmericanPrinter usPrinter;
    std::cout << "=== American Printer ===" << std::endl;
    system.processPayment(&usPrinter, 100.0);

    std::cout << std::endl;

    // European printer — WRAPPED with adapter
    EuropeanPrinter euPrinter;
    EuropeanPrinterAdapter euAdapter(&euPrinter);
    std::cout << "=== European Printer (via Adapter) ===" << std::endl;
    system.processPayment(&euAdapter, 100.0);

    std::cout << std::endl;

    // Japanese printer — WRAPPED with adapter
    JapanesePrinter jpPrinter;
    JapanesePrinterAdapter jpAdapter(&jpPrinter);
    std::cout << "=== Japanese Printer (via Adapter) ===" << std::endl;
    system.processPayment(&jpAdapter, 100.0);

}


/*
===============================================================================
                     Adapter Pattern: Before vs After
===============================================================================

BEFORE (Without Adapter)

                         PaymentSystem
                                │
                  Expects: printInDollars()
                                │
                                ▼
                        EuropeanPrinter
                      printInEuros(...)
                                │
                                ✗
                     Interface Mismatch!

Result:
  ❌ Incompatible interfaces.
  ❌ PaymentSystem cannot use EuropeanPrinter directly.
  ❌ Client must be modified or the existing class must change.


===============================================================================

AFTER (Using Adapter Pattern)

                         PaymentSystem
                                │
                  Expects: printInDollars()
                                │
                                ▼
                  EuropeanPrinterAdapter
                  implements printInDollars()
                                │
                                ▼
                  Converts request if necessary
                                │
                                ▼
                 EuropeanPrinter::printInEuros()

Result:
  ✅ Compatible through the Adapter.
  ✅ Existing classes remain unchanged.
  ✅ Client continues using the expected interface.


===============================================================================
                        Adapter Internals
===============================================================================

Client
  │
  ▼
Adapter.printInDollars($100)
  │
  ▼
Convert Currency
($100 → €85)
  │
  ▼
EuropeanPrinter.printInEuros(€85)

The client never knows that a conversion or adaptation is taking place.
It simply interacts with the interface it expects.

===============================================================================
*/