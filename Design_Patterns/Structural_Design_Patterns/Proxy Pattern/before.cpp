/*

Intent: Provide a substitute or placeholder for another object to control access to it.
    The proxy controls access to the real object for various reasons: security, lazy loading, caching, remote access, etc.

EX: A bank ATM is a proxy for the bank's central server. You don't directly access the server — the ATM acts as a proxy

*/

#include <iostream>
#include <string>
#include <vector>

// ===== Heavy resource that's expensive to load =====
class HighResolutionImage{
private:
    std::string filename;
    std::vector<int> imageData;  // Massive data (simulated)
    bool loaded;

public:
    HighResolutionImage(const std::string& file) : filename(file), loaded(false){
        loadImage();  // Expensive! Always loads immediately!
    }

    void loadImage() {
        std::cout << "Loading high-resolution image: " << filename 
                  << " (3 seconds, 50MB)..." << std::endl;

        // Simulate loading massive data
        for(int i = 0; i < 1000000; i++) imageData.push_back(i);

        loaded = true;
        std::cout << "Image loaded!" << std::endl;
    }

    void display(){
        if(loaded) std::cout << "Displaying full image: " << filename << std::endl;
    }

    void showThumbnail(){
        if(loaded) std::cout << "Showing thumbnail of: " << filename << std::endl;
    }

    std::string getFilename() { return filename; }
};

int main() {
    // PROBLEM 1: Image loads immediately even if we only want a thumbnail!
    // PROBLEM 2: No access control — anyone can call display()
    // PROBLEM 3: No caching — display() always processes full data

    std::cout << "=== Creating 3 images (all load IMMEDIATELY — EXPENSIVE!) ===" << std::endl;
    std::cout << std::endl;

    // User opens a gallery with 3 images
    // Even if they only view 1, ALL 3 are loaded!
    HighResolutionImage img1("landscape.jpg");
    HighResolutionImage img2("portrait.jpg");
    HighResolutionImage img3("sunset.jpg");

    std::cout << std::endl;

    // User only wants to see thumbnail of img1
    img1.showThumbnail();
    // img2 and img3 loaded unnecessarily — wasted 100MB + 9 seconds!

    // User finally views full image of img1
    img1.display();
    // img2 and img3 STILL not viewed — total waste!

    // PROBLEM: No lazy loading
    // PROBLEM: No access control (who can view full image?)
    // PROBLEM: No caching (viewing twice processes twice)

}

/*
===============================================================================
                     Problems Without the Proxy Pattern
===============================================================================

Image Gallery Startup

    img1 (landscape)  ──► LOADED  (3 sec, 50 MB)
    img2 (portrait)   ──► LOADED  (3 sec, 50 MB)
                              ↑
                       User never views it!

    img3 (sunset)     ──► LOADED  (3 sec, 50 MB)
                              ↑
                       User never views it!


Resource Usage

    Total Loaded:
        • 9 seconds
        • 150 MB

    Actually Needed:
        • 3 seconds
        • 50 MB

Result:
    67% of the time and memory were wasted loading objects that
    were never used.


Problems:

❌ No Lazy Loading
   Every heavy object is created immediately, even if it is never
   accessed by the user.

❌ No Access Control
   Any client can directly access the real object without any
   authorization or validation.

❌ No Caching
   Repeated requests perform the same expensive operation again
   instead of reusing existing results.

❌ Wasted Resources
   Memory, CPU time, and I/O are consumed by objects that may never
   actually be used.

❌ Slow Startup
   The application waits for all heavyweight objects to be created
   before it becomes usable.

===============================================================================
*/