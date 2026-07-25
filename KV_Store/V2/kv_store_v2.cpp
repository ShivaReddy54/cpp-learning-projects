// kv_store_v2.cpp
// Custom open-addressing hash map with linear probing
// No linked lists, all data in a flat array

#include <iostream>
#include <string>
#include <optional>
#include <vector>
#include <cstdint>

class KVStoreV2{
private:
    // Each slot has a state, a key, and a value
    enum SlotState { EMPTY, OCCUPIED, DELETED };

    struct Slot {
        SlotState state = EMPTY;
        std::string key;
        std::string value;
    };

    // The array of slots. Capacity must be a power of 2
    std::vector<Slot> slots_;

    // The number of keys currently stored
    size_t num_occupied_ = 0;
    
    // Total capacity (always a power of 2)
    size_t capacity_;

    // When the hash table gets too full, we need to resize
    // load factor = num_occupied_ / capacity_
    // We resize when load factor exceeds this threshold
    static constexpr double MAX_LOAD_FACTOR = 0.75;

    // ─── HASH FUNCTION ───
    // This converts any string into a number (index)
    // We use a simple but effective hash function
    uint64_t hash(const std::string& key){
        // FNV-1a hash — simple, fast, good distribution
        uint64_t h = 14695981039346656037ULL;   // FNV offset basis (64-bit)
        for(char c : key){
            h ^= static_cast<uint64_t>(c);
            h *= 1099511628211ULL;               // FNV prime (64-bit)
        }
        return h;
    }

    // ─── FIND SLOT FOR A KEY ───
    // This is the core search function
    // It returns the index where the key IS (if found)
    // or the index where the key SHOULD GO (if not found)
    // is_found: set to true if key was found, false if not
    size_t find_slot(const std::string& key, bool& is_found){
        is_found = false;

        // Step 1: Compute the starting index from the hash
        uint64_t h = hash(key);
        
        // Why & (capacity_ - 1) instead of % capacity_?
        // Because capacity_ is always a power of 2,
        // h % (power_of_2)  is the same as  h & (power_of_2 - 1)
        // BUT bitwise AND is MUCH faster than modulo (1 cycle vs ~25 cycles)
        size_t index = h & (capacity_ - 1);

        // Step 2: Linear probing — check this slot, then the next, then the next...
        size_t first_deleted = capacity_;  // Remember first DELETED slot we see
        
        for(size_t probes = 0; probes < capacity_; probes++){
            Slot& slot = slots_[index];

            if(slot.state == OCCUPIED){
                // This slot has data. Check if it's our key.
                if(slot.key == key) {
                    // FOUND IT!
                    is_found = true;
                    return index;
                }
                // Not our key. Collision. Keep probing.
            }
            else if(slot.state == DELETED){
                // This slot was deleted. We can put our key here if we're inserting.
                // But we must keep probing because our key might be further ahead.
                if(first_deleted == capacity_){
                    first_deleted = index;   // Remember this position
                }
            }
            else if (slot.state == EMPTY) {
                // EMPTY means: the key is DEFINITELY not further ahead.
                // (If it existed, it would have been placed here or earlier,
                //  because linear probing always uses the first available slot.)
                
                // Key not found. Return the best available slot.
                if(first_deleted != capacity_){
                    // There was a DELETED slot earlier — use that (reuse deleted space)
                    return first_deleted;
                }
                // No deleted slots seen — use this EMPTY slot
                return index;
            }

            // Move to the next slot (wrapping around to 0 if we reach the end)
            index = (index + 1) & (capacity_ - 1);
        }

        // We checked every slot and they're all OCCUPIED or DELETED.
        // Use the first DELETED slot we found.
        return first_deleted;
    }

    // ─── RESIZE ───
    // When the table gets too full, we create a bigger table
    // and move all existing keys into it
    void resize(){
        size_t old_capacity = capacity_;
        std::vector<Slot> old_slots = std::move(slots_);  // Take the old data out

        // Double the capacity (always power of 2)
        capacity_ = old_capacity * 2;
        slots_.resize(capacity_);  // Create new, bigger array (all EMPTY)

        // Re-insert all OCCUPIED keys into the new table
        // Their positions may change because the capacity changed
        num_occupied_ = 0;  // Reset count (we'll count as we re-insert)
        
        for(size_t i = 0; i < old_capacity; i++){
            if(old_slots[i].state == OCCUPIED){
                // Insert this key-value pair into the new, bigger table
                bool found;
                size_t idx = find_slot(old_slots[i].key, found);
                // found should always be false (new table, no duplicates)
                slots_[idx].state = OCCUPIED;
                slots_[idx].key = std::move(old_slots[i].key);
                slots_[idx].value = std::move(old_slots[i].value);
                num_occupied_++;
            }
        }
    }

public:
    // ─── CONSTRUCTOR ───
    explicit KVStoreV2(size_t initial_capacity = 16){
        // Capacity must be a power of 2
        // Find the next power of 2 >= initial_capacity
        capacity_ = 1;
        while(capacity_ < initial_capacity){
            capacity_ *= 2;
        }
        slots_.resize(capacity_);
    }

