/*

Intent: Define a family of algorithms, encapsulate each one, and make them interchangeable.
        Strategy lets the algorithm vary independently from the clients that use it.

EX: A navigation app (Google Maps). You want to get to the airport.
    You can choose: fastest route, shortest route, or eco-friendly route.
    The app doesn't change — just the strategy (algorithm) for finding the route.
    You swap strategies at runtime.

*/

#include <iostream>
#include <string>
#include <vector>

class NavigationApp{
private:
    std::string strategy;

public:
    void setStrategy(const std::string& s) { strategy = s; }

    // PROBLEM: All algorithms crammed in ONE class with if-else!
    std::vector<std::string> calculateRoute(const std::string& start, const std::string& end) {
        std::vector<std::string> route;

        if(strategy == "fastest"){
            std::cout << "Calculating FASTEST route from " << start << " to " << end << std::endl;
            route = {"Highway A", "Express B", "Direct C"};
        }
        else if(strategy == "shortest"){
            std::cout << "Calculating SHORTEST route from " << start << " to " << end << std::endl;
            route = {"Side Street 1", "Lane 2", "Path 3"};
        }
        else if(strategy == "eco"){
            std::cout << "Calculating ECO route from " << start << " to " << end << std::endl;
            route = {"Bike Path X", "Green Way Y", "Low Emission Z"};
        }
        // PROBLEM: Adding a new strategy = modify this class!
        // PROBLEM: All algorithms mixed together — hard to maintain
        // PROBLEM: Can't swap algorithms cleanly
        // PROBLEM: Testing one algorithm requires testing the whole class

        return route;
    }
};

int main() {
    NavigationApp app;

    app.setStrategy("fastest");
    auto route1 = app.calculateRoute("Home", "Airport");

    app.setStrategy("shortest");
    auto route2 = app.calculateRoute("Home", "Airport");

    app.setStrategy("eco");
    auto route3 = app.calculateRoute("Home", "Airport");

    // PROBLEM: Want "avoid tolls" strategy? Must modify NavigationApp!
    // PROBLEM: All strategy logic in one monolithic method!
    // PROBLEM: Strategy is just a string — no type safety!

}

/*

========================== BEFORE (Without Strategy Pattern) ==========================

NavigationApp.calculateRoute():
    if strategy == "fastest"  → algorithm 1
    if strategy == "shortest" → algorithm 2
    if strategy == "eco"      → algorithm 3

    Adding a new strategy (e.g., "avoidTolls")
    requires modifying this existing method!

---------------------------------------------------------------------------------------

⚠️ All routing algorithms are implemented inside a single class.

⚠️ Strategy selection relies on large if-else or switch statements.

⚠️ Adding a new routing strategy requires modifying existing code
   (violates the Open/Closed Principle).

⚠️ Different algorithms cannot be tested independently.

⚠️ No type safety — strategies are represented as plain strings,
   making typos and invalid values possible.

*/