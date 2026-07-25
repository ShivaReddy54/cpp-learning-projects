// kv_store_v3.cpp
// SIMD-accelerated hash map (Swiss Table style)
// This is the version that starts to be HFT-relevant

#include <iostream>
#include <string>
#include <optional>
#include <vector>
#include <cstdint>
#include <cstring>

// ─── SSE2 SIMD HEADERS ───
// These are available on Windows with MSVC
#include <emmintrin.h>  // SSE2: _mm_loadu_si128, _mm_cmpeq_epi8, _mm_movemask_epi8

class KVStoreV3{
private:
    // ─── CONSTANTS ───
    static constexpr uint8_t EMPTY_BYTE   = 0xFF;  // 11111111 — slot is empty
    static constexpr uint8_t DELETED_BYTE = 0xFE;  // 11111110 — slot was deleted
    // Values 0x00 - 0xFD (0 - 253) are valid H2 hints

    static constexpr size_t GROUP_SIZE = 16;  // We process 16 slots at a time (SSE2 = 128 bits = 16 bytes)
    
    static constexpr double MAX_LOAD_FACTOR = 0.875;  // 87.5% — Swiss tables can go higher because probing is fast

    // ─── SLOT STRUCTURE ───
    // We separate metadata from data (Structure of Arrays, not Array of Structures)
    // This is critical: metadata is very compact (1 byte per slot)
    // So 16 metadata bytes fit in 1 cache line = 1 SIMD load
    
    struct Slot {
        std::string key;
        std::string value;
    };

    // ─── DATA MEMBERS ───
    // Metadata array: 1 byte per slot, contiguous
    // This is what makes SIMD possible — we can load 16 bytes at once
    std::vector<uint8_t> metadata_;
    
    // Data array: key-value pairs
    std::vector<Slot> slots_;
    
    size_t num_occupied_ = 0;  // Number of keys stored
    size_t capacity_;           // Total number of slots (always power of 2)

    // ─── HASH FUNCTION ───
    // FNV-1a for now (we'll upgrade to wyhash later)
    uint64_t hash_fn(const std::string& key) const{
        uint64_t h = 14695981039346656037ULL;
        for(char c : key){
            h ^= static_cast<uint64_t>(static_cast<uint8_t>(c));
            h *= 1099511628211ULL;
        }
        return h;
    }

    // ─── SPLIT HASH INTO H1 AND H2 ───
    // H1 = top bits → used to find which GROUP to start probing at
    // H2 = bottom 7 bits → stored in metadata byte for SIMD matching
    //
    // hash is 64 bits
    // H2 = hash & 0x7F (lower 7 bits, values 0-127, all valid metadata values)
    // H1 = hash >> 7 (remaining 57 bits, used for group index)
    
    uint8_t get_h2(uint64_t h) const{
        return static_cast<uint8_t>(h & 0x7F);  // Lower 7 bits
    }

    size_t get_h1(uint64_t h) const{
        return h >> 7;  // Upper 57 bits
    }

    // ─── SIMD GROUP PROBING ───
    // This is the magic. We check 16 metadata bytes simultaneously.
    // Returns a bitmask: bit i is 1 if metadata[group_start + i] matches h2
    
    uint32_t match_in_group(size_t group_start, uint8_t h2) const{
        // Load 16 metadata bytes starting at group_start
        // _mm_loadu_si128 loads 16 bytes from memory into a 128-bit SIMD register
        // "u" means "unaligned" — the address doesn't need to be 16-byte aligned
        __m128i ctrl = _mm_loadu_si128(
            reinterpret_cast<const __m128i*>(metadata_.data() + group_start)
        );
        
        // Create a SIMD register with h2 repeated 16 times
        // _mm_set1_epi8 fills all 16 lanes of the 128-bit register with the same byte
        __m128i target = _mm_set1_epi8(static_cast<char>(h2));
        
        // Compare all 16 bytes simultaneously
        // _mm_cmpeq_epi8: for each of the 16 byte positions,
        //   if ctrl[i] == target[i], the result lane gets 0xFF (all 1s)
        //   if ctrl[i] != target[i], the result lane gets 0x00 (all 0s)
        __m128i cmp = _mm_cmpeq_epi8(ctrl, target);
        
        // Extract the comparison result as a 16-bit bitmask
        // _mm_movemask_epi8: takes the highest bit of each of the 16 result bytes
        //   and packs them into a 16-bit integer
        //   bit i = 1 means ctrl[i] == target[i]
        uint32_t mask = _mm_movemask_epi8(cmp);
        
        return mask;
    }

