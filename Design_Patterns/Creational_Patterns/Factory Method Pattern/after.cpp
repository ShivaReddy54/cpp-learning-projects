#include <iostream>
#include <string>

// ===== STEP 1: Define a common Product interface =====
class Transport{
public:
    virtual ~Transport() = default;
    virtual void deliver() = 0;
};

// ===== STEP 2: Concrete Products implement the interface =====
class Truck : public Transport{
public:
    void deliver() override{
        std::cout << "Delivering by land in a box" << std::endl;
    }
};

class Ship : public Transport{
public:
    void deliver() override{
        std::cout << "Delivering by sea in a container" << std::endl;
    }
};

// ===== STEP 3: Creator class with Factory Method =====
class Logistics {
public:
    virtual ~Logistics() = default;

    // The Factory Method — subclasses will override this
    virtual Transport* createTransport() = 0;

    // Business logic that USES the product — doesn't care which one
    void planDelivery(){
        Transport* transport = createTransport();  // ← The magic!
        transport->deliver();
        delete transport;
    }
};

// ===== STEP 4: Concrete Creators override the Factory Method =====
class RoadLogistics : public Logistics{
public:
    Transport* createTransport() override{
        return new Truck();  // RoadLogistics decides: create Truck
    }
};

class SeaLogistics : public Logistics{
public:
    Transport* createTransport() override{
        return new Ship();   // SeaLogistics decides: create Ship
    }
};

// ===== CLIENT CODE =====
int main() {
    // Road delivery
    Logistics* roadLogistics = new RoadLogistics();
    roadLogistics->planDelivery();
    delete roadLogistics;

    // Sea delivery
    Logistics* seaLogistics = new SeaLogistics();
    seaLogistics->planDelivery();
    delete seaLogistics;

}



/*

Just add a new Product and a new Creator — NO changes to existing code!

class Airplane : public Transport {
public:
    void deliver() override {
        std::cout << "Delivering by air in a cargo hold" << std::endl;
    }
};

class AirLogistics : public Logistics {
public:
    Transport* createTransport() override {
        return new Airplane();
    }
};

That's it! planDelivery() works automatically!

*/


/*
===============================================================================
                  Factory Method: Before vs After
===============================================================================

BEFORE (Without Factory Method)

                         LogisticsApp
                              │
        ┌─────────────────────┼─────────────────────┐
        │                     │                     │
if(type == "truck")   if(type == "ship")   if(type == "airplane")
        │                     │                     │
   new Truck()          new Ship()          Must MODIFY
                                                LogisticsApp


Problems:
  ❌ Tight Coupling
  ❌ Violates Open/Closed Principle
  ❌ No Common Interface
  ❌ Client is responsible for object creation


===============================================================================

AFTER (Using Factory Method)

                        Logistics (Creator)
                               │
                     planDelivery()
                               │
                 calls createTransport()
                               │
                ┌──────────────┴──────────────┐
                │                             │
        RoadLogistics                  SeaLogistics
                │                             │
          new Truck()                  new Ship()
                │                             │
                └──────────► Transport ◄──────┘
                          (Common Interface)

Result:
  ✅ Loose Coupling
  ✅ Follows Open/Closed Principle
  ✅ Common Transport Interface
  ✅ Object creation delegated to subclasses

===============================================================================
*/