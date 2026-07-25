#include <iostream>
#include <string>
#include <vector>

// ===== Subject interface (common for real object and proxy) =====
class Image{
public:
    virtual ~Image() = default;
    virtual void display() = 0;
    virtual void showThumbnail() = 0;
    virtual std::string getFilename() = 0;
};

// ===== Real Subject — the heavy, expensive object =====
class HighResolutionImage : public Image{
private:
    std::string filename;
    std::vector<int> imageData;
    bool loaded;

public:
    HighResolutionImage(const std::string& file) : filename(file), loaded(false){
        // NOT loaded immediately — loaded when first needed!
    }

    void loadImage(){
        if(!loaded){
            std::cout << "Loading high-resolution image: " << filename << " (3 seconds, 50MB)..." << std::endl;
            for(int i = 0; i < 1000000; i++) imageData.push_back(i);

            loaded = true;
            std::cout << "Image loaded!" << std::endl;
        }
    }

    void display() override{
        loadImage();  // Load only when needed!
        std::cout << "Displaying full image: " << filename << std::endl;
    }

    void showThumbnail() override{
        loadImage();
        std::cout << "Showing thumbnail of: " << filename << std::endl;
    }

    std::string getFilename() override { return filename; }
};

// ===== STEP 1: Virtual Proxy — lazy loading =====
class ImageProxy : public Image{
private:
    HighResolutionImage* realImage;  // Not created until needed!
    std::string filename;
    bool accessAuthorized;

public:
    ImageProxy(const std::string& file, bool authorized = true) 
        : realImage(nullptr), filename(file), accessAuthorized(authorized) {
        std::cout << "Creating lightweight proxy for: " << filename << " (instant, almost no memory)" << std::endl;
    }

    ~ImageProxy(){
        if(realImage) delete realImage;
    }

    // LAZY LOADING: Create real image only when first needed!
    void display() override{
        // ACCESS CONTROL: Check authorization first!
        if(!accessAuthorized){
            std::cout << "ACCESS DENIED: You don't have permission to view " << filename << std::endl;
            return;
        }

        // Lazy initialization — create real object only when needed
        if(realImage == nullptr){
            std::cout << "Proxy: Real image not loaded yet. Loading now..." << std::endl;
            realImage = new HighResolutionImage(filename);
        }

        // Delegate to real object
        realImage->display();
    }

    void showThumbnail() override{
        // Thumbnails can be shown without full authorization
        // or with a lightweight placeholder instead of loading the full image
        if(realImage == nullptr){
            // Show a lightweight placeholder thumbnail — NO full load needed!
            std::cout << "Showing lightweight placeholder thumbnail for: " << filename << std::endl;
            return;
        }
        realImage->showThumbnail();
    }

    std::string getFilename() override { return filename; }
};

// ===== STEP 2: Caching Proxy — adds caching on top =====
class CachingImageProxy : public Image{
private:
    HighResolutionImage* realImage;
    std::string filename;
    bool cached;
    std::string cachedDisplayResult;

public:
    CachingImageProxy(const std::string& file)
        : realImage(nullptr), filename(file), cached(false) {}

    ~CachingImageProxy(){
        if(realImage) delete realImage;
    }

    void display() override{
        // CACHING: If we've displayed before, reuse the cached result!
        if(cached){
            std::cout << "Caching Proxy: Using cached result for " << filename << std::endl;
            std::cout << cachedDisplayResult << std::endl;
            return;
        }

        if(realImage == nullptr) realImage = new HighResolutionImage(filename);

        realImage->display();
        cachedDisplayResult = "Displaying full image: " + filename;
        cached = true;
    }

    void showThumbnail() override{
        if(realImage == nullptr){
            std::cout << "Showing lightweight placeholder thumbnail for: " << filename << std::endl;
            return;
        }
        realImage->showThumbnail();
    }

    std::string getFilename() override { return filename; }
};

// ===== CLIENT CODE =====
int main() {
    std::cout << "=== Virtual Proxy (Lazy Loading + Access Control) ===" << std::endl;
    std::cout << std::endl;

    // Create proxies — INSTANT, almost no memory!
    Image* img1 = new ImageProxy("landscape.jpg", true);   // Authorized
    Image* img2 = new ImageProxy("portrait.jpg", true);    // Authorized
    Image* img3 = new ImageProxy("secret.jpg", false);     // NOT authorized!

    std::cout << std::endl;

    // Show thumbnails — NO loading needed! Just placeholders!
    std::cout << "--- Thumbnails (no full loading!) ---" << std::endl;
    img1->showThumbnail();
    img2->showThumbnail();
    img3->showThumbnail();  // Even unauthorized can see thumbnail

    std::cout << std::endl;

    // Display img1 — NOW the real image loads (lazy!)
    std::cout << "--- Display landscape (first time — loads) ---" << std::endl;
    img1->display();

    std::cout << std::endl;

    // Display img1 again — already loaded, just delegates
    std::cout << "--- Display landscape (second time — already loaded) ---" << std::endl;
    img1->display();

    std::cout << std::endl;

    // img2 thumbnail viewed but NOT displayed — real image NEVER loaded!
    std::cout << "--- Portrait never fully displayed, never loaded! ---" << std::endl;
    std::cout << "img2 saved 50MB and 3 seconds!" << std::endl;

    std::cout << std::endl;

    // Access control: unauthorized user can't view full image!
    std::cout << "--- Access Control: unauthorized user tries to view ---" << std::endl;
    img3->display();  // ACCESS DENIED!

    std::cout << std::endl;
    std::cout << std::endl;

    // ===== Caching Proxy Demo =====
    std::cout << "=== Caching Proxy (Lazy Loading + Caching) ===" << std::endl;
    std::cout << std::endl;

    Image* cachedImg = new CachingImageProxy("ocean.jpg");

    // First display — loads and caches
    std::cout << "--- First display (loads + caches) ---" << std::endl;
    cachedImg->display();

    std::cout << std::endl;

    // Second display — uses cache!
    std::cout << "--- Second display (uses cache!) ---" << std::endl;
    cachedImg->display();

    // Cleanup
    delete img1;
    delete img2;
    delete img3;
    delete cachedImg;

}