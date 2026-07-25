#include <iostream>
#include <string>

// ===== STEP 1: Define abstract Product interfaces =====
class Chair{
public:
    virtual ~Chair() = default;
    virtual void sitOn() = 0;
    virtual void hasStyle() = 0;
};

class Sofa{
public:
    virtual ~Sofa() = default;
    virtual void lieOn() = 0;
};

class CoffeeTable{
public:
    virtual ~CoffeeTable() = default;
    virtual void placeItems() = 0;
};

// ===== STEP 2: Concrete Products for Victorian family =====
class VictorianChair : public Chair{
public:
    void sitOn() override { std::cout << "Sitting on a Victorian chair" << std::endl; }
    void hasStyle() override { std::cout << "Ornate, carved wood" << std::endl; }
};

class VictorianSofa : public Sofa{
public:
    void lieOn() override { std::cout << "Lying on a Victorian sofa" << std::endl; }
};

class VictorianCoffeeTable : public CoffeeTable{
public:
    void placeItems() override { std::cout << "Placing items on Victorian coffee table" << std::endl; }
};

// ===== STEP 3: Concrete Products for Modern family =====
class ModernChair : public Chair{
public:
    void sitOn() override { std::cout << "Sitting on a Modern chair" << std::endl; }
    void hasStyle() override { std::cout << "Sleek, minimal metal" << std::endl; }
};

class ModernSofa : public Sofa{
public:
    void lieOn() override { std::cout << "Lying on a Modern sofa" << std::endl; }
};

class ModernCoffeeTable : public CoffeeTable{
public:
    void placeItems() override { std::cout << "Placing items on Modern coffee table" << std::endl; }
};

// ===== STEP 4: Abstract Factory interface =====
class FurnitureFactory{
public:
    virtual ~FurnitureFactory() = default;
    virtual Chair* createChair() = 0;
    virtual Sofa* createSofa() = 0;
    virtual CoffeeTable* createCoffeeTable() = 0;
};

// ===== STEP 5: Concrete Factory for Victorian family =====
class VictorianFurnitureFactory : public FurnitureFactory{
public:
    Chair* createChair() override { return new VictorianChair(); }
    Sofa* createSofa() override { return new VictorianSofa(); }
    CoffeeTable* createCoffeeTable() override { return new VictorianCoffeeTable(); }
};

// ===== STEP 6: Concrete Factory for Modern family =====
class ModernFurnitureFactory : public FurnitureFactory{
public:
    Chair* createChair() override { return new ModernChair(); }
    Sofa* createSofa() override { return new ModernSofa(); }
    CoffeeTable* createCoffeeTable() override { return new ModernCoffeeTable(); }
};

// ===== CLIENT CODE =====
class FurnitureShop{
private:
    Chair* chair;
    Sofa* sofa;
    CoffeeTable* table;

public:
    // The shop receives a factory — it doesn't know or care which family
    FurnitureShop(FurnitureFactory* factory){
        chair = factory->createChair();
        sofa = factory->createSofa();
        table = factory->createCoffeeTable();
    }

    void demonstrateFurniture(){
        chair->sitOn();
        chair->hasStyle();
        sofa->lieOn();
        table->placeItems();
    }

    ~FurnitureShop(){
        delete chair;
        delete sofa;
        delete table;
    }
};

int main(){
    // Victorian set — CONSISTENT family guaranteed!
    FurnitureFactory* victorianFactory = new VictorianFurnitureFactory();
    FurnitureShop shop1(victorianFactory);
    std::cout << "=== Victorian Furniture Set ===" << std::endl;
    shop1.demonstrateFurniture();

    std::cout << std::endl;

    // Modern set — CONSISTENT family guaranteed!
    FurnitureFactory* modernFactory = new ModernFurnitureFactory();
    FurnitureShop shop2(modernFactory);
    std::cout << "=== Modern Furniture Set ===" << std::endl;
    shop2.demonstrateFurniture();

    delete victorianFactory;
    delete modernFactory;

}

/*

class ArtDecoChair : public Chair {
public:
    void sitOn() override { std::cout << "Sitting on an Art Deco chair" << std::endl; }
    void hasStyle() override { std::cout << "Geometric, bold colors" << std::endl; }
};

class ArtDecoSofa : public Sofa {
public:
    void lieOn() override { std::cout << "Lying on an Art Deco sofa" << std::endl; }
};

class ArtDecoCoffeeTable : public CoffeeTable {
public:
    void placeItems() override { std::cout << "Placing items on Art Deco coffee table" << std::endl; }
};

class ArtDecoFurnitureFactory : public FurnitureFactory {
public:
    Chair* createChair() override { return new ArtDecoChair(); }
    Sofa* createSofa() override { return new ArtDecoSofa(); }
    CoffeeTable* createCoffeeTable() override { return new ArtDecoCoffeeTable(); }
};

FurnitureShop works without any modification!

*/

/*
===============================================================================
                 Abstract Factory: Before vs After
===============================================================================

BEFORE (Without Abstract Factory)

                           FurnitureShop
                                │
               ┌────────────────┴────────────────┐
               │                                 │
      if(style == "victorian")         if(style == "modern")
               │                                 │
       new VictorianChair()             new ModernChair()
       new VictorianSofa()              new ModernSofa()
       new VictorianCoffeeTable()       new ModernCoffeeTable()
               │                                 │
               └──────────────┬──────────────────┘
                              │
               Client is responsible for creating
               every concrete product.


Problems:
  ❌ Furniture styles can be mixed accidentally.
  ❌ No common interfaces for product families.
  ❌ Must modify FurnitureShop to add a new style.
  ❌ Client knows every concrete product class.


===============================================================================

AFTER (Using Abstract Factory)

                      FurnitureFactory (Abstract)
                               │
              ┌────────────────┼────────────────┐
              │                │                │
        createChair()    createSofa()   createCoffeeTable()
                               │
                 ┌─────────────┴─────────────┐
                 │                           │
        VictorianFactory             ModernFactory
                 │                           │
      VictorianChair()              ModernChair()
      VictorianSofa()               ModernSofa()
      VictorianCoffeeTable()        ModernCoffeeTable()

               All products created by the same factory
               belong to the same furniture family.


Result:
  ✅ Consistent product families guaranteed.
  ✅ Common interfaces (Chair, Sofa, CoffeeTable).
  ✅ Easy to add new styles by creating another factory.
  ✅ Existing client code remains unchanged.

===============================================================================
*/

/*
===============================================================================
             Factory Method vs Abstract Factory
===============================================================================

+----------------------+-----------------------------------------+----------------------------------------------+
| Feature              | Factory Method                          | Abstract Factory                             |
+----------------------+-----------------------------------------+----------------------------------------------+
| Creates              | One product                             | A family of related products                 |
+----------------------+-----------------------------------------+----------------------------------------------+
| Returns              | A single product type                   | Multiple related product types               |
+----------------------+-----------------------------------------+----------------------------------------------+
| Primary Mechanism    | Inheritance                             | Composition                                  |
|                      | (subclasses override the factory        | (client uses a factory object that creates   |
|                      | method)                                 | an entire product family)                    |
+----------------------+-----------------------------------------+----------------------------------------------+
| Scope                | Creates one product at a time           | Ensures an entire product family is          |
|                      |                                         | created consistently                         |
+----------------------+-----------------------------------------+----------------------------------------------+

===============================================================================
                         Quick Rule of Thumb
===============================================================================

Factory Method
    → "Which single object should I create?"

Abstract Factory
    → "Which family of related objects should I create?"

===============================================================================
*/