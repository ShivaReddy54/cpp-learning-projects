/*

Intent:
Use sharing to support large numbers of similar objects efficiently.
    Separate intrinsic (shared) state from extrinsic (unique) state so that shared objects can be reused.

EX: In a text editor displaying a document with 10,000 characters of the letter 'A' — each 'A' has the same shape, font, size (intrinsic — shared). 
    But each 'A' has a different position on screen (extrinsic — unique).
    Instead of storing 10,000 complete 'A' objects, we store 1 shared 'A' flyweight and 10,000 position references.

*/

#include <iostream>
#include <string>
#include <vector>

// ===== Each character stores ALL its state =====
class Character{
private:
    // Intrinsic state — SAME for all 'A' characters
    char symbol;         // 'A'
    std::string font;    // "Arial"
    int fontSize;        // 12
    std::string color;   // "Black"

    // Extrinsic state — UNIQUE for each character position
    int x;
    int y;

public:
    Character(char sym, const std::string& fnt, int fs, const std::string& clr, int posX, int posY)
        : symbol(sym), font(fnt), fontSize(fs), color(clr), x(posX), y(posY){
        
        // Simulate expensive operation for each character
        std::cout << "Creating character '" << sym << "' with font " << fnt 
                  << " size " << fs << " color " << clr << std::endl;
    }

    void display(){
        std::cout << "Char '" << symbol << "' at (" << x << "," << y 
                  << ") font:" << font << " size:" << fontSize << " color:" << color << std::endl;
    }

    void setPosition(int posX, int posY){
        x = posX;
        y = posY;
    }

    // Memory per character: symbol + font string + fontSize + color string + x + y
    // For 10,000 characters ≈ 10,000 × (all that data)
    // Most of it is REDUNDANT — same font, size, color repeated!
};

int main(){
    std::vector<Character*> document;

    // PROBLEM: Creating 10 characters (imagine 10,000!)
    // Each stores ALL data including redundant intrinsic state
    std::cout << "Creating 10 characters WITHOUT Flyweight:" << std::endl;
    std::cout << std::endl;

    // Row 1: A A A B B B
    document.push_back(new Character('A', "Arial", 12, "Black", 0, 0));
    document.push_back(new Character('A', "Arial", 12, "Black", 1, 0));   // Same intrinsic!
    document.push_back(new Character('A', "Arial", 12, "Black", 2, 0));   // Same intrinsic!
    document.push_back(new Character('B', "Arial", 12, "Black", 3, 0));
    document.push_back(new Character('B', "Arial", 12, "Black", 4, 0));   // Same intrinsic!
    document.push_back(new Character('B', "Arial", 12, "Black", 5, 0));   // Same intrinsic!

    // Row 2: A A B B
    document.push_back(new Character('A', "Arial", 12, "Black", 0, 1));   // Same intrinsic!
    document.push_back(new Character('A', "Arial", 12, "Black", 1, 1));   // Same intrinsic!
    document.push_back(new Character('B', "Arial", 12, "Black", 2, 1));   // Same intrinsic!
    document.push_back(new Character('B', "Arial", 12, "Black", 3, 1));   // Same intrinsic!

    for(Character* c : document) c->display();

    std::cout << std::endl;
    std::cout << "Total objects: " << document.size() << std::endl;
    std::cout << "Intrinsic data repeated: " << document.size() << " times!" << std::endl;
    std::cout << "Unique intrinsic data: only 2 ('A' and 'B')!" << std::endl;

    // Imagine 100,000 characters with only 26 unique intrinsic states!
    // 100,000 × full data vs 26 × intrinsic + 100,000 × position only

    for(Character* c : document) delete c;

}

/*
===============================================================================
                  Problems Without the Flyweight Pattern
===============================================================================

Imagine thousands of character objects in a text editor.

Every character stores:

    symbol = 'A'
    font = "Arial"
    fontSize = 12
    color = "Black"
    x = ?
    y = ?

Notice that:

    Intrinsic (Shared) State
        • symbol
        • font
        • fontSize
        • color

    Extrinsic (Unique) State
        • x
        • y

Only the position (x, y) changes for most characters, while the remaining
data is identical across many objects.


Memory Waste Example

10,000 Character Objects

    Each object stores:
        symbol + font + fontSize + color + x + y

Result:
    The same intrinsic data is duplicated 10,000 times.

Reality:
    There may be only 26 unique intrinsic states
    ('A' through 'Z') that could be shared!


Problems:

❌ Massive Memory Waste
   Every object stores identical intrinsic data, consuming far more
   memory than necessary.

❌ Redundant Data
   Shared information such as font, colour, and size is repeatedly
   duplicated across thousands of objects.

❌ Expensive Object Creation
   Every object must be fully constructed, even though most of its
   state is identical to existing objects.

❌ Performance Degradation
   Creating and managing a very large number of heavyweight objects
   increases memory usage and reduces performance.

❌ Unnecessary Object Proliferation
   Thousands of nearly identical objects exist when only a small set
   of shared objects is actually required.

===============================================================================
*/