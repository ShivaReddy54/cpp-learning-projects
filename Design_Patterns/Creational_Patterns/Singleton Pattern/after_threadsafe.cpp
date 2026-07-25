#include <iostream>
#include <string>
#include <mutex>


class DatabaseConnection {
private:
    std::string connectionString;
    bool isConnected;
    static DatabaseConnection* instance;
    static std::mutex mtx;  // Mutex for thread safety

    DatabaseConnection() : connectionString("Server=MainDB"), isConnected(false){
        std::cout << "Creating the DatabaseConnection" << std::endl;
        isConnected = true;
    }

    DatabaseConnection(const DatabaseConnection&) = delete;
    DatabaseConnection& operator=(const DatabaseConnection&) = delete;

public:
    static DatabaseConnection* getInstance(){
        std::lock_guard<std::mutex> lock(mtx);  // Thread-safe!
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
};

DatabaseConnection* DatabaseConnection::instance = nullptr;
std::mutex DatabaseConnection::mtx;



/*
===============================================================================
                    Thread Safety Issue in Basic Singleton
===============================================================================

The basic Singleton implementation shown in after.cpp is **NOT thread-safe**.

Problem:
If two or more threads call getInstance() at the same time, they may all
observe that no instance exists and each create a new object.

Example:

Thread 1                         Thread 2
---------                        ---------
getInstance()                    getInstance()
      │                                │
instance == nullptr?  YES        instance == nullptr?  YES
      │                                │
Create Object #1                 Create Object #2
      │                                │
Return Object #1                 Return Object #2

Result:
  • Two different Singleton objects are created.
  • The Singleton guarantee is violated.
  • Resources are wasted.
  • The application may behave unpredictably.

This race condition occurs because multiple threads access and modify the
shared 'instance' pointer simultaneously without any synchronization.

===============================================================================
*/