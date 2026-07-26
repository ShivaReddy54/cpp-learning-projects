/*

Intent: Define the skeleton of an algorithm in an operation, deferring some steps to subclasses.
        Template Method lets subclasses redefine certain steps of an algorithm without changing the algorithm's structure.

EX: A recipe for baking. The steps are always: (1) Prepare ingredients → (2) Mix → (3) Bake → (4) Decorate.
    The structure is fixed (template), but each step can be different: preparing cake ingredients vs bread ingredients.
    Subclasses fill in the specific steps, but the overall sequence never changes.

*/

#include <iostream>
#include <string>

// ===== Different data parsers with similar structure =====
class CSVParser{
public:
    void parse(const std::string& file){
        // Step 1: Open file
        std::cout << "CSVParser: Opening file " << file << std::endl;
        
        // Step 2: Read raw data (CSV specific)
        std::cout << "CSVParser: Reading comma-separated values" << std::endl;
        
        // Step 3: Parse data (CSV specific)
        std::cout << "CSVParser: Parsing into rows/columns" << std::endl;
        
        // Step 4: Process data (CSV specific)
        std::cout << "CSVParser: Processing CSV data" << std::endl;
        
        // Step 5: Close file
        std::cout << "CSVParser: Closing file" << std::endl;
        
        // PROBLEM: Steps 1 and 5 are IDENTICAL across all parsers
        // PROBLEM: The ALGORITHM STRUCTURE is identical — only details differ
        // PROBLEM: Lots of code duplication
    }
};

class JSONParser{
public:
    void parse(const std::string& file){
        // Step 1: Open file — SAME as CSVParser!
        std::cout << "JSONParser: Opening file " << file << std::endl;
        
        // Step 2: Read raw data (JSON specific)
        std::cout << "JSONParser: Reading JSON string" << std::endl;
        
        // Step 3: Parse data (JSON specific)
        std::cout << "JSONParser: Parsing into key-value pairs" << std::endl;
        
        // Step 4: Process data (JSON specific)
        std::cout << "JSONParser: Processing JSON data" << std::endl;
        
        // Step 5: Close file — SAME as CSVParser!
        std::cout << "JSONParser: Closing file" << std::endl;
        
        // PROBLEM: Opening/closing duplicated
        // PROBLEM: Algorithm structure duplicated
        // PROBLEM: If we add a step (e.g., "validate") we must modify ALL parsers
    }
};

class XMLParser{
public:
    void parse(const std::string& file){
        // Step 1: Open file — SAME again!
        std::cout << "XMLParser: Opening file " << file << std::endl;
        
        // Step 2: Read raw data (XML specific)
        std::cout << "XMLParser: Reading XML tags" << std::endl;
        
        // Step 3: Parse data (XML specific)
        std::cout << "XMLParser: Parsing into DOM tree" << std::endl;
        
        // Step 4: Process data (XML specific)
        std::cout << "XMLParser: Processing XML data" << std::endl;
        
        // Step 5: Close file — SAME again!
        std::cout << "XMLParser: Closing file" << std::endl;
    }
};

int main() {
    CSVParser csv;
    JSONParser json;
    XMLParser xml;

    csv.parse("data.csv");
    std::cout << std::endl;
    json.parse("data.json");
    std::cout << std::endl;
    xml.parse("data.xml");

    // PROBLEM: 3 classes with nearly identical structure
    // PROBLEM: Common steps (open, close) duplicated 3 times
    // PROBLEM: Adding a common step = modifying ALL 3 classes
    // PROBLEM: Algorithm structure not enforced — could forget a step

}

/*
===========================================================
           PROBLEMS WITH THIS APPROACH (BEFORE)
===========================================================

CSVParser.parse():
    Open → ReadCSV → ParseCSV → ProcessCSV → Close

JSONParser.parse():
    Open → ReadJSON → ParseJSON → ProcessJSON → Close

XMLParser.parse():
    Open → ReadXML → ParseXML → ProcessXML → Close


Common Algorithm Structure:

    Open → Read ? → Parse ? → Process ? → Close
      ↑        ↑         ↑          ↑         ↑
    Fixed    Varies    Varies     Varies    Fixed


Problems:

⚠️ Code duplication
    • The Open() and Close() logic is repeated in every parser.

⚠️ Algorithm duplication
    • Every parser follows the same sequence of steps,
      but each class reimplements the entire algorithm.

⚠️ Poor maintainability
    • Adding a new common step (e.g., Validate() or Log())
      requires modifying every parser implementation.

⚠️ No algorithm enforcement
    • Nothing guarantees that subclasses will follow
      the correct processing sequence.

⚠️ Inconsistent implementations
    • A parser might accidentally skip a required step
      or execute the steps in the wrong order.

===========================================================
*/