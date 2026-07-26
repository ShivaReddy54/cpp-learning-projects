/*

Intent: Define a one-to-many dependency between objects so that when one object changes state,
        all its dependents are notified and updated automatically.

EX: YouTube subscriptions. You subscribe to a channel (you become an observer).
    When the creator uploads a new video (subject changes state), all subscribers get notified.
    You don't check the channel every day — the channel pushes the notification to you.

*/

#include <iostream>
#include <string>
#include <vector>

// ===== Weather station (subject) =====
class WeatherStation{
private:
    double temperature;
    double humidity;
    double pressure;

public:
    void setMeasurements(double temp, double hum, double pres){
        temperature = temp;
        humidity = hum;
        pressure = pres;
        std::cout << "WeatherStation: New measurements - Temp: " << temp 
                  << "°C, Humidity: " << hum << "%, Pressure: " << pres << " hPa" << std::endl;

        // PROBLEM: Must MANUALLY notify each display!
        // If we add a new display, we must modify WeatherStation!
        // Tight coupling between station and displays!
    }

    double getTemperature() { return temperature; }
    double getHumidity() { return humidity; }
    double getPressure() { return pressure; }
};

// ===== Display devices (observers) =====
class PhoneDisplay{
public:
    void update(double temp, double humidity){
        std::cout << "PhoneDisplay: Temperature = " << temp << "°C, Humidity = " << humidity << "%" << std::endl;
    }
};

class WindowDisplay{
public:
    void update(double temp, double humidity, double pressure){
        std::cout << "WindowDisplay: Temp = " << temp << "°C, Hum = " << humidity 
                  << "%, Pressure = " << pressure << " hPa" << std::endl;
    }
};

class StatisticsDisplay{
public:
    void update(double temp, double pressure){
        std::cout << "StatisticsDisplay: Avg Temp = " << temp << "°C, Pressure = " << pressure << " hPa" << std::endl;
    }
};

// ===== PROBLEM: Station must KNOW about every display! =====
class WeatherStationWithDisplays : public WeatherStation {
private:
    PhoneDisplay* phone;      // Direct reference!
    WindowDisplay* window;    // Direct reference!
    StatisticsDisplay* stats; // Direct reference!

public:
    WeatherStationWithDisplays(PhoneDisplay* p, WindowDisplay* w, StatisticsDisplay* s)
        : phone(p), window(w), stats(s) {}

    void setMeasurements(double temp, double hum, double pres){
        WeatherStation::setMeasurements(temp, hum, pres);
        // Manually call each display — tight coupling!
        phone->update(temp, hum);
        window->update(temp, hum, pres);
        stats->update(temp, pres);
        // Adding a new display requires modifying this class!
    }
};

int main() {
    PhoneDisplay phone;
    WindowDisplay window;
    StatisticsDisplay stats;

    WeatherStationWithDisplays station(&phone, &window, &stats);

    station.setMeasurements(25.5, 65.0, 1013.2);
    station.setMeasurements(27.0, 70.0, 1015.0);

    // PROBLEM: Want to add a new AlarmDisplay? Must modify WeatherStationWithDisplays!
    // PROBLEM: Want to remove PhoneDisplay? Must modify WeatherStationWithDisplays!
    // PROBLEM: Each display needs different data — station must know each display's needs!
    // PROBLEM: Can't add/remove observers dynamically!

}

/*
===============================================================================
                   Problems Without the Observer Pattern
===============================================================================

WeatherStation directly communicates with every display.

                    WeatherStation
                          │
          ┌───────────────┼────────────────┐
          │               │                │
          ▼               ▼                ▼
    PhoneDisplay    WindowDisplay   StatisticsDisplay

Every time the weather changes, the WeatherStation must
explicitly notify each display.


Problems:

❌ Tight Coupling
   The WeatherStation knows about every concrete display type,
   creating strong dependencies.

❌ No Dynamic Observer Management
   Displays cannot be added or removed at runtime without changing
   the WeatherStation.

❌ Violates Open/Closed Principle
   Adding a new display requires modifying the WeatherStation class.

❌ Display-Specific Knowledge
   The WeatherStation must know how each display should be updated,
   making it responsible for too many details.

❌ No Common Notification Interface
   Different display types may require different update methods,
   preventing uniform notifications.

❌ Poor Scalability
   As more displays are added, the WeatherStation becomes larger,
   more complex, and harder to maintain.

===============================================================================
*/