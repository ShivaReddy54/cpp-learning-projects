// kv_store_v1.cpp
// The simplest possible key-value store
// This uses std::unordered_map which is a basic hash table

#include <iostream>
#include <string>
#include <unordered_map>
#include <optional>

class KVStoreV1{
private:
    // This is our storage. std::unordered_map is a hash table
    // provided by the C++ standard library.
    // Key is std::string, Value is std::string
    std::unordered_map<std::string, std::string> data_;

public:
    // SET: Store a key-value pair
    // If the key already exists, replace its value
    void set(const std::string& key, const std::string& value){
        data_[key] = value;
        // data_[key] = value does:
        //   - If key exists: replace the value
        //   - If key doesn't exist: create a new entry
    }

    // GET: Find the value for a key
    // If the key doesn't exist, return nothing (std::nullopt)
    std::optional<std::string> get(const std::string& key){
        auto it = data_.find(key);
        // data_.find(key) returns an "iterator" — like a pointer to the entry
        
        if(it != data_.end()){
            // Found the key! Return the value
            return it->second;
        }else{
            // Key not found. Return "nothing" (nullopt)
            return std::nullopt;
        }
    }

    // DELETE: Remove a key and its value
    // Returns true if the key existed and was deleted
    // Returns false if the key didn't exist
    bool delete_key(const std::string& key){
        auto it = data_.find(key);
        if(it != data_.end()){
            data_.erase(it);
            return true;
        }
        return false;
    }

    // EXISTS: Check if a key exists
    bool exists(const std::string& key){
        return data_.find(key) != data_.end();
    }

    // COUNT: How many keys are stored
    size_t count(){
        return data_.size();
    }

    // CLEAR: Remove all keys
    void clear(){
        data_.clear();
    }
};

int main() {
    KVStoreV1 store;

    // Test SET
    store.set("Alice", "555-1234");
    store.set("Bob", "555-5678");
    store.set("Charlie", "555-9012");

    // Test GET
    auto result1 = store.get("Alice");
    if(result1){
        std::cout << "GET Alice: " << *result1 << std::endl;
    }else{
        std::cout << "GET Alice: NOT FOUND" << std::endl;
    }

    auto result2 = store.get("David");  // Key doesn't exist
    if(result2){
        std::cout << "GET David: " << *result2 << std::endl;
    }else{
        std::cout << "GET David: NOT FOUND" << std::endl;
    }

    // Test UPDATE (SET on existing key)
    store.set("Alice", "555-9999");
    auto result3 = store.get("Alice");
    std::cout << "GET Alice after update: " << *result3 << std::endl;

    // Test DELETE
    bool deleted = store.delete_key("Bob");
    std::cout << "DELETE Bob: " << (deleted ? "SUCCESS" : "NOT FOUND") << std::endl;

    auto result4 = store.get("Bob");
    std::cout << "GET Bob after delete: " << (result4 ? *result4 : "NOT FOUND") << std::endl;

    // Test COUNT
    std::cout << "Count: " << store.count() << std::endl;
}