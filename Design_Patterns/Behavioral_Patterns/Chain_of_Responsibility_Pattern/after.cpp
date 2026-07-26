#include <iostream>
#include <string>

// ===== STEP 1: Handler interface =====
class Handler{
public:
    virtual ~Handler() = default;

    // Set the next handler in the chain
    virtual Handler* setNext(Handler* handler){
        nextHandler = handler;
        return handler;  // Return next handler for easy chaining
    }

    // Handle the request — or pass it to next handler
    virtual void handleRequest(const std::string& issue, int severity){
        if(nextHandler) nextHandler->handleRequest(issue, severity);
        else std::cout << "End of chain: Issue '" << issue << "' unresolvable (severity " << severity << ")" << std::endl;
    }

protected:
    Handler* nextHandler = nullptr;
};

// ===== STEP 2: Concrete Handlers =====
class Level1Support : public Handler{
public:
    void handleRequest(const std::string& issue, int severity) override{
        if(severity <= 1) std::cout << "Level 1: Handling basic issue - " << issue << std::endl;
        else{
            std::cout << "Level 1: Cannot handle severity " << severity << ". Passing to next..." << std::endl;
            Handler::handleRequest(issue, severity);  // Delegate to next in chain
        }
    }
};

class Level2Support : public Handler{
public:
    void handleRequest(const std::string& issue, int severity) override{
        if(severity <= 2) std::cout << "Level 2: Handling technical issue - " << issue << std::endl;
        else{
            std::cout << "Level 2: Cannot handle severity " << severity << ". Passing to next..." << std::endl;
            Handler::handleRequest(issue, severity);
        }
    }
};

class Level3Support : public Handler{
public:
    void handleRequest(const std::string& issue, int severity) override{
        if(severity <= 3) std::cout << "Level 3 (Manager): Handling critical issue - " << issue << std::endl;
        else{
            std::cout << "Level 3: Cannot handle severity " << severity << ". Passing to next..." << std::endl;
            Handler::handleRequest(issue, severity);
        }
    }
};

// ===== CLIENT CODE =====
int main() {
    // ===== Create handlers =====
    Level1Support level1;
    Level2Support level2;
    Level3Support level3;

    // ===== Build the chain — flexible, configurable! =====
    // Chain: Level1 → Level2 → Level3
    level1.setNext(&level2)->setNext(&level3);

    // Now client only needs to know the FIRST handler!
    // The chain takes care of escalation automatically.

    std::cout << "=== Severity 1 (Basic) ===" << std::endl;
    level1.handleRequest("Password reset", 1);
    // Level1 handles it — never reaches Level2 or Level3

    std::cout << std::endl;

    std::cout << "=== Severity 2 (Technical) ===" << std::endl;
    level1.handleRequest("Server crash", 2);
    // Level1 passes → Level2 handles it

    std::cout << std::endl;

    std::cout << "=== Severity 3 (Critical) ===" << std::endl;
    level1.handleRequest("Data breach", 3);
    // Level1 passes → Level2 passes → Level3 handles it

    std::cout << std::endl;

    std::cout << "=== Severity 4 (Unresolvable) ===" << std::endl;
    level1.handleRequest("Alien invasion", 4);
    // Level1 passes → Level2 passes → Level3 passes → End of chain

    std::cout << std::endl;
    std::cout << std::endl;

    // ===== Chain can be RECONFIGURED dynamically! =====
    std::cout << "=== Reconfigured chain: Skip Level1, start at Level2 ===" << std::endl;
    level2.handleRequest("Simple query", 1);
    // Level2 handles it directly — Level1 not involved at all!

    std::cout << std::endl;

    // ===== Add a new handler easily =====
    class EmergencySupport : public Handler{
    public:
        void handleRequest(const std::string& issue, int severity) override{
            if(severity >= 5) std::cout << "Emergency: Handling extreme issue - " << issue << std::endl;
            else Handler::handleRequest(issue, severity);
        }
    };

    EmergencySupport emergency;
    // New chain: Level1 → Level2 → Level3 → Emergency
    level1.setNext(&level2)->setNext(&level3)->setNext(&emergency);

    std::cout << "=== Severity 5 (Emergency — NEW handler!) ===" << std::endl;
    level1.handleRequest("Nuclear meltdown", 5);

}

/*
===============================================================================
             Chain of Responsibility: Before vs After
===============================================================================

BEFORE (Without Chain of Responsibility)

    Level1  ─────────►  Creates Level2
                           │
                           ▼
                       Creates Level3

Every handler directly creates or references the next handler.

Result:
    Level1 knows about Level2.
    Level2 knows about Level3.

This creates a rigid, hardcoded request-processing chain.


Problems:
  ❌ Tight coupling between handlers.
  ❌ Chain order is hardcoded.
  ❌ Reordering handlers requires code changes.
  ❌ Adding a new handler means modifying existing handlers.
  ❌ No runtime flexibility.


===============================================================================

AFTER (Using Chain of Responsibility)

                Handler Interface
         (setNext() + handleRequest())
                        │
                        ▼
      Level1 ──► Level2 ──► Level3
           ▲         ▲          ▲
           │         │          │
      Linked using setNext()

The client builds the chain:

    level1.setNext(level2);
    level2.setNext(level3);

Each handler only knows:

    "I have a next handler."

If it can process the request:
    → Handle it.

Otherwise:
    → Forward it to the next handler.

The handler never needs to know the concrete type of the next handler.


Result:
  ✅ Loose coupling between handlers.
  ✅ Chain is built externally by the client.
  ✅ Handlers can be reordered easily.
  ✅ New handlers can be inserted without modifying existing ones.
  ✅ Chain can be configured dynamically at runtime.

===============================================================================
*/