    // ─── SET (INSERT OR UPDATE) ───
    void set(const std::string& key, const std::string& value){
        // Check if we need to resize first
        // We resize when: num_occupied_ / capacity_ > MAX_LOAD_FACTOR
        // Which is: num_occupied_ > MAX_LOAD_FACTOR * capacity_
        if(num_occupied_ >= static_cast<size_t>(MAX_LOAD_FACTOR * capacity_)){
            resize();
        }

        bool found;
        size_t index = find_slot(key, found);

        if(found){
            // Key already exists — UPDATE the value
            slots_[index].value = value;
        }else{
            // Key doesn't exist — INSERT new key-value pair
            slots_[index].state = OCCUPIED;
            slots_[index].key = key;
            slots_[index].value = value;
            num_occupied_++;
        }
    }

    // ─── GET (LOOKUP) ───
    std::optional<std::string> get(const std::string& key){
        bool found;
        size_t index = find_slot(key, found);

        if(found){
            return slots_[index].value;
        }
        return std::nullopt;  // Not found
    }

    // ─── DELETE ───
    bool delete_key(const std::string& key){
        bool found;
        size_t index = find_slot(key, found);

        if(found){
            slots_[index].state = DELETED;  // Mark as DELETED (not EMPTY!)
            // Note: we don't clear key/value — they'll be overwritten eventually
            // This saves time on deletion
            num_occupied_--;
            return true;
        }
        return false;  // Key didn't exist
    }

    // ─── EXISTS ───
    bool exists(const std::string& key){
        bool found;
        find_slot(key, found);
        return found;
    }

    // ─── COUNT ───
    size_t count() const{
        return num_occupied_;
    }

    // ─── CLEAR ───
    void clear(){
        for(size_t i = 0; i < capacity_; i++){
            slots_[i].state = EMPTY;
        }
        num_occupied_ = 0;
    }
};

#ifndef BENCHMARK_MODE
int main(){
    KVStoreV2 store(16); // Start with 16 slots

    // Test SET
    store.set("Alice", "555-1234");
    store.set("Bob", "555-5678");
    store.set("Charlie", "555-9012");
    store.set("David", "555-3333");
    store.set("Eve", "555-7777");

    // Test GET
    auto r1 = store.get("Alice");
    std::cout << "GET Alice: " << (r1 ? *r1 : "NOT FOUND") << std::endl;

    auto r2 = store.get("Bob");
    std::cout << "GET Bob: " << (r2 ? *r2 : "NOT FOUND") << std::endl;

    auto r3 = store.get("Unknown");
    std::cout << "GET Unknown: " << (r3 ? *r3 : "NOT FOUND") << std::endl;

    // Test UPDATE
    store.set("Alice", "555-9999");
    auto r4 = store.get("Alice");
    std::cout << "GET Alice after update: " << (r4 ? *r4 : "NOT FOUND") << std::endl;

    // Test DELETE
    bool d1 = store.delete_key("Bob");
    std::cout << "DELETE Bob: " << (d1 ? "SUCCESS" : "NOT FOUND") << std::endl;

    auto r5 = store.get("Bob");
    std::cout << "GET Bob after delete: " << (r5 ? *r5 : "NOT FOUND") << std::endl;

    // Test that we can still find keys after a deletion
    auto r6 = store.get("Charlie");
    std::cout << "GET Charlie after Bob deleted: " << (r6 ? *r6 : "NOT FOUND") << std::endl;

    // Test COUNT
    std::cout << "Count: " << store.count() << std::endl;

    // Test inserting after deletion (should reuse DELETED slot)
    store.set("Frank", "555-4444");
    std::cout << "GET Frank: " << (*store.get("Frank")) << std::endl;
    std::cout << "Count after insert: " << store.count() << std::endl;

    // Test resize — insert many keys to trigger resize
    std::cout << "\n--- Testing Resize ---" << std::endl;
    for(int i = 0; i < 100; i++){
        store.set("key_" + std::to_string(i), "value_" + std::to_string(i));
    }
    std::cout << "Count after 100 inserts: " << store.count() << std::endl;
    
    // Verify some of those keys still work
    auto r7 = store.get("key_50");
    std::cout << "GET key_50: " << (r7 ? *r7 : "NOT FOUND") << std::endl;
    
    // Verify old keys still work after resize
    auto r8 = store.get("Alice");
    std::cout << "GET Alice after resize: " << (r8 ? *r8 : "NOT FOUND") << std::endl;
}
#endif