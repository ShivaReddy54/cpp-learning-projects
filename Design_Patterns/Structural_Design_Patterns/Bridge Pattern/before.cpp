/*

Intent: Decouple abstraction from implementation so that both can vary independently without affecting each other.

EX: A remote control (abstraction) can work with any device (implementation). You can have a remote for a TV, another for a radio — but the remote's buttons (power, volume) stay the same. 
    If you upgrade from a basic remote to an advanced remote, it still works with the same devices. 
    Both can change independently.

*/

#include <iostream>
#include <string>

// ===== Monolithic classes: abstraction TIED to implementation =====

// TV Remote — only works with TV
class BasicTVRemote{
private:
    std::string tvBrand;
    bool tvPower;
    int tvVolume;

public:
    BasicTVRemote(const std::string& brand) 
        : tvBrand(brand), tvPower(false), tvVolume(50) {}

    void powerOn(){
        tvPower = true;
        std::cout << tvBrand << " TV powered ON" << std::endl;
    }
    void powerOff(){
        tvPower = false;
        std::cout << tvBrand << " TV powered OFF" << std::endl;
    }
    void volumeUp(){
        tvVolume++;
        std::cout << tvBrand << " TV volume: " << tvVolume << std::endl;
    }
    void volumeDown(){
        tvVolume--;
        std::cout << tvBrand << " TV volume: " << tvVolume << std::endl;
    }
};

// Radio Remote — only works with Radio (DUPLICATED logic!)
class BasicRadioRemote {
private:
    std::string radioBrand;
    bool radioPower;
    int radioVolume;

public:
    BasicRadioRemote(const std::string& brand)
        : radioBrand(brand), radioPower(false), radioVolume(30) {}

    void powerOn(){
        radioPower = true;
        std::cout << radioBrand << " Radio powered ON" << std::endl;
    }
    void powerOff(){
        radioPower = false;
        std::cout << radioBrand << " Radio powered OFF" << std::endl;
    }
    void volumeUp(){
        radioVolume++;
        std::cout << radioBrand << " Radio volume: " << radioVolume << std::endl;
    }
    void volumeDown(){
        radioVolume--;
        std::cout << radioBrand << " Radio volume: " << radioVolume << std::endl;
    }
};

// Now if we want an ADVANCED remote, we need 2 MORE classes:
class AdvancedTVRemote {
    // Same as BasicTVRemote but with mute() — lots of duplication!
    // + TV-specific state duplicated again
};

class AdvancedRadioRemote {
    // Same as BasicRadioRemote but with mute() — lots of duplication!
    // + Radio-specific state duplicated again
};

int main() {
    BasicTVRemote tvRemote("Samsung");
    tvRemote.powerOn();
    tvRemote.volumeUp();
    tvRemote.volumeDown();
    tvRemote.powerOff();

    BasicRadioRemote radioRemote("Sony");
    radioRemote.powerOn();
    radioRemote.volumeUp();

    // PROBLEM: Want advanced remote? Must create ENTIRELY new classes
    // PROBLEM: Want DVD remote? Must create MORE classes
    // PROBLEM: 2 remote types × 3 device types = 6 classes!
    
}

/*
===============================================================================
                    Problems Without the Bridge Pattern
===============================================================================

Without Bridge, every combination of Remote and Device requires a separate
class.

                     Basic Remote        Advanced Remote
                     ------------        ----------------
TV                  BasicTVRemote        AdvancedTVRemote
Radio               BasicRadioRemote     AdvancedRadioRemote
DVD                 BasicDVDRemote       AdvancedDVDRemote

Total:
    3 Devices × 2 Remote Types = 6 Classes


What happens as the system grows?

Add 1 more Device
    └──► Need +2 more Remote classes

Add 1 more Remote Type
    └──► Need +3 more Device-specific classes

As both dimensions grow, the number of classes grows as:

            Total Classes = Devices × Remote Types

This is known as the **Class Explosion** problem.


Problems:

❌ Class Explosion
   Every new device must be combined with every remote type,
   leading to an explosion in the number of classes.

❌ Tight Coupling
   The abstraction (Remote) and the implementation (Device)
   are tightly coupled together.

❌ Code Duplication
   Similar logic is repeated across many nearly identical classes.

❌ Difficult to Extend
   Adding either a new device or a new remote requires creating
   many additional classes.

❌ Poor Maintainability
   Changes in one hierarchy often require modifications in the
   other, increasing maintenance effort.

===============================================================================
*/