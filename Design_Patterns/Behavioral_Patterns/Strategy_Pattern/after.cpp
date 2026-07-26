#include <iostream>
#include <string>
#include <vector>

// ===== STEP 1: Strategy interface =====
class RouteStrategy{
public:
    virtual ~RouteStrategy() = default;
    virtual std::vector<std::string> calculateRoute(const std::string& start, const std::string& end) = 0;
};

// ===== STEP 2: Concrete Strategies — each algorithm encapsulated =====
class FastestRouteStrategy : public RouteStrategy{
public:
    std::vector<std::string> calculateRoute(const std::string& start, const std::string& end) override{
        std::cout << "FastestRoute: Calculating fastest route from " << start << " to " << end 
                  << " (Time: 20 min, Distance: 30 km)" << std::endl;
        return {"Highway A", "Express B", "Direct C"};
    }
};

class ShortestRouteStrategy : public RouteStrategy{
public:
    std::vector<std::string> calculateRoute(const std::string& start, const std::string& end) override{
        std::cout << "ShortestRoute: Calculating shortest route from " << start << " to " << end 
                  << " (Time: 35 min, Distance: 15 km)" << std::endl;
        return {"Side Street 1", "Lane 2", "Path 3"};
    }
};

class EcoRouteStrategy : public RouteStrategy{
public:
    std::vector<std::string> calculateRoute(const std::string& start, const std::string& end) override{
        std::cout << "EcoRoute: Calculating eco-friendly route from " << start << " to " << end 
                  << " (Time: 40 min, Distance: 20 km, CO2: Low)" << std::endl;
        return {"Bike Path X", "Green Way Y", "Low Emission Z"};
    }
};

class AvoidTollsStrategy : public RouteStrategy{
public:
    std::vector<std::string> calculateRoute(const std::string& start, const std::string& end) override{
        std::cout << "AvoidTollsRoute: Calculating toll-free route from " << start << " to " << end 
                  << " (Time: 45 min, Distance: 25 km, Tolls: $0)" << std::endl;
        return {"Free Road F1", "Local Road L2", "No Toll N3"};
    }
};

// ===== STEP 3: Context — holds a strategy and delegates =====
class NavigationApp{
private:
    RouteStrategy* strategy;  // Current strategy — can be swapped!

public:
    NavigationApp(RouteStrategy* initialStrategy) : strategy(initialStrategy) {}

    // Swap strategy at RUNTIME!
    void setStrategy(RouteStrategy* newStrategy){
        strategy = newStrategy;
    }

    std::vector<std::string> calculateRoute(const std::string& start, const std::string& end){
        // Just DELEGATE to strategy — no if-else!
        return strategy->calculateRoute(start, end);
    }
};

// ===== CLIENT CODE =====
int main() {
    // Create strategies
    FastestRouteStrategy fastest;
    ShortestRouteStrategy shortest;
    EcoRouteStrategy eco;
    AvoidTollsStrategy avoidTolls;

    // ===== Create app with initial strategy =====
    NavigationApp app(&fastest);
    std::cout << "=== Fastest Route ===" << std::endl;
    app.calculateRoute("Home", "Airport");

    std::cout << std::endl;

    // ===== Swap strategy at runtime! =====
    std::cout << "=== Switch to Shortest Route ===" << std::endl;
    app.setStrategy(&shortest);
    app.calculateRoute("Home", "Airport");

    std::cout << std::endl;

    std::cout << "=== Switch to Eco Route ===" << std::endl;
    app.setStrategy(&eco);
    app.calculateRoute("Home", "Airport");

    std::cout << std::endl;

    // ===== New strategy added WITHOUT modifying NavigationApp! =====
    std::cout << "=== Switch to Avoid Tolls (NEW strategy!) ===" << std::endl;
    app.setStrategy(&avoidTolls);
    app.calculateRoute("Home", "Airport");

    // NavigationApp NEVER changed!
    // Just created AvoidTollsStrategy — plugged it in!

}

/*
===========================================================
            STRATEGY PATTERN vs STATE PATTERN
===========================================================

+----------------------+--------------------------------------+--------------------------------------+
| Aspect               | Strategy Pattern                     | State Pattern                        |
+----------------------+--------------------------------------+--------------------------------------+
| Purpose              | Swap different algorithms            | Change behavior based on             |
|                      | for the same task                    | the object's current state           |
+----------------------+--------------------------------------+--------------------------------------+
| Who Changes?         | Client explicitly selects            | Object changes its own state         |
|                      | the strategy                         | based on internal conditions         |
+----------------------+--------------------------------------+--------------------------------------+
| Client Knowledge     | Client knows exactly                 | Client does NOT know                 |
|                      | which strategy is being used         | which state is currently active      |
+----------------------+--------------------------------------+--------------------------------------+
| State/Strategy       | Manual                               | Automatic                            |
| Transitions          | Client replaces the strategy         | State objects handle transitions     |
|                      | when needed                          | internally                           |
+----------------------+--------------------------------------+--------------------------------------+

Key Difference:

Strategy:
    Client decides HOW the work should be done.
        NavigationApp
              |
              └── setStrategy(new FastestRouteStrategy())

State:
    Object decides HOW it should behave based on its current state.
        VendingMachine
              |
              └── currentState = new HasCoinState()

Strategy = Different algorithms for the same problem.
State    = Different behaviors for the same object.

===========================================================
*/