    // Also check if the group has any EMPTY slots (so we know when to stop probing)
    uint32_t match_empty_in_group(size_t group_start) const{
        __m128i ctrl = _mm_loadu_si128(
            reinterpret_cast<const __m128i*>(metadata_.data() + group_start)
        );
        __m128i empty = _mm_set1_epi8(static_cast<char>(EMPTY_BYTE));
        __m128i cmp = _mm_cmpeq_epi8(ctrl, empty);
        uint32_t mask = _mm_movemask_epi8(cmp);
        return mask;
    }

    // ─── FIND SLOT ───
    // Returns the index of the key if found, or the best empty/deleted slot for insertion
    // found is set to true if the key exists, false otherwise
    
    struct FindResult{
        size_t index;    // Where the key is (or where to insert)
        bool found;      // Whether the key was found
    };

    FindResult find_slot(const std::string& key) const{
        uint64_t h = hash_fn(key);
        uint8_t h2 = get_h2(h);
        size_t h1 = get_h1(h);
        
        // Starting group index
        // & (capacity_ - 1) because capacity_ is a power of 2
        // This maps h1 to a valid group index
        size_t group_idx = (h1 * GROUP_SIZE) & (capacity_ - 1);
        // Note: groups are 16 slots wide, so group_idx is the start of a group
        // Actually, we need to be more careful. group_idx should point to the
        // start of a group (aligned to GROUP_SIZE within the capacity).
        // For simplicity, let's just use the hash directly:
        group_idx = h1 & (capacity_ - 1);
        // Then round down to group boundary:
        // Actually for linear probing, we just start at the slot index and probe forward.
        // But for SIMD, we process 16 slots at a time, so we start at a group.
        
        // Let me simplify: the starting position is based on h1, 
        // and we probe group-by-group (16 slots at a time)
        size_t probe_idx = h1 & (capacity_ - 1);
        
        size_t first_available = SIZE_MAX;  // First EMPTY or DELETED slot for insertion
        
        for(size_t probes = 0; probes < capacity_; probes += GROUP_SIZE){
            size_t group_start = probe_idx & (capacity_ - 1);
            
            // SIMD: check which slots in this group have matching H2
            uint32_t match_mask = match_in_group(group_start, h2);
            
            // Iterate over matching slots (bits that are 1 in match_mask)
            while(match_mask != 0){
                // __builtin_ctz finds the position of the lowest set bit
                // This tells us which slot in the group matched
                // On MSVC, use _BitScanForward instead
                unsigned long slot_offset;
                #ifdef _MSC_VER
                    _BitScanForward(&slot_offset, match_mask);
                #else
                    slot_offset = __builtin_ctz(match_mask);
                #endif
                
                size_t slot_idx = group_start + slot_offset;
                if(slot_idx >= capacity_) slot_idx -= capacity_;
                
                // H2 matched! Now check if the FULL key matches
                if(slots_[slot_idx].key == key){
                    return {slot_idx, true};  // FOUND!
                }
                
                // H2 matched but key didn't — hash collision at the H2 level
                // Clear this bit and check the next matching slot
                match_mask &= match_mask - 1;  // Clear lowest set bit
            }
            
            // Check for EMPTY slots in this group (means key can't be further ahead)
            uint32_t empty_mask = match_empty_in_group(group_start);
            
            if(empty_mask != 0){
                // There's at least one EMPTY slot in this group
                // The key is NOT in the table (if it were, it would have been placed
                // in this group or an earlier group, not past an EMPTY slot)
                
                if(first_available == SIZE_MAX){
                    // Remember the first EMPTY slot for insertion
                    unsigned long offset;
                    #ifdef _MSC_VER
                        _BitScanForward(&offset, empty_mask);
                    #else
                        offset = __builtin_ctz(empty_mask);
                    #endif
                    first_available = group_start + offset;
                    if (first_available >= capacity_) first_available -= capacity_;
                }
                
                return {first_available, false};  // Not found, but we know where to insert
            }
            
            // Also look for DELETED slots (for insertion reuse)
            if(first_available == SIZE_MAX){
                __m128i ctrl = _mm_loadu_si128(
                    reinterpret_cast<const __m128i*>(metadata_.data() + group_start)
                );
                __m128i del = _mm_set1_epi8(static_cast<char>(DELETED_BYTE));
                __m128i cmp = _mm_cmpeq_epi8(ctrl, del);
                uint32_t del_mask = _mm_movemask_epi8(cmp);
                
                if(del_mask != 0){
                    unsigned long offset;
                    #ifdef _MSC_VER
                        _BitScanForward(&offset, del_mask);
                    #else
                        offset = __builtin_ctz(del_mask);
                    #endif
                    first_available = group_start + offset;
                    if (first_available >= capacity_) first_available -= capacity_;
                }
            }
            
            // Move to the next group
            probe_idx += GROUP_SIZE;
        }
        
        // Should never reach here if load factor is controlled
        return {first_available, false};
    }

