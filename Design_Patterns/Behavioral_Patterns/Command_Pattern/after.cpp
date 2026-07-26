#include <iostream>
#include <string>
#include <vector>
#include <stack>

// ===== Receiver classes (the devices) =====
class Light{
private:
    std::string location;
    bool isOn;

public:
    Light(const std::string& loc) : location(loc), isOn(false) {}
    void turnOn() { isOn = true; std::cout << location << " Light: ON" << std::endl; }
    void turnOff() { isOn = false; std::cout << location << " Light: OFF" << std::endl; }
    bool isLightOn() { return isOn; }
};

class Fan{
private:
    std::string location;
    int speed;

public:
    Fan(const std::string& loc) : location(loc), speed(0) {}
    void setSpeed(int s) { speed = s; std::cout << location << " Fan: Speed " << speed << std::endl; }
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

// ===== STEP 1: Command interface =====
class Command{
public:
    virtual ~Command() = default;
    virtual void execute() = 0;
    virtual void undo() = 0;  // Every command supports undo!
};

// ===== STEP 2: Concrete Commands — each encapsulates ONE action =====
class LightOnCommand : public Command{
private:
    Light* light;

public:
    LightOnCommand(Light* l) : light(l) {}
    void execute() override { light->turnOn(); }
    void undo() override { light->turnOff(); }  // Undo Light ON = Light OFF!
};

class LightOffCommand : public Command{
private:
    Light* light;

public:
    LightOffCommand(Light* l) : light(l) {}
    void execute() override { light->turnOff(); }
    void undo() override { light->turnOn(); }
};

class FanHighCommand : public Command{
private:
    Fan* fan;
    int previousSpeed;  // Save previous state for undo!

public:
    FanHighCommand(Fan* f) : fan(f), previousSpeed(0) {}

    void execute() override{
        previousSpeed = fan->getSpeed();  // Save state BEFORE executing!
        fan->setSpeed(3);
    }

    void undo() override{
        fan->setSpeed(previousSpeed);  // Restore previous state!
    }
};

class FanOffCommand : public Command{
private:
    Fan* fan;
    int previousSpeed;

public:
    FanOffCommand(Fan* f) : fan(f), previousSpeed(0) {}

    void execute() override{
        previousSpeed = fan->getSpeed();
        fan->setSpeed(0);
    }

    void undo() override{
        fan->setSpeed(previousSpeed);
    }
};

class TVOnCommand : public Command{
private:
    TV* tv;

public:
    TVOnCommand(TV* t) : tv(t) {}
    void execute() override { tv->turnOn(); }
    void undo() override { tv->turnOff(); }
};

// ===== STEP 3: Macro Command — executes multiple commands together =====
class MacroCommand : public Command{
private:
    std::vector<Command*> commands;

public:
    MacroCommand(const std::vector<Command*>& cmds) : commands(cmds) {}

    void execute() override{
        for(Command* cmd : commands) cmd->execute();  // Execute all!
    }

    void undo() override{
        // Undo in REVERSE order!
        for(int i = commands.size() - 1; i >= 0; i--) commands[i]->undo();
    }
};

// ===== STEP 4: Invoker (Remote Control) — doesn't know about devices! =====
class RemoteControl {
private:
    std::vector<Command*> onCommands;    // "On" buttons
    std::vector<Command*> offCommands;   // "Off" buttons
    std::stack<Command*> history;        // Command history for undo!

public:
    RemoteControl(){
        // Initialize with null commands (7 button slots)
        for(int i = 0; i < 7; i++){
            onCommands.push_back(nullptr);
            offCommands.push_back(nullptr);
        }
    }

    // Configure a button slot — can be changed at RUNTIME!
    void setCommand(int slot, Command* onCmd, Command* offCmd){
        onCommands[slot] = onCmd;
        offCommands[slot] = offCmd;
    }

    void pressOnButton(int slot){
        if(onCommands[slot]){
            onCommands[slot]->execute();
            history.push(onCommands[slot]);  // Save for undo!
        }
    }

    void pressOffButton(int slot){
        if(offCommands[slot]){
            offCommands[slot]->execute();
            history.push(offCommands[slot]);
        }
    }

