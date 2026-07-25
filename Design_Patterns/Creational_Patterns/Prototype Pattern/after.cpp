#include <iostream>
#include <string>

// ===== STEP 1: Define Prototype interface with clone() =====
class CharacterPrototype{
public:
    virtual ~CharacterPrototype() = default;
    virtual CharacterPrototype* clone() = 0;
    virtual void display() = 0;
};

// ===== STEP 2: Concrete Prototype implements clone() =====
class Character : public CharacterPrototype{
public:
    std::string name;
    std::string hairColor;
    std::string eyeColor;
    std::string skinTone;
    std::string armorType;
    std::string weaponType;
    std::string skillTree;
    std::string animations;
    std::string physicsModel;

    // Constructor — expensive (called only ONCE per prototype)
    Character(const std::string& n, const std::string& hair,
              const std::string& eye, const std::string& skin,
              const std::string& armor, const std::string& weapon)
        : name(n), hairColor(hair), eyeColor(eye), skinTone(skin),
          armorType(armor), weaponType(weapon){
        
        std::cout << "Loading skill tree from database..." << std::endl;
        skillTree = "Full skill tree loaded";
        
        std::cout << "Loading animations from file..." << std::endl;
        animations = "All animations loaded";
        
        std::cout << "Computing physics model..." << std::endl;
        physicsModel = "Physics model computed";
    }

    // ===== STEP 3: Copy constructor — (used by clone()) =====
    Character(const Character& other){
        name = other.name;
        hairColor = other.hairColor;
        eyeColor = other.eyeColor;
        skinTone = other.skinTone;
        armorType = other.armorType;
        weaponType = other.weaponType;
        skillTree = other.skillTree;      // Just copying strings!
        animations = other.animations;    // No database loading!
        physicsModel = other.physicsModel; // No computation!
        
        std::cout << "Cloning character" << std::endl;
    }

    // ===== STEP 4: clone() method =====
    CharacterPrototype* clone() override{
        return new Character(*this);  // Uses copy constructor
    }

    void display() override{
        std::cout << "Character: " << name << std::endl;
        std::cout << "  Hair: " << hairColor << ", Eyes: " << eyeColor
                  << ", Skin: " << skinTone << std::endl;
        std::cout << "  Armor: " << armorType << ", Weapon: " << weaponType << std::endl;
        std::cout << "  Skills: " << skillTree << std::endl;
        std::cout << "  Animations: " << animations << std::endl;
        std::cout << "  Physics: " << physicsModel << std::endl;
    }
};

// ===== STEP 5: Prototype Registry (optional but useful) =====
class CharacterRegistry{
private:
    // Store pre-built prototypes
    Character* warriorPrototype;
    Character* magePrototype;
    Character* archerPrototype;

public:
    CharacterRegistry(){
        // Create prototypes ONCE — expensive, but only once!
        std::cout << "=== Building Prototypes (One-Time Cost) ===" << std::endl;
        warriorPrototype = new Character("Warrior", "Brown", "Blue", "Fair", "Plate", "Sword");
        magePrototype = new Character("Mage", "White", "Green", "Pale", "Robe", "Staff");
        archerPrototype = new Character("Archer", "Red", "Brown", "Tan", "Leather", "Bow");
        std::cout << "=== Prototypes Ready ===" << std::endl << std::endl;
    }

    Character* getWarrior() { return static_cast<Character*>(warriorPrototype->clone()); }
    Character* getMage() { return static_cast<Character*>(magePrototype->clone()); }
    Character* getArcher() { return static_cast<Character*>(archerPrototype->clone()); }

    ~CharacterRegistry(){
        delete warriorPrototype;
        delete magePrototype;
        delete archerPrototype;
    }
};

// ===== CLIENT CODE =====
int main() {
    // Registry creates prototypes ONCE
    CharacterRegistry registry;

    // ===== Clone warriors — CHEAP! =====
    std::cout << "--- Creating Warrior 1 (via Clone — CHEAP!) ---" << std::endl;
    Character* warrior1 = registry.getWarrior();
    warrior1->name = "Knight Arthur";
    warrior1->display();

    std::cout << std::endl;

    std::cout << "--- Creating Warrior 2 (via Clone — CHEAP!) ---" << std::endl;
    Character* warrior2 = registry.getWarrior();
    warrior2->name = "Knight Lancelot";
    warrior2->weaponType = "Axe";  // Small variation!
    warrior2->display();

    std::cout << std::endl;

    // ===== Clone mage — CHEAP! =====
    std::cout << "--- Creating Mage (via Clone — CHEAP!) ---" << std::endl;
    Character* mage = registry.getMage();
    mage->name = "Wizard Gandalf";
    mage->display();

    std::cout << std::endl;

    // ===== Clone archer — CHEAP! =====
    std::cout << "--- Creating Archer (via Clone — CHEAP!) ---" << std::endl;
    Character* archer = registry.getArcher();
    archer->name = "Ranger Legolas";
    archer->hairColor = "Blonde";  // Small variation!
    archer->display();

    // Cleanup
    delete warrior1;
    delete warrior2;
    delete mage;
    delete archer;

}

/*
===============================================================================
                    Prototype Pattern: Before vs After
===============================================================================

BEFORE (Without Prototype)

Every time a new character is created:

    Load Skill Tree      (EXPENSIVE)
             │
    Load Animations      (EXPENSIVE)
             │
    Compute Physics      (EXPENSIVE)
             │
             ▼
       New Character

Result:
    Total Cost = EXPENSIVE × Number of Characters


Problems:
  ❌ Expensive initialization is repeated for every object.
  ❌ Client must know all construction parameters.
  ❌ Creating small variations requires rebuilding everything.


===============================================================================

AFTER (Using Prototype Pattern)

                 Prototype Registry
                 ──────────────────

        Warrior Prototype   (Built Once)
        Mage Prototype      (Built Once)
        Archer Prototype    (Built Once)
                 │
                 ▼
            clone()  ─────────► Cheap Copy
                 │
            clone()  ─────────► Cheap Copy
                 │
            clone()  ─────────► Cheap Copy

Each clone can then be customized:

    Character* warrior = registry.getWarrior();
    warrior->name = "Knight Arthur";
    warrior->weaponType = "Axe";


Result:
  ✅ Expensive initialization happens only once.
  ✅ New objects are created by cloning existing prototypes.
  ✅ Modify only the fields that are different.
  ✅ Creating similar objects becomes fast and efficient.

===============================================================================
*/