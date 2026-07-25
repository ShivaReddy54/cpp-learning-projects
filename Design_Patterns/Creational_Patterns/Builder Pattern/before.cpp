/*

Intent: Separate the construction of a complex object from its representation.
    so that the same construction process can create different representations.

*/

#include <iostream>
#include <string>
#include <vector>

// ===== A complex object with many configuration options =====
class House{
public:
    std::string foundation;
    std::string structure;
    std::string roof;
    std::string interior;
    bool hasGarage;
    bool hasSwimmingPool;
    bool hasGarden;
    bool hasFancyStatues;

    void display(){
        std::cout << "House Configuration:" << std::endl;
        std::cout << "  Foundation: " << foundation << std::endl;
        std::cout << "  Structure: " << structure << std::endl;
        std::cout << "  Roof: " << roof << std::endl;
        std::cout << "  Interior: " << interior << std::endl;
        std::cout << "  Garage: " << (hasGarage ? "Yes" : "No") << std::endl;
        std::cout << "  Swimming Pool: " << (hasSwimmingPool ? "Yes" : "No") << std::endl;
        std::cout << "  Garden: " << (hasGarden ? "Yes" : "No") << std::endl;
        std::cout << "  Fancy Statues: " << (hasFancyStatues ? "Yes" : "No") << std::endl;
    }
};

// ===== Client directly constructs the object =====
// PROBLEM 1: Telescoping Constructor
class HouseConstructor{
public:
    // A constructor with 8 parameters — NIGHTMARE!
    House build(const std::string& foundation,
                const std::string& structure,
                const std::string& roof,
                const std::string& interior,
                bool hasGarage,
                bool hasSwimmingPool,
                bool hasGarden,
                bool hasFancyStatues){
        House house;
        house.foundation = foundation;
        house.structure = structure;
        house.roof = roof;
        house.interior = interior;
        house.hasGarage = hasGarage;
        house.hasSwimmingPool = hasSwimmingPool;
        house.hasGarden = hasGarden;
        house.hasFancyStatues = hasFancyStatues;
        return house;
    }
};

int main() {
    HouseConstructor constructor;

    // PROBLEM: Can you tell which true/false means what?
    House myHouse = constructor.build(
        "Concrete", "Wood", "Tiles", "Modern",
        true, false, true, false
        // ↑ What is true? What is false? Very confusing!
    );
    myHouse.display();

    // PROBLEM: What if I forget to set something?
    // The house might be in an incomplete state

}

/*
===============================================================================
                  Problems Without the Builder Pattern
===============================================================================

Constructing a complex object directly:

House(
    foundation,
    structure,
    roof,
    interior,
    true,
    false,
    true,
    false
);

       ↑           ↑          ↑          ↑         ↑      ↑      ↑      ↑
    Clear values                           WHAT DO THESE BOOLEANS MEAN??


Problems:

❌ Telescoping Constructor
   Constructors become extremely long as more configuration options are added,
   making them difficult to use and maintain.

❌ Poor Readability
   Boolean values such as true, false, true, false provide no indication of
   what each value represents.

❌ Incomplete Objects
   A client can accidentally forget to initialize important fields, leaving
   the object in an invalid or incomplete state.

❌ No Validation
   There is no central place to validate whether the combination of options
   is valid before creating the object.

❌ No Step-by-Step Construction
   All values must be supplied at once, even when building the object
   naturally happens in stages.

❌ Difficult to Support Different Variants
   Building Luxury, Modern, or Eco-friendly houses requires different
   construction logic scattered throughout the client code.

❌ Cannot Reuse Construction Process
   Common construction steps must be rewritten whenever another type of
   house is created.


===============================================================================

*/