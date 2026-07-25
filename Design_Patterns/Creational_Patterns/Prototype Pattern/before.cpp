/*

Intent: Create new objects by cloning an existing object (the prototype) instead of creating from scratch.

Ex: You have a resume template. Instead of typing a new resume from scratch every time, you copy the template and make changes. 
    The original is the prototype; the copies are clones.

*/

#include <iostream>
#include <string>

// ===== Complex objects that are expensive to create =====
class Character{
public:
    std::string name;
    std::string hairColor;
    std::string eyeColor;
    std::string skinTone;
    std::string armorType;
    std::string weaponType;
    std::string skillTree;    // Expensive computation
    std::string animations;   // Expensive loading from file
    std::string physicsModel; // Expensive simulation setup

    Character(const std::string& n, const std::string& hair, 
              const std::string& eye, const std::string& skin,
              const std::string& armor, const std::string& weapon)
        : name(n), hairColor(hair), eyeColor(eye), skinTone(skin),
          armorType(armor), weaponType(weapon){
        
        // Simulate expensive operations
        std::cout << "Loading skill tree from database..." << std::endl;
        skillTree = "Full skill tree loaded";
        
        std::cout << "Loading animations from file..." << std::endl;
        animations = "All animations loaded";
        
        std::cout << "Computing physics model..." << std::endl;
        physicsModel = "Physics model computed";
    }

    void display(){
        std::cout << "Character: " << name << std::endl;
        std::cout << "  Hair: " << hairColor << ", Eyes: " << eyeColor 
                  << ", Skin: " << skinTone << std::endl;
        std::cout << "  Armor: " << armorType << ", Weapon: " << weaponType << std::endl;
        std::cout << "  Skills: " << skillTree << std::endl;
        std::cout << "  Animations: " << animations << std::endl;
        std::cout << "  Physics: " << physicsModel << std::endl;
    }
};

int main() {
    // Creating first character — expensive!
    std::cout << "--- Creating Character 1 (EXPENSIVE) ---" << std::endl;
    Character* warrior1 = new Character("Warrior", "Brown", "Blue", "Fair", "Plate", "Sword");
    warrior1->display();

    std::cout << std::endl;

    // Creating second character — ALSO expensive! (Same type, just different name)
    std::cout << "--- Creating Character 2 (EXPENSIVE AGAIN!) ---" << std::endl;
    Character* warrior2 = new Character("Warrior Clone", "Brown", "Blue", "Fair", "Plate", "Sword");
    warrior2->display();

    // PROBLEM: We repeated ALL the expensive operations for nearly identical objects!
    // The skill tree, animations, and physics model are the SAME — wasted resources!

    delete warrior1;
    delete warrior2;

}

/*
===============================================================================
                  Problems Without the Prototype Pattern
===============================================================================

Character 1 Creation

    Load Skill Tree
            │
    Load Animations
            │
    Compute Physics
            │
            ▼
        "Warrior"


Character 2 Creation

    Load Skill Tree
            │
    Load Animations        ← DUPLICATE WORK
            │
    Compute Physics
            │
            ▼
     "Warrior Clone"
      (Only the name changed)


Problems:

❌ Expensive Operations Repeated
   Every new character performs the same costly initialization process,
   even when it is almost identical to an existing one.

❌ Wasted Time and Resources
   Loading assets, computing physics, and initializing data repeatedly
   wastes CPU time and memory.

❌ Client Knows Too Much
   The client must understand every step required to construct
   a fully initialized character.

❌ Difficult to Create Variations
   Creating a slightly different character requires rebuilding
   the entire object from scratch instead of copying and modifying it.

===============================================================================
*/