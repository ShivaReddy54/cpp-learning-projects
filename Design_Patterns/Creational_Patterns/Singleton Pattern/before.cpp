#include <iostream>
#include <vector>
#include <string>

#include <thread>
#include <chrono>

class DatabaseConnection{
private:
    std::string connectionString;
    bool isConnected;

public:
    DatabaseConnection(const std::string& connStr) : connectionString(connStr), isConnected(false){
        std::cout << "Creating DatabaseConnection with: " << connectionString << std::endl;

        // Simulate expensive connection setup
        // std::this_thread::sleep_for(std::chrono::seconds(2));

        isConnected = true;
    }

    void query(const std::string& sql){
        if(isConnected){
            std::cout << "Executing: " << sql << std::endl;

            // Simulate query execution time
            // std::this_thread::sleep_for(std::chrono::seconds(2));
        }
    }

    ~DatabaseConnection() {
        std::cout << "Destroying DatabaseConnection" << std::endl;
    }
};

// ===== CLIENT CODE =====
int main() {
    // Developer A creates a connection
    DatabaseConnection* db1 = new DatabaseConnection("Server=MainDB");
    db1->query("SELECT * FROM users");

    // Developer B creates ANOTHER connection (not knowing A already did)
    DatabaseConnection* db2 = new DatabaseConnection("Server=MainDB");
    db2->query("SELECT * FROM orders");

    // Developer C creates YET ANOTHER connection
    DatabaseConnection* db3 = new DatabaseConnection("Server=MainDB");
    db3->query("SELECT * FROM products");

    std::cout << "db1 address: " << db1 << std::endl;
    std::cout << "db2 address: " << db2 << std::endl;
    std::cout << "db3 address: " << db3 << std::endl;

    delete db1;
    delete db2;
    delete db3;
}


/*

When every developer creates their own DatabaseConnection object, several
issues arise:

+----------------------------+-----------------------------------------------+
| Problem                    | Explanation                                   |
+----------------------------+-----------------------------------------------+
| Multiple instances created | Three different objects connect to the same   |
|                            | database, wasting resources.                  |
+----------------------------+-----------------------------------------------+
| Resource waste             | Each connection consumes memory, network      |
|                            | ports, and database connection pool slots.    |
+----------------------------+-----------------------------------------------+
| Inconsistent state         | Changes made through one connection may not   |
|                            | be reflected in others.                       |
+----------------------------+-----------------------------------------------+
| No enforcement             | Nothing prevents developers from creating     |
|                            | additional DatabaseConnection objects.        |
+----------------------------+-----------------------------------------------+
| No global access           | The connection object must be passed around   |
|                            | manually throughout the application.          |
+----------------------------+-----------------------------------------------+


*/