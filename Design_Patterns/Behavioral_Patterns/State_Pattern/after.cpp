#include <iostream>
#include <string>

// ===== STEP 1: State interface =====
class VendingState{
public:
    virtual ~VendingState() = default;
    virtual void insertCoin() = 0;
    virtual void ejectCoin() = 0;
    virtual void selectProduct() = 0;
    virtual std::string getStateName() = 0;
};

// ===== STEP 2: Context — delegates behavior to current state =====
class VendingMachine{
private:
    VendingState* currentState;
    int productCount;

    // All possible states (pre-created)
    VendingState* noCoinState;
    VendingState* hasCoinState;
    VendingState* soldOutState;
    VendingState* dispensingState;

public:
    VendingMachine(int count);
    ~VendingMachine();

    // Context delegates ALL actions to current state!
    void insertCoin() { currentState->insertCoin(); }
    void ejectCoin() { currentState->ejectCoin(); }
    void selectProduct() { currentState->selectProduct(); }

    // State transitions — called by states
    void setState(VendingState* state){
        currentState = state;
        std::cout << "State changed to: " << currentState->getStateName() << std::endl;
    }

    VendingState* getNoCoinState() { return noCoinState; }
    VendingState* getHasCoinState() { return hasCoinState; }
    VendingState* getSoldOutState() { return soldOutState; }
    VendingState* getDispensingState() { return dispensingState; }
    VendingState* getCurrentState() { return currentState; }

    int getProductCount() { return productCount; }
    void setProductCount(int count) { productCount = count; }
    void releaseProduct(){
        if(productCount > 0){
            productCount--;
            std::cout << "Product dispensed! (" << productCount << " remaining)" << std::endl;
        }
    }
};

// ===== STEP 3: Concrete States — each encapsulates behavior for ONE state =====
class NoCoinState : public VendingState{
private:
    VendingMachine* machine;

public:
    NoCoinState(VendingMachine* m) : machine(m) {}

    void insertCoin() override{
        std::cout << "NoCoinState: Coin inserted." << std::endl;
        machine->setState(machine->getHasCoinState());  // Transition!
    }

    void ejectCoin() override{
        std::cout << "NoCoinState: No coin to eject." << std::endl;
        // Stay in same state
    }

    void selectProduct() override{
        std::cout << "NoCoinState: Insert coin first!" << std::endl;
        // Stay in same state
    }

    std::string getStateName() override { return "NoCoin"; }
};

class HasCoinState : public VendingState{
private:
    VendingMachine* machine;

public:
    HasCoinState(VendingMachine* m) : machine(m) {}

    void insertCoin() override{
        std::cout << "HasCoinState: Already has coin! Can't insert another." << std::endl;
        // Stay in same state
    }

    void ejectCoin() override{
        std::cout << "HasCoinState: Coin ejected." << std::endl;
        machine->setState(machine->getNoCoinState());  // Transition!
    }

    void selectProduct() override{
        std::cout << "HasCoinState: Product selected!" << std::endl;
        machine->releaseProduct();

        if(machine->getProductCount() > 0) machine->setState(machine->getNoCoinState());  // Transition!
        else machine->setState(machine->getSoldOutState()); // Transition!
    }

    std::string getStateName() override { return "HasCoin"; }
};

class SoldOutState : public VendingState{
private:
    VendingMachine* machine;

public:
    SoldOutState(VendingMachine* m) : machine(m) {}

    void insertCoin() override{
        std::cout << "SoldOutState: Sold out! Can't insert coin." << std::endl;
        // Stay in same state
    }

    void ejectCoin() override{
        std::cout << "SoldOutState: No coin to eject." << std::endl;
    }

    void selectProduct() override{
        std::cout << "SoldOutState: Sold out! Can't select product." << std::endl;
    }

    std::string getStateName() override { return "SoldOut"; }
};

class DispensingState : public VendingState{
private:
    VendingMachine* machine;

public:
    DispensingState(VendingMachine* m) : machine(m) {}

    void insertCoin() override{
        std::cout << "DispensingState: Wait! Product is being dispensed." << std::endl;
    }

    void ejectCoin() override{
        std::cout << "DispensingState: Can't eject, product being dispensed." << std::endl;
    }

