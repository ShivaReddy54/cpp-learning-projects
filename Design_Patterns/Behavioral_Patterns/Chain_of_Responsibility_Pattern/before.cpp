/*

Intent: Pass a request along a chain of handlers.
        Each handler decides either to process the request or to pass it to the next handler in the chain.

EX: You call customer support. Level 1 (basic questions) → if they can't help, escalate to Level 2 (technical) → if still unresolved,
    escalate to Level 3 (manager).
    Each handler decides: "Can I handle this? Yes → handle it. No → pass it up.

*/

#include <iostream>
#include <string>

// ===== Different handler types — no chain, client decides who to call =====
class Level3Support{
public:
    void handleRequest(const std::string& issue, int severity){
        if(severity <= 3) std::cout << "Level 3 (Manager): Handling critical issue - " << issue << std::endl;
        else std::cout << "Level 3: Issue unresolvable - " << issue << std::endl;
    }
};

class Level2Support{
public:
    void handleRequest(const std::string& issue, int severity){
        if(severity <= 2) std::cout << "Level 2: Handling technical issue - " << issue << std::endl;
        else{
            std::cout << "Level 2: Cannot handle severity " << severity << ". Escalating..." << std::endl;
            // PROBLEM: Level2 directly calls Level3 — TIGHT COUPLING!
            Level3Support level3;
            level3.handleRequest(issue, severity);
        }
    }
};

class Level1Support{
public:
    void handleRequest(const std::string& issue, int severity){
        if(severity <= 1) std::cout << "Level 1: Handling basic issue - " << issue << std::endl;
        else{
            std::cout << "Level 1: Cannot handle severity " << severity << ". Escalating..." << std::endl;
            // PROBLEM: Level1 directly calls Level2 — TIGHT COUPLING!
            Level2Support level2;
            level2.handleRequest(issue, severity);
        }
    }
};

// ===== CLIENT CODE =====
int main() {
    // PROBLEM 1: Client must know which handler to call first
    // PROBLEM 2: Handlers are tightly coupled — each directly creates and calls the next
    // PROBLEM 3: If handler order changes, must modify handler classes
    // PROBLEM 4: Can't dynamically add/remove handlers
    // PROBLEM 5: No flexibility in the chain

    Level1Support level1;

    std::cout << "=== Severity 1 (Basic) ===" << std::endl;
    level1.handleRequest("Password reset", 1);

    std::cout << std::endl;

    std::cout << "=== Severity 2 (Technical) ===" << std::endl;
    level1.handleRequest("Server crash", 2);

    std::cout << std::endl;

    std::cout << "=== Severity 3 (Critical) ===" << std::endl;
    level1.handleRequest("Data breach", 3);

}

/*
===============================================================================
           Problems Without the Chain of Responsibility Pattern
===============================================================================

Request Flow

    Level1  ─────────►  Level2  ─────────►  Level3
       │                  │                  │
       └── Direct Call ───┴── Direct Call ──┘

Each handler directly knows about and invokes the next handler in the chain,
creating a rigid and tightly coupled processing flow.


Problems:

❌ Tight Coupling
   Every handler is aware of the next handler, making the chain
   highly dependent on the implementation details of other handlers.

❌ No Flexibility
   Handlers create or reference the next handler directly, making
   it difficult to change the processing sequence.

❌ Difficult to Reorder
   Changing the order of handlers requires modifying existing
   handler classes.

❌ Cannot Skip Handlers Dynamically
   There is no easy way to enable, disable, or bypass specific
   handlers at runtime.

❌ Difficult to Extend
   Adding a new handler often requires modifying existing handlers
   instead of simply inserting the new one into the chain.

❌ Client Must Know the Entry Point
   The client needs to know which handler should receive the request
   first, exposing chain implementation details.

❌ Violates Open/Closed Principle
   Existing code must be modified whenever the request-processing
   chain changes.

===============================================================================
*/