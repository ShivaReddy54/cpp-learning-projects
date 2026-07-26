/*

Intent: Allow an object to alter its behavior when its internal state changes. The object will appear to change its class.

EX: A vending machine. When it has no coin → you can only insert coins. When it has a coin → you can select a product. 
    When it's dispensing → you can't do anything until product is delivered. When it's sold out → you can't select products. 
    The machine's behavior changes based on its current state.

*/

#include <iostream>
#include <string>

// ===== Vending machine with state logic in one class =====
class VendingMachine{
private:
    int coinCount;
    bool hasCoin;
    bool soldOut;

public:
    VendingMachine() : coinCount(0), hasCoin(false), soldOut(false) {}

    void insertCoin(){
        if(soldOut) std::cout << "VendingMachine: Sold out! Can't insert coin." << std::endl;
        else if (hasCoin) std::cout << "VendingMachine: Already has coin! Can't insert another." << std::endl;
        else{
            hasCoin = true;
            coinCount++;
            std::cout << "VendingMachine: Coin inserted." << std::endl;
        }
    }

    void ejectCoin(){
        if(!hasCoin) std::cout << "VendingMachine: No coin to eject." << std::endl;
        else{
            hasCoin = false;
            coinCount--;
            std::cout << "VendingMachine: Coin ejected." << std::endl;
        }
    }

    void selectProduct(){
        if(soldOut) std::cout << "VendingMachine: Sold out! Can't select product." << std::endl;
        else if (!hasCoin) std::cout << "VendingMachine: Insert coin first!" << std::endl;
        else{
            hasCoin = false;
            std::cout << "VendingMachine: Product dispensed!" << std::endl;
            // After dispensing, check if sold out
            if(coinCount == 0) soldOut = true;
        }
    }

    void refill(int count){
        coinCount = count;
        soldOut = false;
        std::cout << "VendingMachine: Refilled with " << count << " products." << std::endl;
    }
};

int main() {
    VendingMachine machine;

    // PROBLEM: All state logic crammed into ONE class with MANY if-else checks!
    // Every method checks hasCoin, soldOut — tangled logic!
    // Adding a new state (e.g., "DispensingState") requires modifying EVERY method!
    // Hard to understand — which behavior belongs to which state?

    std::cout << "=== No coin state ===" << std::endl;
    machine.selectProduct();  // Can't select
    machine.ejectCoin();      // Can't eject
    machine.insertCoin();     // Can insert

    std::cout << std::endl;

    std::cout << "=== Has coin state ===" << std::endl;
    machine.selectProduct();  // Can select!
    machine.insertCoin();     // Can't insert another

    std::cout << std::endl;

    std::cout << "=== Sold out state ===" << std::endl;
    machine.insertCoin();     // Can't insert

}

/*
===============================================================================
                     Problems Without the State Pattern
===============================================================================

All state-dependent behavior is implemented inside a single class.

                           VendingMachine
                                 │
        ┌────────────────────────┼────────────────────────┐
        │                        │                        │
        ▼                        ▼                        ▼

    insertCoin()             ejectCoin()          selectProduct()

        if (soldOut)             if (!hasCoin)        if (soldOut)
            ...                      ...                  ...
        else if (hasCoin)       else                 else if (!hasCoin)
            ...                      ...                  ...
        else                                           ...
                                                     else
                                                        dispense()


Every operation must manually determine the current state by checking
multiple conditions before performing its action.


Problems:

❌ All States in One Class
   The VendingMachine is responsible for handling the behaviour of
   every possible state, making the class large and difficult to maintain.

❌ Massive if-else Chains
   Every public method contains conditional logic to determine what
   should happen in the current state.

❌ Difficult to Add New States
   Introducing a new state (e.g. Maintenance, OutOfService) requires
   modifying nearly every method in the class.

❌ State-Specific Behaviour is Mixed Together
   Logic for different states is scattered throughout the class instead
   of being encapsulated in separate objects.

❌ Error-Prone
   Forgetting a condition or handling states inconsistently can easily
   introduce bugs.

❌ Scattered State Transitions
   Transitions between states are spread across multiple methods,
   making the workflow difficult to understand and maintain.

===============================================================================
*/