    void selectProduct() override{
        std::cout << "DispensingState: Already dispensing!" << std::endl;
    }

    std::string getStateName() override { return "Dispensing"; }
};

// ===== VendingMachine constructor =====
VendingMachine::VendingMachine(int count) : productCount(count){
    noCoinState = new NoCoinState(this);
    hasCoinState = new HasCoinState(this);
    soldOutState = new SoldOutState(this);
    dispensingState = new DispensingState(this);

    if(productCount > 0) currentState = noCoinState;
    else currentState = soldOutState;

    std::cout << "VendingMachine initialized with " << productCount << " products." << std::endl;
    std::cout << "Current state: " << currentState->getStateName() << std::endl;
}

VendingMachine::~VendingMachine(){
    delete noCoinState;
    delete hasCoinState;
    delete soldOutState;
    delete dispensingState;
}

// ===== CLIENT CODE =====
int main() {
    VendingMachine machine(2);

    std::cout << std::endl;

    // ===== NoCoin state =====
    std::cout << "=== NoCoin State ===" << std::endl;
    machine.selectProduct();  // Can't select
    machine.ejectCoin();      // Can't eject
    machine.insertCoin();     // Insert → transitions to HasCoin!

    std::cout << std::endl;

    // ===== HasCoin state =====
    std::cout << "=== HasCoin State ===" << std::endl;
    machine.insertCoin();     // Can't insert another
    machine.ejectCoin();      // Eject → transitions to NoCoin!

    std::cout << std::endl;

    // ===== Full cycle: insert → select → dispense → back to NoCoin =====
    std::cout << "=== Full Cycle ===" << std::endl;
    machine.insertCoin();     // → HasCoin
    machine.selectProduct();  // → dispenses → back to NoCoin (1 remaining)

    std::cout << std::endl;

    // ===== Another full cycle → then sold out =====
    std::cout << "=== Second Cycle (last product) ===" << std::endl;
    machine.insertCoin();     // → HasCoin
    machine.selectProduct();  // → dispenses → SoldOut (0 remaining)

    std::cout << std::endl;

    // ===== SoldOut state =====
    std::cout << "=== SoldOut State ===" << std::endl;
    machine.insertCoin();     // Can't insert!
    machine.selectProduct();  // Can't select!

    // Each state class handles its own behavior cleanly!
    // No if-else scattered everywhere!
    // Adding new state? Just create new VendingState subclass!

}

/*
===============================================================================
                        State Pattern: Before vs After
===============================================================================

BEFORE (Without State Pattern)

                        VendingMachine
                              │
        ┌─────────────────────┼─────────────────────┐
        │                     │                     │
        ▼                     ▼                     ▼

    insertCoin()         ejectCoin()        selectProduct()

        if (soldOut)         if (!hasCoin)      if (soldOut)
            ...                  ...                ...
        else if (hasCoin)   else               else if (!hasCoin)
            ...                  ...                ...
        else                                     else
            insert                                  dispense()

All state-dependent behaviour is handled using if-else logic inside
every method.

Problems:
  ❌ if-else checks everywhere.
  ❌ All states mixed into one class.
  ❌ Difficult to extend with new states.
  ❌ State transitions scattered across multiple methods.


===============================================================================

AFTER (Using State Pattern)

                        VendingMachine
                           (Context)
                               │
      currentState->insertCoin()
      currentState->ejectCoin()
      currentState->selectProduct()
                               │
                               ▼
                    Delegates to current state

                         ┌───────────────────┐
                         │   NoCoinState     │
                         ├───────────────────┤
                         │   HasCoinState    │
                         ├───────────────────┤
                         │   SoldOutState    │
                         ├───────────────────┤
                         │ DispensingState   │
                         └───────────────────┘

Each state class:
    • Implements one clear behaviour.
    • Handles its own state transitions.
    • Eliminates scattered conditional logic.


Result:

  ❌ if-else everywhere            ✅ No if-else — state objects handle behaviour
  ❌ All states in one class       ✅ Each state is a separate class
  ❌ Hard to extend                ✅ Add a new state by creating a new class
  ❌ Scattered transitions         ✅ State transitions encapsulated in state classes

===============================================================================
*/