    // ─── RESIZE ───
    void resize(){
        size_t old_capacity = capacity_;
        std::vector<uint8_t> old_metadata = std::move(metadata_);
        std::vector<Slot> old_slots = std::move(slots_);

        capacity_ = old_capacity * 2;
        metadata_.resize(capacity_, EMPTY_BYTE);  // All new slots are EMPTY
        slots_.resize(capacity_);

        num_occupied_ = 0;
        size_t num_tombstones = 0;

        // Re-insert all occupied keys
        for(size_t i = 0; i < old_capacity; i++){
            if (old_metadata[i] != EMPTY_BYTE && old_metadata[i] != DELETED_BYTE) {
                // This slot has a valid key — re-insert into new table
                set(std::move(old_slots[i].key), std::move(old_slots[i].value));
            }
        }
    }

public:
    // ─── CONSTRUCTOR ───
    explicit KVStoreV3(size_t initial_capacity = 128){
        capacity_ = 1;
        while(capacity_ < initial_capacity){
            capacity_ *= 2;
        }
        // Capacity must be at least GROUP_SIZE (16)
        if (capacity_ < GROUP_SIZE) capacity_ = GROUP_SIZE;
        
        metadata_.resize(capacity_, EMPTY_BYTE);
        slots_.resize(capacity_);
    }

    // ─── SET ───
    void set(const std::string& key, const std::string& value){
        if(num_occupied_ >= static_cast<size_t>(MAX_LOAD_FACTOR * capacity_)){
            resize();
        }

        FindResult result = find_slot(key);

        if(result.found){
            // Update existing value
            slots_[result.index].value = value;
        }else{
            // Insert new key
            uint64_t h = hash_fn(key);
            uint8_t h2 = get_h2(h);
            
            metadata_[result.index] = h2;            // Store the H2 hint
            slots_[result.index].key = key;
            slots_[result.index].value = value;
            num_occupied_++;
        }
    }

    // Also support move semantics for efficiency
    void set(std::string&& key, std::string&& value){
        if(num_occupied_ >= static_cast<size_t>(MAX_LOAD_FACTOR * capacity_)){
            resize();
        }

        FindResult result = find_slot(key);

        if(result.found){
            slots_[result.index].value = std::move(value);
        }else{
            uint64_t h = hash_fn(key);
            uint8_t h2 = get_h2(h);
            
            metadata_[result.index] = h2;
            slots_[result.index].key = std::move(key);
            slots_[result.index].value = std::move(value);
            num_occupied_++;
        }
    }

