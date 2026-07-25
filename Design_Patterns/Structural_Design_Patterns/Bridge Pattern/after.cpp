#include <iostream>
#include <string>

// ===== STEP 1: Implementation interface (Device) =====
class Device{
public:
    virtual ~Device() = default;
    virtual bool isEnabled() = 0;
    virtual void enable() = 0;
    virtual void disable() = 0;
    virtual int getVolume() = 0;
    virtual void setVolume(int vol) = 0;
    virtual std::string getName() = 0;
};

// ===== STEP 2: Concrete Implementations =====
class TV : public Device{
private:
    std::string brand;
    bool power;
    int volume;

public:
    TV(const std::string& b) : brand(b), power(false), volume(50) {}

    bool isEnabled() override { return power; }
    void enable() override { power = true; std::cout << brand << " TV powered ON" << std::endl; }
    void disable() override { power = false; std::cout << brand << " TV powered OFF" << std::endl; }
    int getVolume() override { return volume; }
    void setVolume(int vol) override { volume = vol; std::cout << brand << " TV volume: " << volume << std::endl; }
    std::string getName() override { return brand + " TV"; }
};

class Radio : public Device{
private:
    std::string brand;
    bool power;
    int volume;

public:
    Radio(const std::string& b) : brand(b), power(false), volume(30) {}

    bool isEnabled() override { return power; }
    void enable() override { power = true; std::cout << brand << " Radio powered ON" << std::endl; }
    void disable() override { power = false; std::cout << brand << " Radio powered OFF" << std::endl; }
    int getVolume() override { return volume; }
    void setVolume(int vol) override { volume = vol; std::cout << brand << " Radio volume: " << volume << std::endl; }
    std::string getName() override { return brand + " Radio"; }
};

// ===== STEP 3: Abstraction (Remote Control) =====
class RemoteControl {
protected:
    Device* device;  // ← BRIDGE: Abstraction holds reference to Implementation

public:
    RemoteControl(Device* dev) : device(dev) {}
    virtual ~RemoteControl() = default;

    void powerToggle(){
        if(device->isEnabled())device->disable();
        else device->enable();
    }

    void volumeUp(){
        device->setVolume(device->getVolume() + 1);
    }

    void volumeDown(){
        device->setVolume(device->getVolume() - 1);
    }

    virtual void showInfo(){
        std::cout << "Basic remote controlling: " << device->getName() << std::endl;
    }
};

// ===== STEP 4: Extended Abstraction (Advanced Remote) =====
class AdvancedRemoteControl : public RemoteControl{
public:
    AdvancedRemoteControl(Device* dev) : RemoteControl(dev) {}

    // Additional functionality — but still works with ANY device!
    void mute(){
        std::cout << "Muting " << device->getName() << std::endl;
        device->setVolume(0);
    }

    void showInfo() override{
        std::cout << "Advanced remote controlling: " << device->getName() 
                  << " (with mute feature)" << std::endl;
    }
};

// ===== CLIENT CODE =====
int main() {
    // ===== Basic Remote with TV =====
    TV* samsungTV = new TV("Samsung");
    RemoteControl* basicRemote = new RemoteControl(samsungTV);
    std::cout << "=== Basic Remote + Samsung TV ===" << std::endl;
    basicRemote->powerToggle();
    basicRemote->volumeUp();
    basicRemote->volumeUp();
    basicRemote->volumeDown();
    basicRemote->powerToggle();

    std::cout << std::endl;

    // ===== Advanced Remote with SAME TV =====
    AdvancedRemoteControl* advancedRemoteTV = new AdvancedRemoteControl(samsungTV);
    std::cout << "=== Advanced Remote + Samsung TV ===" << std::endl;
    advancedRemoteTV->powerToggle();
    advancedRemoteTV->volumeUp();
    advancedRemoteTV->mute();      // Advanced feature!
    advancedRemoteTV->showInfo();

    std::cout << std::endl;

    // ===== Basic Remote with Radio =====
    Radio* sonyRadio = new Radio("Sony");
    RemoteControl* basicRemoteRadio = new RemoteControl(sonyRadio);
    std::cout << "=== Basic Remote + Sony Radio ===" << std::endl;
    basicRemoteRadio->powerToggle();
    basicRemoteRadio->volumeUp();
    basicRemoteRadio->showInfo();

    std::cout << std::endl;

    // ===== Advanced Remote with SAME Radio =====
    AdvancedRemoteControl* advancedRemoteRadio = new AdvancedRemoteControl(sonyRadio);
    std::cout << "=== Advanced Remote + Sony Radio ===" << std::endl;
    advancedRemoteRadio->powerToggle();
    advancedRemoteRadio->mute();   // Advanced feature works on Radio too!
    advancedRemoteRadio->showInfo();

    // Cleanup
    delete samsungTV;
    delete sonyRadio;
    delete basicRemote;
    delete advancedRemoteTV;
    delete basicRemoteRadio;
    delete advancedRemoteRadio;

}

/*
===============================================================================
                     Bridge Pattern: Before vs After
===============================================================================

BEFORE (Without Bridge)

Every Remote type must be combined with every Device.

                     Basic Remote        Advanced Remote
                     ------------        ----------------
TV                  BasicTVRemote        AdvancedTVRemote
Radio               BasicRadioRemote     AdvancedRadioRemote
DVD                 BasicDVDRemote       AdvancedDVDRemote

Total:
    3 Devices × 2 Remote Types = 6 Classes


Problems:
  ❌ Too many classes (M × N growth).
  ❌ Remote and Device are tightly coupled.
  ❌ Large amount of duplicated code.
  ❌ Extending either hierarchy is difficult.


===============================================================================

AFTER (Using Bridge)

                 RemoteControl
                 (Abstraction)
                       │
                       │
             AdvancedRemoteControl
          (Extended Abstraction)
                       │
                Holds a Device*
                       │
        ┌──────────────┼──────────────┐
        │              │              │
       TV            Radio           DVD
 (Implementation) (Implementation) (Implementation)


Total:
    2 Remote Classes + 3 Device Classes = 5 Classes

Growth becomes independent:

Add a new Device
    └──► Only +1 Device class

Add a new Remote type
    └──► Only +1 Remote class


Result:
  ✅ Fewer classes (M + N instead of M × N).
  ✅ Remote and Device evolve independently.
  ✅ Eliminates duplicated combination classes.
  ✅ Easy to extend either hierarchy without affecting the other.

===============================================================================
*/