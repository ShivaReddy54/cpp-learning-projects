#include <iostream>
#include <string>
#include <vector>

// ===== STEP 1: Visitor interface — declares visit() for each element type =====
class Visitor{
public:
    virtual ~Visitor() = default;
    virtual void visitBook(class Book* book) = 0;
    virtual void visitFruit(class Fruit* fruit) = 0;
    virtual void visitElectronics(class Electronics* electronics) = 0;
};

// ===== STEP 2: Element interface — items must accept visitors =====
class ItemElement{
public:
    virtual ~ItemElement() = default;
    virtual void accept(Visitor* visitor) = 0;
};

// ===== STEP 3: Concrete Elements — each implements accept() =====
class Book : public ItemElement{
private:
    std::string title;
    double price;

public:
    Book(const std::string& t, double p) : title(t), price(p) {}
    std::string getTitle() { return title; }
    double getPrice() { return price; }

    void accept(Visitor* visitor) override {
        visitor->visitBook(this);  // "I'm a Book — visitor handles me as a Book"
    }
};

class Fruit : public ItemElement{
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

    void accept(Visitor* visitor) override{
        visitor->visitFruit(this);  // "I'm a Fruit — visitor handles me as a Fruit"
    }
};

class Electronics : public ItemElement{
private:
    std::string brand;
    double price;

public:
    Electronics(const std::string& b, double p) : brand(b), price(p) {}
    std::string getBrand() { return brand; }
    double getPrice() { return price; }

    void accept(Visitor* visitor) override{
        visitor->visitElectronics(this);  // "I'm Electronics — visitor handles me"
    }
};

// ===== STEP 4: Concrete Visitors — each encapsulates ONE operation =====

// Visitor 1: Price Calculator
class PriceCalculatorVisitor : public Visitor{
private:
    double totalPrice;

public:
    PriceCalculatorVisitor() : totalPrice(0) {}

    void visitBook(Book* book) override{
        totalPrice += book->getPrice();
    }

    void visitFruit(Fruit* fruit) override{
        totalPrice += fruit->getTotalPrice();
    }

    void visitElectronics(Electronics* electronics) override{
        totalPrice += electronics->getPrice();
    }

    double getTotalPrice() { return totalPrice; }
};

// Visitor 2: Tax Calculator
class TaxCalculatorVisitor : public Visitor{
private:
    double totalTax;

public:
    TaxCalculatorVisitor() : totalTax(0) {}

    void visitBook(Book* book) override{
        double tax = book->getPrice() * 0.0;     // Books: 0% tax
        totalTax += tax;
        std::cout << "  Book '" << book->getTitle() << "' tax: $0 (0%)" << std::endl;
    }

    void visitFruit(Fruit* fruit) override{
        double tax = fruit->getTotalPrice() * 0.05;  // Fruits: 5% tax
        totalTax += tax;
        std::cout << "  Fruit '" << fruit->getName() << "' tax: $" << tax << " (5%)" << std::endl;
    }

    void visitElectronics(Electronics* electronics) override{
        double tax = electronics->getPrice() * 0.18;  // Electronics: 18% tax
        totalTax += tax;
        std::cout << "  Electronics '" << electronics->getBrand() << "' tax: $" << tax << " (18%)" << std::endl;
    }

    double getTotalTax() { return totalTax; }
};

// Visitor 3: Receipt Generator
class ReceiptGeneratorVisitor : public Visitor{
public:
    void visitBook(Book* book) override{
        std::cout << "  Book: " << book->getTitle() << " - $" << book->getPrice() << std::endl;
    }

    void visitFruit(Fruit* fruit) override{
        std::cout << "  Fruit: " << fruit->getName() << " " << fruit->getWeight() 
                  << "kg @ $" << fruit->getPricePerKg() << "/kg - $" << fruit->getTotalPrice() << std::endl;
    }

    void visitElectronics(Electronics* electronics) override{
        std::cout << "  Electronics: " << electronics->getBrand() << " - $" << electronics->getPrice() << std::endl;
    }
};

// Visitor 4: Discount Calculator — NEW operation, NO modification to items!
class DiscountCalculatorVisitor : public Visitor{
private:
    double totalDiscount;

public:
    DiscountCalculatorVisitor() : totalDiscount(0) {}

