#include <iostream>
#include <string>
#include <vector>

// ===== The Product =====
class House{
public:
    std::string foundation;
    std::string structure;
    std::string roof;
    std::string interior;
    bool hasGarage = false;
    bool hasSwimmingPool = false;
    bool hasGarden = false;
    bool hasFancyStatues = false;

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

// ===== STEP 1: Builder Interface =====
class HouseBuilder{
public:
    virtual ~HouseBuilder() = default;
    virtual HouseBuilder& buildFoundation(const std::string& type) = 0;
    virtual HouseBuilder& buildStructure(const std::string& type) = 0;
    virtual HouseBuilder& buildRoof(const std::string& type) = 0;
    virtual HouseBuilder& buildInterior(const std::string& style) = 0;
    virtual HouseBuilder& addGarage() = 0;
    virtual HouseBuilder& addSwimmingPool() = 0;
    virtual HouseBuilder& addGarden() = 0;
    virtual HouseBuilder& addFancyStatues() = 0;
    virtual House getResult() = 0;
};

// ===== STEP 2: Concrete Builder =====
class ConcreteHouseBuilder : public HouseBuilder{
private:
    House house;

public:
    // Each method returns *this so we can CHAIN calls!
    HouseBuilder& buildFoundation(const std::string& type) override{
        house.foundation = type;
        return *this;
    }

    HouseBuilder& buildStructure(const std::string& type) override{
        house.structure = type;
        return *this;
    }

    HouseBuilder& buildRoof(const std::string& type) override{
        house.roof = type;
        return *this;
    }

    HouseBuilder& buildInterior(const std::string& style) override{
        house.interior = style;
        return *this;
    }

    HouseBuilder& addGarage() override{
        house.hasGarage = true;
        return *this;
    }

    HouseBuilder& addSwimmingPool() override{
        house.hasSwimmingPool = true;
        return *this;
    }

    HouseBuilder& addGarden() override{
        house.hasGarden = true;
        return *this;
    }

    HouseBuilder& addFancyStatues() override{
        house.hasFancyStatues = true;
        return *this;
    }

    House getResult() override{
        return house;
    }
};

// ===== STEP 3: Director (optional — defines standard construction sequences) =====
class ConstructionDirector{
public:
    House buildLuxuryHouse(HouseBuilder& builder) {
        return builder.buildFoundation("Reinforced Concrete")
                      .buildStructure("Premium Steel Frame")
                      .buildRoof("Marble Dome")
                      .buildInterior("Royal Palace Style")
                      .addGarage()
                      .addSwimmingPool()
                      .addGarden()
                      .addFancyStatues()
                      .getResult();
    }

    House buildSimpleHouse(HouseBuilder& builder) {
        return builder.buildFoundation("Concrete")
                      .buildStructure("Wood")
                      .buildRoof("Tiles")
                      .buildInterior("Modern Minimalist")
                      .addGarden()
                      .getResult();
    }
};

// ===== CLIENT CODE =====
int main() {
    // ===== Method 1: Using the Director (predefined templates) =====
    ConstructionDirector director;
    ConcreteHouseBuilder builder1;

    std::cout << "=== Luxury House (via Director) ===" << std::endl;
    House luxuryHouse = director.buildLuxuryHouse(builder1);
    luxuryHouse.display();

    std::cout << std::endl;

    // ===== Method 2: Building manually step-by-step (maximum flexibility) =====
    ConcreteHouseBuilder builder2;
    std::cout << "=== Custom House (Manual Step-by-Step) ===" << std::endl;
    House customHouse = builder2.buildFoundation("Stone")
                                 .buildStructure("Brick")
                                 .buildRoof("Thatch")
                                 .buildInterior("Cozy Cottage")
                                 .addGarden()
                                 .addGarage()
                                 // NO swimming pool, NO fancy statues — that's fine!
                                 .getResult();
    customHouse.display();

    std::cout << std::endl;

    // ===== Method 3: Director for simple house =====
    ConcreteHouseBuilder builder3;
    std::cout << "=== Simple House (via Director) ===" << std::endl;
    House simpleHouse = director.buildSimpleHouse(builder3);
    simpleHouse.display();

}

/*
===============================================================================
                    Builder Pattern: Before vs After
===============================================================================

BEFORE (Without Builder)

House(
    "Concrete",
    "Wood",
    "Tiles",
    "Modern",
    true,
    false,
    true,
    false
);

Problems:
  ❌ What does each true/false represent?
  ❌ Every value must be provided at once.
  ❌ No validation during construction.
  ❌ Hard to read and maintain.
  ❌ Construction logic cannot be reused.


===============================================================================

AFTER (Using Builder Pattern)

HouseBuilder builder;

builder.buildFoundation("Stone")
       .buildStructure("Brick")
       .buildRoof("Thatch")
       .buildInterior("Cozy Cottage")
       .addGarden()
       .addGarage();

House house = builder.getResult();


Result:
  ✅ Every construction step is clear and self-explanatory.
  ✅ Optional features can simply be skipped.
  ✅ Fluent method chaining improves readability.
  ✅ Construction can be validated before the object is created.
  ✅ A Director can reuse the same construction sequence to build
     multiple house variants.

===============================================================================
*/