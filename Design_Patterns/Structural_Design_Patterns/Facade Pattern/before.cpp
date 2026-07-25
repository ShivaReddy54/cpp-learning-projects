/*

Intent: Provide a unified, simplified interface to a complex subsystem.
        Facade defines a higher-level interface that makes the subsystem easier to use.

EX: When you call customer service, you dial one number.
    Behind that, there are departments: billing, technical support, returns, shipping.
    The customer service rep (Facade) figures out which department to route to.
    You don't need to know the internal complexity — you just make one call.

*/

#include <iostream>
#include <string>

// ===== Complex subsystem — many classes with many methods =====
class CPU{
public:
    void freeze() { std::cout << "CPU: Freezing processor" << std::endl; }
    void jump(long position) { std::cout << "CPU: Jumping to position " << position << std::endl; }
    void execute() { std::cout << "CPU: Executing instructions" << std::endl; }
};

class Memory{
public:
    void load(long position, const std::string& data) {
        std::cout << "Memory: Loading '" << data << "' at position " << position << std::endl;
    }
};

class HardDrive{
public:
    std::string read(long sector, int size) {
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

// ===== CLIENT must know ALL subsystem details =====
int main() {
    // PROBLEM: Client must orchestrate 5 different classes in the RIGHT order!
    // This is complex, error-prone, and tightly coupled to subsystem internals.

    CPU cpu;
    Memory memory;
    HardDrive hardDrive;
    PowerSupply powerSupply;
    CoolingSystem cooling;

    // Step 1: Turn on power
    powerSupply.turnOn();

    // Step 2: Check voltage
    if(!powerSupply.isVoltageStable()){
        std::cout << "Voltage unstable! Aborting boot." << std::endl;
        return 1;
    }

    // Step 3: Start cooling
    cooling.startFans();

    // Step 4: Check temperature
    if(cooling.getTemperature() > 80){
        std::cout << "Temperature too high! Aborting boot." << std::endl;
        return 1;
    }

    // Step 5: CPU freeze
    cpu.freeze();

    // Step 6: Load boot data from hard drive to memory
    std::string bootData = hardDrive.read(0, 1024);
    memory.load(0, bootData);

    // Step 7: CPU jump to boot position
    cpu.jump(0);

    // Step 8: CPU execute
    cpu.execute();

    std::cout << "Computer booted successfully!" << std::endl;

    // PROBLEM: If boot sequence changes, EVERY client must be updated
    // PROBLEM: If new subsystem class is added, client must know about it
    // PROBLEM: This is too complex for most clients to handle
    // PROBLEM: 5 classes, 10+ method calls — just to boot a computer!

}

/*
===============================================================================
                    Problems Without the Facade Pattern
===============================================================================

The client is responsible for interacting with the entire subsystem.

The client must know:

  • All subsystem classes.
  • Which methods to call.
  • The correct order of operations.
  • How to handle failures at every step.
  • Internal implementation details of the subsystem.

Example:

Client
   │
   ├──► PowerManager.start()
   ├──► MemoryManager.initialize()
   ├──► CPU.initialize()
   ├──► DeviceManager.loadDrivers()
   ├──► FileSystem.mount()
   ├──► NetworkManager.connect()
   ├──► SecurityManager.authenticate()
   └──► Application.start()

One mistake in the sequence may cause the entire operation to fail.


Problems:

❌ Too Complex for the Client
   The client must understand the complete boot process instead of
   simply requesting "start the system."

❌ Tight Coupling
   The client becomes tightly coupled to the subsystem's internal
   classes and implementation details.

❌ Difficult to Maintain
   If the boot sequence changes, every client using the subsystem
   must also be updated.

❌ Poor Scalability
   As the subsystem grows, clients must learn additional classes,
   methods, and dependencies.

❌ Code Duplication
   Every client repeats the same sequence of subsystem calls,
   resulting in duplicated logic.

❌ Error-Prone
   Calling methods in the wrong order can leave the system in an
   invalid or partially initialized state.

===============================================================================
*/