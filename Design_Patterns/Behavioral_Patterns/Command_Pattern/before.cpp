/*

Intent: Encapsulate a request as an object, thereby letting you parameterize clients with different requests,
        queue or log requests, and support undoable operations.

EX: A remote control. Each button (Power, Volume+, Volume-) is a Command object.
    The remote doesn't know what the button does — it just presses the command.
    You can swap buttons, queue commands, or undo them (pressing Volume- undoes Volume+).

*/

#include <iostream>
#include <string>

// ===== Smart home devices =====
class Light{
private:
    bool isOn;
    std::string location;

public:
    Light(const std::string& loc) : location(loc), isOn(false) {}

    void turnOn() { isOn = true; std::cout << location << " Light: ON" << std::endl; }
    void turnOff() { isOn = false; std::cout << location << " Light: OFF" << std::endl; }
    bool isLightOn() { return isOn; }
};

class Fan{
private:
    int speed;
    std::string location;

public:
    Fan(const std::string& loc) : location(loc), speed(0) {}

    void setSpeed(int s){
        speed = s;
        std::cout << location << " Fan: Speed " << speed << std::endl; 
    }
    int getSpeed() { return speed; }
};

class TV{
private:
    std::string location;
    bool isOn;

public:
    TV(const std::string& loc) : location(loc), isOn(false) {}

    void turnOn() { isOn = true; std::cout << location << " TV: ON" << std::endl; }
    void turnOff() { isOn = false; std::cout << location << " TV: OFF" << std::endl; }
};

// ===== PROBLEM: Remote control directly calls device methods =====
class SimpleRemoteControl {
private:
    Light* light;
    Fan* fan;
    TV* tv;

public:
    SimpleRemoteControl(Light* l, Fan* f, TV* t) : light(l), fan(f), tv(t) {}

    // Each button HARDCODED to a specific device and action
    void pressButton1() { light->turnOn(); }       // Button1 = Light ON
    void pressButton2() { light->turnOff(); }      // Button2 = Light OFF
    void pressButton3() { fan->setSpeed(1); }      // Button3 = Fan speed 1
    void pressButton4() { fan->setSpeed(2); }      // Button4 = Fan speed 2
    void pressButton5() { tv->turnOn(); }          // Button5 = TV ON

    // PROBLEMS:
    // - No undo! Pressed button1? Can't undo "Light ON"
    // - Can't reconfigure buttons — button1 is always Light ON
    // - Can't queue commands (e.g., "turn off everything at bedtime")
    // - Can't log commands (e.g., "show history of what was pressed")
    // - Tightly coupled to specific devices
    // - Adding a new device requires modifying the remote control class!
};

int main() {
    Light livingRoomLight("Living Room");
    Fan bedroomFan("Bedroom");
    TV kitchenTV("Kitchen");

    SimpleRemoteControl remote(&livingRoomLight, &bedroomFan, &kitchenTV);

    // Direct action — no undo possible!
    remote.pressButton1();  // Light ON
    remote.pressButton3();  // Fan speed 1

    // PROBLEM: I turned the light on — how do I undo that?
    // I have to remember what button1 did and manually call pressButton2!
    // No automatic undo, no command history, no flexibility!

    // PROBLEM: Can't say "turn off everything" — must call each button separately
    // PROBLEM: Can't swap button assignments without modifying code

}

/*
===============================================================================
                  Problems Without the Command Pattern
===============================================================================

SimpleRemoteControl

    Button 1  ──► light.turnOn()
    Button 2  ──► light.turnOff()
    Button 3  ──► fan.setSpeed(1)
    ...

Each button is hardcoded to invoke a specific method on a specific device.

Result:
    Remote Control
        │
        ├── Directly knows Light
        ├── Directly knows Fan
        ├── Directly knows TV
        └── Directly knows every other device


Problems:

❌ No Undo Capability
   Once an action is executed, there is no way to reverse it.

❌ Hardcoded Button Actions
   Every button is permanently tied to a specific operation,
   making the remote difficult to customize.

❌ No Runtime Configuration
   Buttons cannot be reassigned to different actions without
   modifying the source code.

❌ Cannot Queue Commands
   Operations cannot be stored and executed later in sequence.

❌ No Command History
   Since actions are invoked directly, they cannot easily be
   logged, recorded, or replayed.

❌ Tight Coupling
   The remote control depends directly on concrete device classes,
   reducing flexibility and reusability.

❌ Difficult to Extend
   Supporting a new device usually requires modifying the
   remote control implementation.

❌ No Macro Commands
   Multiple operations (e.g., "Turn Off Everything") cannot be
   grouped into a single executable action.

===============================================================================
*/