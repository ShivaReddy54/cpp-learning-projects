/*

Intent: Provide an interface for creating families of related objects without specifying their concrete classes.

Ex: Imagine you're buying furniture. You want everything to match — Victorian style chair + sofa + table, or Modern style chair + sofa + table. 
    You should never end up with a Victorian chair and a Modern table together. The factory ensures consistency.

*/

#include <iostream>
#include <string>

// ===== Products (no common interface — problem!) =====
class VictorianChair{
public:
    void sitOn() { std::cout << "Sitting on a Victorian chair" << std::endl; }
    void hasStyle() { std::cout << "Ornate, carved wood" << std::endl; }
};

class ModernChair{
public:
    void sitOn() { std::cout << "Sitting on a Modern chair" << std::endl; }
    void hasStyle() { std::cout << "Sleek, minimal metal" << std::endl; }
};

class VictorianSofa{
public:
    void lieOn() { std::cout << "Lying on a Victorian sofa" << std::endl; }
};

class ModernSofa{
public:
    void lieOn() { std::cout << "Lying on a Modern sofa" << std::endl; }
};

class VictorianCoffeeTable{
public:
    void placeItems() { std::cout << "Placing items on Victorian coffee table" << std::endl; }
};

class ModernCoffeeTable {
public:
    void placeItems() { std::cout << "Placing items on Modern coffee table" << std::endl; }
};

// ===== Client directly creates objects =====
class FurnitureShop {
public:
    void createFurnitureSet(const std::string& style){
        // DANGER: Nothing prevents mixing incompatible styles!
        if(style == "victorian"){
            VictorianChair* chair = new VictorianChair();
            VictorianSofa* sofa = new VictorianSofa();
            VictorianCoffeeTable* table = new VictorianCoffeeTable();

            chair->sitOn();
            sofa->lieOn();
            table->placeItems();

            delete chair; delete sofa; delete table;
        }
        else if(style == "modern"){
            ModernChair* chair = new ModernChair();
            ModernSofa* sofa = new ModernSofa();
            ModernCoffeeTable* table = new ModernCoffeeTable();

            chair->sitOn();
            sofa->lieOn();
            table->placeItems();

            delete chair; delete sofa; delete table;
        }

        // PROBLEM: developer could do THIS:
        // VictorianChair* chair = new VictorianChair();
        // ModernSofa* sofa = new ModernSofa();          ← WRONG! Mismatched!
        // VictorianCoffeeTable* table = new VictorianCoffeeTable();
    }
};

int main() {
    FurnitureShop shop;
    shop.createFurnitureSet("victorian");
    shop.createFurnitureSet("modern");

}

/*
===============================================================================
                 Problems Without Abstract Factory
===============================================================================

developer can accidentally create a mismatched furniture set:

            VictorianChair   +   ModernSofa   +   VictorianCoffeeTable
                  ✗                  ✗                     ✗

                    INCONSISTENT PRODUCT FAMILY!

Nothing in the current design prevents mixing products from different styles.
As a result, the application may produce furniture sets that do not belong
together.


Additional Problems:

❌ No Common Interface
   VictorianChair and ModernChair (and other product variants) are unrelated
   classes, so they cannot be treated polymorphically.

❌ Massive if-else Blocks
   Every furniture style requires another branch containing object creation,
   usage, and cleanup logic.

❌ Violates Open/Closed Principle
   Adding a new furniture style (e.g., ArtDeco) requires modifying the existing
   FurnitureShop class.

❌ Cannot Treat Products Uniformly
   Since there is no common base class for each product type, the client must
   know the exact concrete classes.

❌ Code Duplication
   The same object creation pattern is repeated for every furniture style.

❌ Tight Coupling
   FurnitureShop is tightly coupled to every concrete product class.

===============================================================================
*/