    void visitBook(Book* book) override{
        double discount = book->getPrice() * 0.10;  // Books: 10% discount
        totalDiscount += discount;
        std::cout << "  Book '" << book->getTitle() << "' discount: $" << discount << " (10%)" << std::endl;
    }

    void visitFruit(Fruit* fruit) override{
        double discount = fruit->getTotalPrice() * 0.0;  // Fruits: no discount
        totalDiscount += discount;
        std::cout << "  Fruit '" << fruit->getName() << "' discount: $0 (0%)" << std::endl;
    }

    void visitElectronics(Electronics* electronics) override{
        double discount = electronics->getPrice() * 0.05;  // Electronics: 5% discount
        totalDiscount += discount;
        std::cout << "  Electronics '" << electronics->getBrand() << "' discount: $" << discount << " (5%)" << std::endl;
    }

    double getTotalDiscount() { return totalDiscount; }
};

// ===== STEP 5: Object Structure — holds items and applies visitor to all =====
class ShoppingCart {
private:
    std::vector<ItemElement*> items;

public:
    void addItem(ItemElement* item){
        items.push_back(item);
    }

    // Apply ANY visitor to ALL items — one method handles all operations!
    void applyVisitor(Visitor* visitor){
        for(ItemElement* item : items) item->accept(visitor);  // Double dispatch!
    }
};

// ===== CLIENT CODE =====
int main() {
    ShoppingCart cart;
    cart.addItem(new Book("Design Patterns", 50.0));
    cart.addItem(new Fruit("Apple", 3.0, 2.5));
    cart.addItem(new Electronics("Laptop", 1000.0));

    // ===== Operation 1: Calculate total price =====
    std::cout << "=== Price Calculation ===" << std::endl;
    PriceCalculatorVisitor priceVisitor;
    cart.applyVisitor(&priceVisitor);
    std::cout << "Total Price: $" << priceVisitor.getTotalPrice() << std::endl;

    std::cout << std::endl;

    // ===== Operation 2: Calculate tax =====
    std::cout << "=== Tax Calculation ===" << std::endl;
    TaxCalculatorVisitor taxVisitor;
    cart.applyVisitor(&taxVisitor);
    std::cout << "Total Tax: $" << taxVisitor.getTotalTax() << std::endl;

    std::cout << std::endl;

    // ===== Operation 3: Generate receipt =====
    std::cout << "=== Receipt ===" << std::endl;
    ReceiptGeneratorVisitor receiptVisitor;
    cart.applyVisitor(&receiptVisitor);

    std::cout << std::endl;

    // ===== Operation 4: Calculate discount — NEW! No item classes modified! =====
    std::cout << "=== Discount Calculation (NEW Operation!) ===" << std::endl;
    DiscountCalculatorVisitor discountVisitor;
    cart.applyVisitor(&discountVisitor);
    std::cout << "Total Discount: $" << discountVisitor.getTotalDiscount() << std::endl;

    std::cout << std::endl;

    // ===== Adding a new operation = just create a new Visitor class =====
    // ===== Item classes (Book, Fruit, Electronics) NEVER change! =====
    // ===== ShoppingCart's applyVisitor() NEVER changes! =====

}

/* 
===========================================================
                BEFORE vs AFTER
               (Visitor Pattern)
===========================================================

BEFORE:
    Add a new operation
        → Modify ShoppingCart
        → Add another traversal loop
        → Touch existing code

    Add a new item type
        → Modify every existing operation

AFTER:
    Add a new operation
        → Create a new Visitor class only

    Add a new item type
        → Add one new visit() method to the Visitor interface
        → Implement it in concrete visitors


Problems Solved:

⚠️ Add operation = modify ShoppingCart
    ✅ Add operation = create a new Visitor class

⚠️ Item classes become full of unrelated logic
    ✅ Business logic lives inside Visitor classes

⚠️ N operations × M item types = repeated traversal code
    ✅ One traversal, many interchangeable visitors

⚠️ ShoppingCart keeps growing
    ✅ ShoppingCart remains simple and unchanged

⚠️ Difficult to extend with new operations
    ✅ New operations added without modifying existing item classes

===========================================================

*/