/*

Intent: Define an interface for creating objects, but let subclasses decide which class to instantiate. Defer instantiation to subclasses.

Ex: A logistics company. Initially it only delivers by Truck. Later it adds Ship delivery. 
    You don't want to change your core code every time you add a new transport — you want each subclass to decide what transport to create.

*/

#include <iostream>
#include <string>

// ===== Product types =====
class Truck{
public:
    void deliver(){
        std::cout << "Delivering by land in a box" << std::endl;
    }
};

class Ship{
public:
    void deliver(){
        std::cout << "Delivering by sea in a container" << std::endl;
    }
};

// ===== Client that directly creates objects =====
class LogisticsApp {
public:
    void planDelivery(const std::string& type){
        // PROBLEM: Client directly creates objects using 'new'
        //          and must know about ALL concrete types
        if(type == "truck"){
            Truck* truck = new Truck();
            truck->deliver();
            delete truck;
        }
        else if(type == "ship"){
            Ship* ship = new Ship();
            ship->deliver();
            delete ship;
        }
        // If we add "Airplane" tomorrow, we must modify THIS class
        // This violates the Open/Closed Principle!
    }
};

int main() {
    LogisticsApp app;
    app.planDelivery("truck");
    app.planDelivery("ship");
    
}


/*
===============================================================================
                    Problems With This Approach
===============================================================================

                           LogisticsApp
                                │
                         planDelivery(type)
                                │
                    ┌───────────┴───────────┐
                    │                       │
             if(type == "truck")    if(type == "ship")
                    │                       │
               new Truck()            new Ship()
                    │                       │
               deliver()              deliver()
                    │                       │
              delete Truck         delete Ship()


Problems:

⚠️ Tight Coupling
   LogisticsApp knows about every concrete transport class
   (Truck, Ship, etc.).

⚠️ Violates Open/Closed Principle
   Adding a new transport (e.g., Airplane) requires modifying
   the existing LogisticsApp class.

⚠️ Code Duplication
   The object creation pattern (new → use → delete) is repeated
   for every transport type.

⚠️ No Common Interface
   Truck and Ship are unrelated classes, so they cannot be treated
   polymorphically through a common base class.

⚠️ Scattered Object Creation
   Creation logic is spread across multiple if-else blocks, making
   the code harder to maintain.

⚠️ Difficult to Test
   Since the client directly creates concrete objects, it is hard to
   substitute mock or fake implementations during testing.

===============================================================================
*/