    // ─── GET ───
    std::optional<std::string> get(const std::string& key) const{
        FindResult result = find_slot(key);
        if(result.found){
            return slots_[result.index].value;
        }
        return std::nullopt;
    }

    // ─── DELETE ───
    bool delete_key(const std::string& key){
        FindResult result = find_slot(key);
        if(result.found){
            metadata_[result.index] = DELETED_BYTE;
            // Don't clear key/value — they'll be overwritten when this slot is reused
            num_occupied_--;
            return true;
        }
        return false;
    }

    // ─── EXISTS ───
    bool exists(const std::string& key) const{
        return find_slot(key).found;
    }

    // ─── COUNT ───
    size_t count() const { return num_occupied_; }

    // ─── CLEAR ───
    void clear(){
        std::fill(metadata_.begin(), metadata_.end(), EMPTY_BYTE);
        num_occupied_ = 0;
    }
};

#ifndef BENCHMARK_MODE
int main(){
    KVStoreV3 store(128);

    std::cout << "=== Basic Operations ===" << std::endl;

    // SET
    store.set("Alice", "555-1234");
    store.set("Bob", "555-5678");
    store.set("Charlie", "555-9012");
    store.set("David", "555-3333");
    store.set("Eve", "555-7777");

    // GET
    auto r1 = store.get("Alice");
    std::cout << "GET Alice: " << (r1 ? *r1 : "NOT FOUND") << std::endl;

    auto r2 = store.get("Charlie");
    std::cout << "GET Charlie: " << (r2 ? *r2 : "NOT FOUND") << std::endl;

    auto r3 = store.get("Unknown");
    std::cout << "GET Unknown: " << (r3 ? *r3 : "NOT FOUND") << std::endl;

    // UPDATE
    store.set("Alice", "555-9999");
    auto r4 = store.get("Alice");
    std::cout << "GET Alice after update: " << (r4 ? *r4 : "NOT FOUND") << std::endl;

    // DELETE
    store.delete_key("Bob");
    auto r5 = store.get("Bob");
    std::cout << "GET Bob after delete: " << (r5 ? *r5 : "NOT FOUND") << std::endl;

    // Verify Charlie still works (probing past deleted slot)
    auto r6 = store.get("Charlie");
    std::cout << "GET Charlie: " << (r6 ? *r6 : "NOT FOUND") << std::endl;

    std::cout << "Count: " << store.count() << std::endl;

    // INSERT after delete (should reuse DELETED slot)
    store.set("Frank", "555-4444");
    auto r7 = store.get("Frank");
    std::cout << "GET Frank: " << (r7 ? *r7 : "NOT FOUND") << std::endl;

    // ─── LARGE SCALE TEST ───
    std::cout << "\n=== Large Scale Test ===" << std::endl;
    
    for(int i = 0; i < 100000; i++){
        store.set("key_" + std::to_string(i), "val_" + std::to_string(i));
    }
    
    std::cout << "Count after 100K inserts: " << store.count() << std::endl;
    
    // Verify random keys
    auto r8 = store.get("key_50000");
    std::cout << "GET key_50000: " << (r8 ? *r8 : "NOT FOUND") << std::endl;
    
    // Verify old key still works after many inserts and resizes
    auto r9 = store.get("Alice");
    std::cout << "GET Alice: " << (r9 ? *r9 : "NOT FOUND") << std::endl;
    
    // Delete many keys and verify
    for(int i = 0; i < 50000; i++){
        store.delete_key("key_" + std::to_string(i));
    }
    std::cout << "Count after 50K deletes: " << store.count() << std::endl;
    
    auto r10 = store.get("key_60000");
    std::cout << "GET key_60000: " << (r10 ? *r10 : "NOT FOUND") << std::endl;

}
#endif