    // UNDO the last command!
    void pressUndoButton(){
        if(!history.empty()){
            std::cout << "--- Undoing last command ---" << std::endl;
            Command* lastCommand = history.top();
            lastCommand->undo();
            history.pop();
        } else std::cout << "No commands to undo" << std::endl;
    }
};

// ===== CLIENT CODE =====
int main() {
    // Create receivers (devices)
    Light livingRoomLight("Living Room");
    Fan bedroomFan("Bedroom");
    TV kitchenTV("Kitchen");

    // Create commands — encapsulate device + action
    Command* lightOn = new LightOnCommand(&livingRoomLight);
    Command* lightOff = new LightOffCommand(&livingRoomLight);
    Command* fanHigh = new FanHighCommand(&bedroomFan);
    Command* fanOff = new FanOffCommand(&bedroomFan);
    Command* tvOn = new TVOnCommand(&kitchenTV);

    // Create macro: "Night mode" — turn off everything
    std::vector<Command*> nightModeCmds = {lightOff, fanOff};
    Command* nightMode = new MacroCommand(nightModeCmds);

    // ===== Configure the remote — buttons can be reassigned! =====
    RemoteControl remote;
    remote.setCommand(0, lightOn, lightOff);   // Slot 0: Light
    remote.setCommand(1, fanHigh, fanOff);     // Slot 1: Fan
    remote.setCommand(2, tvOn, nullptr);       // Slot 2: TV (no off button)
    remote.setCommand(3, nightMode, nullptr);  // Slot 3: Night mode (macro!)

    // ===== Execute commands =====
    std::cout << "=== Pressing buttons ===" << std::endl;
    remote.pressOnButton(0);   // Light ON
    remote.pressOnButton(1);   // Fan HIGH

    std::cout << std::endl;

    // ===== UNDO! =====
    std::cout << "=== Undo last command (Fan HIGH) ===" << std::endl;
    remote.pressUndoButton();  // Fan returns to previous speed (0)

    std::cout << std::endl;

    // ===== Undo again! =====
    std::cout << "=== Undo again (Light ON) ===" << std::endl;
    remote.pressUndoButton();  // Light OFF

    std::cout << std::endl;

    // ===== Execute again and undo =====
    std::cout << "=== Light ON, then undo ===" << std::endl;
    remote.pressOnButton(0);
    remote.pressUndoButton();

    std::cout << std::endl;

    // ===== Macro command (Night mode) =====
    std::cout << "=== Night mode (macro: Light OFF + Fan OFF) ===" << std::endl;
    remote.pressOnButton(0);   // Light ON first
    remote.pressOnButton(1);   // Fan HIGH first
    remote.pressOnButton(3);   // Night mode: both OFF!

    std::cout << std::endl;

    // Undo macro — reverses ALL commands in macro!
    std::cout << "=== Undo Night mode ===" << std::endl;
    remote.pressUndoButton();

    // Cleanup
    delete lightOn; delete lightOff;
    delete fanHigh; delete fanOff;
    delete tvOn; delete nightMode;

}

/*
===============================================================================
                     Command Pattern: Before vs After
===============================================================================

BEFORE (Without Command)

Remote.pressButton1()
    └──► light.turnOn()

Remote.pressButton2()
    └──► light.turnOff()

Every button is permanently hardcoded to a specific device action.

Result:
    Remote Control
        │
        ├── Directly calls Light
        ├── Directly calls Fan
        ├── Directly calls TV
        └── Must know every device and operation


Problems:
  ❌ No undo functionality.
  ❌ Buttons cannot be reconfigured.
  ❌ Commands cannot be queued.
  ❌ No command history or logging.
  ❌ Macro operations are impossible.
  ❌ Tight coupling between the remote and concrete devices.


===============================================================================

AFTER (Using Command Pattern)

Remote.pressOnButton(0)
    │
    ▼
onCommands[0]->execute();

Example configuration:

    onCommands[0] = LightOnCommand;
    onCommands[1] = FanHighCommand;
    onCommands[2] = TVOnCommand;

Commands are assigned at runtime and can be replaced whenever needed.


Additional Capabilities

Undo
    ✅ pressUndoButton()

Runtime Reconfiguration
    ✅ setCommand(slot, command)

Command Queue
    ✅ Store commands in a queue/vector and execute later.

Command History
    ✅ Record executed commands for logging or replay.

Macro Commands
    ✅ Execute multiple commands through a single MacroCommand.

Loose Coupling
    ✅ The Remote knows only the Command interface,
       never the concrete device classes.


Result:
  ✅ Undo support.
  ✅ Runtime button reconfiguration.
  ✅ Command queuing.
  ✅ Command history and logging.
  ✅ Macro commands.
  ✅ Loose coupling between invoker and receivers.

===============================================================================
*/