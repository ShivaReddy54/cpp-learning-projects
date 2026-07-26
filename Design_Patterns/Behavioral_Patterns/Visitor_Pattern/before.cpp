/*

Intent: Represent an operation to be performed on elements of an object structure.
        Visitor lets you define new operations without changing the classes of the elements on which it operates.

EX: A tax inspector (visitor) visits different businesses: Restaurant, Hospital, School.
    The inspector applies different tax rules to each type. The businesses don't change — they just accept the visitor.
    The visitor knows how to handle each business type.
    If you add a new tax rule (new visitor), you don't modify any business — you just create a new visitor type.

*/

#include <iostream>
#include <string>
#include <vector>

// ===== Different item types in a shopping cart =====
class Book{
private:
    std::string title;
    double price;

public:
    Book(const std::string& t, double p) : title(t), price(p) {}
    std::string getTitle() { return title; }
    double getPrice() { return price; }
};

class Fruit{
private:
    std::string name;
    double pricePerKg;
    double weight;

public:
    Fruit(const std::string& n, double p, double w) : name(n), pricePerKg(p), weight(w) {}
    std::string getName() { return name; }
    double getPricePerKg() { return pricePerKg; }
    double getWeight() { return weight; }
    double getTotalPrice() { return pricePerKg * weight; }
};

class Electronics{
private:
    std::string brand;
    double price;

public:
    Electronics(const std::string& b, double p) : brand(b), price(p) {}
    std::string getBrand() { return brand; }
    double getPrice() { return price; }
};

// ===== PROBLEM: Each operation requires modifying item classes =====
class ShoppingCart{
private:
    std::vector<Book*> books;
    std::vector<Fruit*> fruits;
    std::vector<Electronics*> electronics;

public:
    void addBook(Book* b) { books.push_back(b); }
    void addFruit(Fruit* f) { fruits.push_back(f); }
    void addElectronics(Electronics* e) { electronics.push_back(e); }

    // Operation 1: Calculate total price
    double calculateTotal(){
        double total = 0;
        for (Book* b : books) total += b->getPrice();
        for (Fruit* f : fruits) total += f->getTotalPrice();
        for (Electronics* e : electronics) total += e->getPrice();
        return total;
    }

    // Operation 2: Calculate tax
    double calculateTax(){
        double tax = 0;
        for (Book* b : books) tax += b->getPrice() * 0.0;    // Books: 0% tax
        for (Fruit* f : fruits) tax += f->getTotalPrice() * 0.05; // Fruits: 5% tax
        for (Electronics* e : electronics) tax += e->getPrice() * 0.18; // Electronics: 18% tax
        return tax;
    }

    // Operation 3: Generate receipt
    void generateReceipt(){
        std::cout << "=== RECEIPT ===" << std::endl;
        for (Book* b : books) std::cout << "Book: " << b->getTitle() << " - $" << b->getPrice() << std::endl;
        for (Fruit* f : fruits) std::cout << "Fruit: " << f->getName() << " " << f->getWeight() << "kg - $" << f->getTotalPrice() << std::endl;
        for (Electronics* e : electronics) std::cout << "Electronics: " << e->getBrand() << " - $" << e->getPrice() << std::endl;
    }

    // PROBLEM: Adding a NEW operation (e.g., discount calculation)
    // requires adding a NEW method with loops for EVERY item type!
    // The cart grows with every new operation!
    // Each operation must know about ALL item types!
};

int main() {
    ShoppingCart cart;
    cart.addBook(new Book("Design Patterns", 50.0));
    cart.addFruit(new Fruit("Apple", 3.0, 2.5));
    cart.addElectronics(new Electronics("Laptop", 1000.0));

    cart.generateReceipt();
    std::cout << "Total: $" << cart.calculateTotal() << std::endl;
    std::cout << "Tax: $" << cart.calculateTax() << std::endl;

    // PROBLEM: Want to add "Discount" operation? Must add another method
    // with loops for books, fruits, electronics!
    // PROBLEM: Want to add "ShippingCost" operation? Same issue!
    // PROBLEM: Adding new item type (Clothes) requires modifying ALL methods!

}

/*
===========================================================
          PROBLEMS WITH THIS APPROACH (BEFORE)
===========================================================

ShoppingCart methods:

    calculateTotal()
        └── Loop through Books
        └── Loop through Fruits
        └── Loop through Electronics

    calculateTax()
        └── Loop through Books
        └── Loop through Fruits
        └── Loop through Electronics

    generateReceipt()
        └── Loop through Books
        └── Loop through Fruits
        └── Loop through Electronics

    discountCalc()      ← NEW operation
        └── Loop through Books
        └── Loop through Fruits
        └── Loop through Electronics

    shippingCost()      ← NEW operation
        └── Loop through Books
        └── Loop through Fruits
        └── Loop through Electronics


Every new operation repeats the same traversal logic!


Problems:

⚠️ Code duplication
    • Every operation traverses the same collection again.

⚠️ Poor maintainability
    • Adding a new operation means writing another loop
      over all item types.

⚠️ Violates Open/Closed Principle
    • Existing classes must be modified whenever a new
      operation is introduced.

⚠️ Business logic scattered
    • Calculation, taxation, discounts, shipping, and
      reporting logic are spread across multiple methods.

⚠️ Hard to extend
    • New features require touching existing code instead
      of simply adding a new operation.

⚠️ Item classes become bloated
    • As more operations are added, classes grow larger
      and harder to maintain.

===========================================================
*/