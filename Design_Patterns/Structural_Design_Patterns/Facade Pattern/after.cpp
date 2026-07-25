#include <iostream>
#include <string>

// ===== Subsystem classes (unchanged — they stay complex internally) =====
class CPU{
public:
    void freeze() { std::cout << "CPU: Freezing processor" << std::endl; }
    void jump(long position) { std::cout << "CPU: Jumping to position " << position << std::endl; }
    void execute() { std::cout << "CPU: Executing instructions" << std::endl; }
};

class Memory{
public:
    void load(long position, const std::string& data){
        std::cout << "Memory: Loading '" << data << "' at position " << position << std::endl;
    }
};

class HardDrive{
public:
    std::string read(long sector, int size){
        std::cout << "HardDrive: Reading " << size << " bytes from sector " << sector << std::endl;
        return "BOOT_DATA";
    }
};

class PowerSupply{
public:
    void turnOn() { std::cout << "PowerSupply: Turning on" << std::endl; }
    void turnOff() { std::cout << "PowerSupply: Turning off" << std::endl; }
    bool isVoltageStable(){
        std::cout << "PowerSupply: Checking voltage stability" << std::endl;
        return true;
    }
};

class CoolingSystem{
public:
    void startFans() { std::cout << "CoolingSystem: Starting fans" << std::endl; }
    void stopFans() { std::cout << "CoolingSystem: Stopping fans" << std::endl; }
    int getTemperature() { std::cout << "CoolingSystem: Checking temperature" << std::endl; return 35; }
};

// ===== STEP 1: FACADE — simplifies the complex subsystem =====
class ComputerFacade{
private:
    CPU cpu;
    Memory memory;
    HardDrive hardDrive;
    PowerSupply powerSupply;
    CoolingSystem cooling;

public:
    // One simple method — hides ALL the complexity!
    void startComputer(){
        std::cout << "=== Starting Computer ===" << std::endl;

        // All orchestration happens INSIDE the facade
        powerSupply.turnOn();

        if(!powerSupply.isVoltageStable()){
            std::cout << "Boot failed: unstable voltage" << std::endl;
            return;
        }

        cooling.startFans();

        if(cooling.getTemperature() > 80){
            std::cout << "Boot failed: overheating" << std::endl;
            return;
        }

        cpu.freeze();
        std::string bootData = hardDrive.read(0, 1024);
        memory.load(0, bootData);
        cpu.jump(0);
        cpu.execute();

        std::cout << "Computer started successfully!" << std::endl;
    }

    void shutdownComputer(){
        std::cout << "=== Shutting Down Computer ===" << std::endl;
        cpu.freeze();
        cooling.stopFans();
        powerSupply.turnOff();
        std::cout << "Computer shut down successfully!" << std::endl;
    }
};

// ===== CLIENT CODE — now SIMPLE! =====
int main() {
    ComputerFacade computer;

    // Just ONE call to start! No need to know ANY subsystem details!
    computer.startComputer();

    std::cout << std::endl;

    // Just ONE call to shutdown!
    computer.shutdownComputer();

    // Client doesn't know about CPU, Memory, HardDrive, PowerSupply, CoolingSystem
    // Client doesn't know the boot sequence
    // If subsystem changes, only Facade needs to update — client unchanged!

}

/*
===============================================================================
                      Facade Pattern: Before vs After
===============================================================================

BEFORE (Without Facade)

Client
   │
   ├──► CPU
   ├──► Memory
   ├──► HardDrive
   ├──► PowerSupply
   ├──► CoolingSystem
   └──► ... many more subsystem classes

The client must call 10+ methods in the correct order:

    powerOn()
    initializeCPU()
    initializeMemory()
    spinUpHardDrive()
    startCoolingSystem()
    ...
    bootOperatingSystem()


Problems:
  ❌ Client depends on many subsystem classes.
  ❌ Client must know the exact startup sequence.
  ❌ Client understands internal implementation details.
  ❌ Startup logic is fragile and difficult to maintain.


===============================================================================

AFTER (Using Facade)

Client
   │
   ▼
ComputerFacade
   │
   ├──► CPU
   ├──► Memory
   ├──► HardDrive
   ├──► PowerSupply
   ├──► CoolingSystem
   └──► ... all other subsystems

The client simply calls:

    startComputer();

The Facade internally performs every subsystem operation in the
correct order.


Result:
  ✅ One client-facing class instead of many.
  ✅ A single method call instead of 10+ calls.
  ✅ Correct execution order handled by the Facade.
  ✅ Subsystem implementation details remain hidden.
  ✅ Simpler, safer, and more maintainable client code.

===============================================================================
*/