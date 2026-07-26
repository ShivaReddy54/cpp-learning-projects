#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

// ===== STEP 1: Observer interface =====
class Observer{
public:
    virtual ~Observer() = default;
    virtual void update(double temp, double humidity, double pressure) = 0;
};

// ===== STEP 2: Subject interface =====
class Subject{
public:
    virtual ~Subject() = default;
    virtual void addObserver(Observer* observer) = 0;
    virtual void removeObserver(Observer* observer) = 0;
    virtual void notifyObservers() = 0;
};

// ===== STEP 3: Concrete Subject =====
class WeatherStation : public Subject{
private:
    std::vector<Observer*> observers;
    double temperature;
    double humidity;
    double pressure;

public:
    void addObserver(Observer* observer) override{
        observers.push_back(observer);
        std::cout << "Observer added (total: " << observers.size() << ")" << std::endl;
    }

    void removeObserver(Observer* observer) override{
        observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
        std::cout << "Observer removed (total: " << observers.size() << ")" << std::endl;
    }

    void notifyObservers() override{
        std::cout << "Notifying " << observers.size() << " observers..." << std::endl;
        for(Observer* observer : observers) observer->update(temperature, humidity, pressure);
    }

    void setMeasurements(double temp, double hum, double pres) {
        temperature = temp;
        humidity = hum;
        pressure = pres;
        std::cout << "WeatherStation: New measurements - Temp: " << temp 
                  << "°C, Humidity: " << hum << "%, Pressure: " << pres << " hPa" << std::endl;
        notifyObservers();  // Automatically notify all observers!
    }
};

// ===== STEP 4: Concrete Observers =====
class PhoneDisplay : public Observer{
public:
    void update(double temp, double humidity, double pressure) override{
        std::cout << "PhoneDisplay: Temperature = " << temp << "°C, Humidity = " << humidity << "%" << std::endl;
    }
};

class WindowDisplay : public Observer{
public:
    void update(double temp, double humidity, double pressure) override{
        std::cout << "WindowDisplay: Temp = " << temp << "°C, Hum = " << humidity 
                  << "%, Pressure = " << pressure << " hPa" << std::endl;
    }
};

class StatisticsDisplay : public Observer{
private:
    std::vector<double> temperatures;

public:
    void update(double temp, double humidity, double pressure) override{
        temperatures.push_back(temp);
        double avg = 0;
        for(double t : temperatures) avg += t;
        avg /= temperatures.size();
        std::cout << "StatisticsDisplay: Avg Temp = " << avg << "°C (from "
                  << temperatures.size() << " readings), Pressure = " << pressure << " hPa" << std::endl;
    }
};

class AlarmDisplay : public Observer{
private:
    double threshold;

public:
    AlarmDisplay(double t) : threshold(t) {}

    void update(double temp, double humidity, double pressure) override {
        if(temp > threshold){
            std::cout << "AlarmDisplay: 🔥 HIGH TEMPERATURE ALERT! " << temp 
                      << "°C exceeds threshold " << threshold << "°C!" << std::endl;
        } else std::cout << "AlarmDisplay: Temperature normal (" << temp << "°C)" << std::endl;
    }
};

// ===== CLIENT CODE =====
int main() {
    WeatherStation station;

    // ===== Create observers =====
    PhoneDisplay phone;
    WindowDisplay window;
    StatisticsDisplay stats;
    AlarmDisplay alarm(30.0);

    // ===== Register observers DYNAMICALLY! =====
    std::cout << "=== Registering observers ===" << std::endl;
    station.addObserver(&phone);
    station.addObserver(&window);
    station.addObserver(&stats);

    std::cout << std::endl;

    // ===== Measurements update — all observers notified automatically! =====
    std::cout << "=== First measurement ===" << std::endl;
    station.setMeasurements(25.5, 65.0, 1013.2);

    std::cout << std::endl;

    std::cout << "=== Second measurement ===" << std::endl;
    station.setMeasurements(27.0, 70.0, 1015.0);

    std::cout << std::endl;

    // ===== Add alarm observer DYNAMICALLY — no modification to station! =====
    std::cout << "=== Adding Alarm observer ===" << std::endl;
    station.addObserver(&alarm);

    std::cout << std::endl;

    std::cout << "=== Third measurement (below threshold) ===" << std::endl;
    station.setMeasurements(28.0, 72.0, 1014.0);

    std::cout << std::endl;

    // ===== High temperature — alarm triggers! =====
    std::cout << "=== Fourth measurement (HIGH temperature!) ===" << std::endl;
    station.setMeasurements(35.0, 80.0, 1010.0);

    std::cout << std::endl;

    // ===== Remove phone display DYNAMICALLY =====
    std::cout << "=== Removing Phone observer ===" << std::endl;
    station.removeObserver(&phone);

    std::cout << std::endl;

    // ===== Fifth measurement — phone NOT notified! =====
    std::cout << "=== Fifth measurement (Phone removed) ===" << std::endl;
    station.setMeasurements(30.0, 75.0, 1012.0);

    // Station never knows about specific display types!
    // Observers register/remove themselves dynamically!
    // Adding new observer type doesn't require modifying station!

}

/*
===============================================================================
                     Observer Pattern: Before vs After
===============================================================================

BEFORE (Without Observer)

                    WeatherStation
                     /     |      \
                    /      |       \
                   ▼       ▼        ▼
            PhoneDisplay  WindowDisplay  StatisticsDisplay

The WeatherStation keeps direct references to every display.

Problems:
    • Adding a new display requires modifying WeatherStation.
    • Removing a display also requires modifying WeatherStation.
    • No dynamic registration or removal of displays.
    • Strong coupling between the station and every display.


===============================================================================

AFTER (Using Observer)

                           Subject
                              │
            ┌─────────────────┼─────────────────┐
            │                 │                 │
            ▼                 ▼                 ▼
     addObserver()    removeObserver()   notifyObservers()
                              │
                              ▼
                     Observer Interface
                              │
          ┌───────────────────┼───────────────────┐
          │                   │                   │
          ▼                   ▼                   ▼
     PhoneDisplay      WindowDisplay     StatisticsDisplay

Adding a new display:
    → Simply implement the Observer interface.

Removing a display:
    → Call removeObserver().

Observers can register and unregister dynamically at runtime.


Result:

  ❌ Tight coupling                     ✅ Loose coupling
  ❌ No dynamic registration            ✅ Dynamic registration
  ❌ Violates Open/Closed Principle     ✅ Follows Open/Closed Principle
  ❌ No common notification interface   ✅ Common Observer interface

===============================================================================
*/