#include <iostream>
#include <string>

// ===== STEP 1: Abstract base class with Template Method =====
class DataParser{
public:
    // TEMPLATE METHOD — defines the ALGORITHM SKELETON (final = can't override)
    virtual void parse(const std::string& file) final{
        // The structure is FIXED here — subclasses can't change it!
        openFile(file);          // Step 1: Common
        readData();              // Step 2: Varies → subclass implements
        parseData();             // Step 3: Varies → subclass implements
        processData();           // Step 4: Varies → subclass implements
        closeFile(file);         // Step 5: Common
    }

    virtual ~DataParser() = default;

protected:
    // COMMON steps — implemented in base class (subclasses don't override)
    virtual void openFile(const std::string& file){
        std::cout << "DataParser: Opening file " << file << std::endl;
    }

    virtual void closeFile(const std::string& file){
        std::cout << "DataParser: Closing file " << file << std::endl;
    }

    // ABSTRACT steps — subclasses MUST implement (pure virtual)
    virtual void readData() = 0;
    virtual void parseData() = 0;

    // HOOK step — optional override (has default implementation)
    virtual void processData(){
        std::cout << "DataParser: Processing data (default)" << std::endl;
    }
};

// ===== STEP 2: Concrete subclasses fill in the specific steps =====
class CSVParser : public DataParser{
protected:
    void readData() override{
        std::cout << "CSVParser: Reading comma-separated values" << std::endl;
    }

    void parseData() override{
        std::cout << "CSVParser: Parsing into rows/columns" << std::endl;
    }

    void processData() override{
        std::cout << "CSVParser: Processing CSV data" << std::endl;
    }
};

class JSONParser : public DataParser{
protected:
    void readData() override{
        std::cout << "JSONParser: Reading JSON string" << std::endl;
    }

    void parseData() override {
        std::cout << "JSONParser: Parsing into key-value pairs" << std::endl;
    }

    // processData() NOT overridden — uses DEFAULT hook!
};

class XMLParser : public DataParser{
protected:
    void readData() override{
        std::cout << "XMLParser: Reading XML tags" << std::endl;
    }

    void parseData() override{
        std::cout << "XMLParser: Parsing into DOM tree" << std::endl;
    }

    void processData() override{
        std::cout << "XMLParser: Processing XML data with validation" << std::endl;
    }
};

// ===== STEP 3: Adding a new parser is trivial! =====
class YAMLParser : public DataParser{
protected:
    void readData() override{
        std::cout << "YAMLParser: Reading YAML document" << std::endl;
    }

    void parseData() override{
        std::cout << "YAMLParser: Parsing into nested structure" << std::endl;
    }

    // processData() uses default hook
};

// ===== CLIENT CODE =====
int main() {
    CSVParser csv;
    JSONParser json;
    XMLParser xml;
    YAMLParser yaml;

    // ===== All parsers follow the SAME algorithm structure! =====
    std::cout << "=== CSV Parser ===" << std::endl;
    csv.parse("data.csv");

    std::cout << std::endl;

    std::cout << "=== JSON Parser ===" << std::endl;
    json.parse("data.json");

    std::cout << std::endl;

    std::cout << "=== XML Parser ===" << std::endl;
    xml.parse("data.xml");

    std::cout << std::endl;

    std::cout << "=== YAML Parser (NEW — no modification to base!) ===" << std::endl;
    yaml.parse("data.yaml");

    // The algorithm structure is FIXED by the Template Method
    // You CAN'T forget a step — the base class enforces it
    // Common code is in ONE place (base class)
    // Each subclass only fills in the varying parts

}

/*
===========================================================
                BEFORE vs AFTER
           (Template Method Pattern)
===========================================================

BEFORE:

CSVParser {
    openFile()
    readData()
    parseData()
    processData()
    closeFile()
}

JSONParser {
    openFile()      ← DUPLICATED
    readData()
    parseData()
    processData()
    closeFile()     ← DUPLICATED
}

XMLParser {
    openFile()      ← DUPLICATED
    ...
    closeFile()     ← DUPLICATED
}


AFTER:

DataParser (Base Class)
│
├── parse()          ← TEMPLATE METHOD (final)
│     openFile()         ← Common implementation
│     readData()         ← Abstract (implemented by subclass)
│     parseData()        ← Abstract (implemented by subclass)
│     processData()      ← Hook (optional override)
│     closeFile()        ← Common implementation
│
├── CSVParser
│      ├── readData()
│      ├── parseData()
│      └── processData()
│
├── JSONParser
│      ├── readData()
│      ├── parseData()
│      └── (uses default processData())
│
├── XMLParser
│      ├── readData()
│      ├── parseData()
│      └── processData()
│
└── YAMLParser
       ├── readData()
       ├── parseData()
       └── (uses default processData())


Algorithm Structure:
    Open → Read → Parse → Process → Close
          ↑        ↑         ↑
       Varies   Varies    Optional Hook

The overall workflow is ENFORCED by the base class,
while subclasses customize only the varying steps.


Problems Solved:

⚠️ Code duplication
    ✅ Common logic (Open/Close/template flow) exists only once
       in the base class.

⚠️ Algorithm structure not enforced
    ✅ The template method guarantees the correct execution order.

⚠️ Hard to modify
    ✅ Add or modify a common step once in the base class—
       every parser inherits the change automatically.

⚠️ Easy to forget a step
    ✅ Subclasses cannot skip required steps because the
       base class controls the algorithm.

===========================================================
*/