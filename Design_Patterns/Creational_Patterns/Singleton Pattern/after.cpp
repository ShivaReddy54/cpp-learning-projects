#include <iostream>
#include <string>

class DatabaseConnection {
private:
    std::string connectionString;
    bool isConnected;

    // ===== STEP 1: Private constructor =====
    DatabaseConnection() : connectionString("Server=MainDB"), isConnected(false){
        std::cout << "Creating the DatabaseConnection" << std::endl;
        isConnected = true;
    }

    // ===== STEP 2: Delete copy constructor and assignment =====
    DatabaseConnection(const DatabaseConnection&) = delete;
    DatabaseConnection& operator=(const DatabaseConnection&) = delete;

    // ===== STEP 3: Static instance pointer =====
    static DatabaseConnection* instance;

public:
    // ===== STEP 4: Static access method =====
    static DatabaseConnection* getInstance(){
        if(instance == nullptr){
            instance = new DatabaseConnection();
        }
        return instance;
    }

    void query(const std::string& sql){
        if(isConnected){
            std::cout << "Executing: " << sql << std::endl;
        }
    }

    ~DatabaseConnection(){
        std::cout << "Destroying DatabaseConnection" << std::endl;
    }
};

// ===== STEP 5: Initialize static member =====
DatabaseConnection* DatabaseConnection::instance = nullptr;

// ===== CLIENT CODE =====
int main() {
    // Developer A gets the connection
    DatabaseConnection* db1 = DatabaseConnection::getInstance();
    db1->query("SELECT * FROM users");

    // Developer B gets the SAME connection
    DatabaseConnection* db2 = DatabaseConnection::getInstance();
    db2->query("SELECT * FROM orders");

    // Developer C gets the SAME connection
    DatabaseConnection* db3 = DatabaseConnection::getInstance();
    db3->query("SELECT * FROM products");

    std::cout << "db1 address: " << db1 << std::endl;
    std::cout << "db2 address: " << db2 << std::endl;
    std::cout << "db3 address: " << db3 << std::endl;

    // All three point to the SAME object
    std::cout << "All same? " << (db1 == db2 && db2 == db3 ? "YES" : "NO") << std::endl;

    // The following lines will NOT compile (enforcement!):
    // DatabaseConnection copy = *db1;           // Error: copy constructor deleted
    // DatabaseConnection* db4 = new DatabaseConnection(); // Error: private constructor
}


/*
===============================================================================
                    How Singleton Solves the Problems
===============================================================================

BEFORE (Without Singleton)

Developer A ──► new DatabaseConnection() ──► Object #1
Developer B ──► new DatabaseConnection() ──► Object #2
Developer C ──► new DatabaseConnection() ──► Object #3

Result:
  • 3 different objects
  • 3 separate database connections
  • Wasted resources
  • No centralized control


AFTER (With Singleton)

Developer A ──► getInstance() ──┐
Developer B ──► getInstance() ──┼──► SAME DatabaseConnection Object
Developer C ──► getInstance() ──┘

Result:
  • 1 shared object
  • 1 database connection
  • Controlled access
  • Consistent application state


===============================================================================
                           Key Singleton Features
===============================================================================

+----------------------+------------------------------------------------------+
| Feature              | How It Works                                         |
+----------------------+------------------------------------------------------+
| Single Instance      | Private constructor prevents object creation using   |
|                      | 'new' from outside the class.                        |
+----------------------+------------------------------------------------------+
| Global Access        | getInstance() is a static method, so it can be       |
|                      | accessed from anywhere in the application.           |
+----------------------+------------------------------------------------------+
| Lazy Initialization  | The object is created only when getInstance() is     |
|                      | called for the first time.                           |
+----------------------+------------------------------------------------------+
| No Duplication       | Copy constructor and copy assignment operator are    |
|                      | deleted, preventing accidental copies.               |
+----------------------+------------------------------------------------------+

===============================================================================
*/