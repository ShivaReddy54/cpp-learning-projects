#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

// ===== STEP 1: Flyweight — stores ONLY intrinsic (shared) state =====
class CharacterFlyweight{
private:
    // Intrinsic state — shared by all characters of same type
    char symbol;
    std::string font;
    int fontSize;
    std::string color;

public:
    CharacterFlyweight(char sym, const std::string& fnt, int fs, const std::string& clr)
        : symbol(sym), font(fnt), fontSize(fs), color(clr) {
        std::cout << "Creating Flyweight for '" << sym << "' (one-time, expensive)" << std::endl;
    }

    // The extrinsic state (position) is passed in at display time — NOT stored!
    void display(int x, int y){
        std::cout << "Char '" << symbol << "' at (" << x << "," << y
                  << ") font:" << font << " size:" << fontSize << " color:" << color << std::endl;
    }

    char getSymbol() { return symbol; }
};

// ===== STEP 2: Flyweight Factory — creates and manages shared flyweights =====
class CharacterFlyweightFactory{
private:
    std::unordered_map<char, CharacterFlyweight*> flyweights;

public:
    CharacterFlyweight* getFlyweight(char symbol) {
        // If flyweight already exists, RETURN existing one (sharing!)
        if(flyweights.find(symbol) != flyweights.end()){
            std::cout << "Reusing existing Flyweight for '" << symbol << "'" << std::endl;
            return flyweights[symbol];
        }

        // If not, CREATE new flyweight (only once per unique intrinsic state!)
        std::cout << "Creating new Flyweight for '" << symbol << "'" << std::endl;
        CharacterFlyweight* flyweight = new CharacterFlyweight(symbol, "Arial", 12, "Black");
        flyweights[symbol] = flyweight;
        return flyweight;
    }

    int getFlyweightCount() {
        return flyweights.size();
    }

    ~CharacterFlyweightFactory() {
        for (auto& pair : flyweights) {
            delete pair.second;
        }
    }
};

// ===== STEP 3: Context — stores ONLY extrinsic (unique) state + reference to flyweight =====
class CharacterContext{
private:
    // Reference to shared flyweight (intrinsic state)
    CharacterFlyweight* flyweight;

    // Extrinsic state — UNIQUE per character, stored here
    int x;
    int y;

public:
    CharacterContext(CharacterFlyweight* fw, int posX, int posY)
        : flyweight(fw), x(posX), y(posY) {}

    void display(){
        flyweight->display(x, y);  // Flyweight handles intrinsic, we provide extrinsic
    }

    void setPosition(int posX, int posY){
        x = posX;
        y = posY;
    }
};

// ===== CLIENT CODE =====
int main() {
    CharacterFlyweightFactory factory;

    std::vector<CharacterContext*> document;

    std::cout << "=== Creating 10 characters WITH Flyweight ===" << std::endl;
    std::cout << std::endl;

    // Row 1: A A A B B B
    // All 'A' characters share ONE flyweight!
    // All 'B' characters share ONE flyweight!
    document.push_back(new CharacterContext(factory.getFlyweight('A'), 0, 0));
    document.push_back(new CharacterContext(factory.getFlyweight('A'), 1, 0));  // Reuses!
    document.push_back(new CharacterContext(factory.getFlyweight('A'), 2, 0));  // Reuses!
    document.push_back(new CharacterContext(factory.getFlyweight('B'), 3, 0));
    document.push_back(new CharacterContext(factory.getFlyweight('B'), 4, 0));  // Reuses!
    document.push_back(new CharacterContext(factory.getFlyweight('B'), 5, 0));  // Reuses!

    // Row 2: A A B B
    document.push_back(new CharacterContext(factory.getFlyweight('A'), 0, 1));  // Reuses!
    document.push_back(new CharacterContext(factory.getFlyweight('A'), 1, 1));  // Reuses!
    document.push_back(new CharacterContext(factory.getFlyweight('B'), 2, 1));  // Reuses!
    document.push_back(new CharacterContext(factory.getFlyweight('B'), 3, 1));  // Reuses!

    std::cout << std::endl;
    std::cout << "=== Display all characters ===" << std::endl;
    for(CharacterContext* c : document) c->display();

    std::cout << std::endl;
    std::cout << "=== Flyweight Statistics ===" << std::endl;
    std::cout << "Total character positions: " << document.size() << std::endl;
    std::cout << "Flyweight objects created: " << factory.getFlyweightCount() << std::endl;
    std::cout << "Memory saved: Instead of 10 full objects, only 2 flyweights + 10 positions!" << std::endl;

    // For 100,000 characters with 26 letters:
    // BEFORE: 100,000 full objects
    // AFTER: 26 flyweights + 100,000 lightweight contexts
    // MASSIVE memory savings!

    for(CharacterContext* c : document) delete c;

}

/*
===============================================================================
                    Flyweight Pattern: Before vs After
===============================================================================

BEFORE (Without Flyweight)

10 Full Character Objects

Each object stores:

    symbol
    font
    fontSize
    color
    x
    y

Result:
    10 × Full Objects
    = 10 × (Intrinsic + Extrinsic State)

Problems:
  ❌ Every object duplicates the same intrinsic data.
  ❌ Large memory consumption.
  ❌ Every object is expensive to create.


===============================================================================

AFTER (Using Flyweight)

Shared Flyweights (Intrinsic State Only)

    Flyweight 'A'
        • symbol
        • font
        • fontSize
        • color

    Flyweight 'B'
        • symbol
        • font
        • fontSize
        • color


Lightweight Context Objects (Extrinsic State)

Each context stores:

    • Flyweight Reference
    • x
    • y

Memory Usage:

    2 Shared Flyweights
           +
    10 Lightweight Contexts

Instead of:

    10 Full Character Objects


Result:
  ✅ Only shared intrinsic data is stored once.
  ✅ No redundant object state.
  ✅ Only 2 expensive object creations.
  ✅ Remaining objects simply reuse existing flyweights.
  ✅ Significantly lower memory usage.

===============================================================================
*/