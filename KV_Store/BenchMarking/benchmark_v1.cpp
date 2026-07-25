// benchmark_v1.cpp
// Simple benchmark to compare our hash maps

#define BENCHMARK_MODE

#include <iostream>
#include <string>
#include <chrono>
#include <random>
#include <vector>
#include <algorithm>

// Include our hash map implementations
// (Copy the HashMapV2 and HashMapV3 class definitions here,
//  or include them from header files)

#include "../V2/kv_store_v2.cpp" // Basic open-addressing
#include "../V3/kv_store_v3.cpp"  // SIMD Swiss table

// ─── TIMER ───
class Timer {
    std::chrono::high_resolution_clock::time_point start_;
public:
    Timer() : start_(std::chrono::high_resolution_clock::now()) {}
    
    double elapsed_ns(){
        auto end = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<double, std::nano>(end - start_).count();
    }
    
    double elapsed_us(){
        return elapsed_ns() / 1000.0;
    }
    
    double elapsed_ms(){
        return elapsed_ns() / 1000000.0;
    }
};

// ─── GENERATE RANDOM KEYS ───
std::vector<std::string> generate_keys(size_t count){
    std::vector<std::string> keys;
    keys.reserve(count);
    
    std::mt19937 rng(42);  // Fixed seed for reproducibility
    for (size_t i = 0; i < count; i++) {
        keys.push_back("key_" + std::to_string(rng()));
    }
    return keys;
}

// ─── BENCHMARK A HASH MAP ───
template<typename HashMap>
void benchmark(const std::string& name, size_t num_keys, size_t num_ops) {
    HashMap map(num_keys * 2);  // Pre-allocate enough capacity to avoid resizes during benchmark
    
    std::vector<std::string> keys = generate_keys(num_keys);
    std::vector<std::string> lookup_keys = generate_keys(num_ops);
    
    // Mix in some existing keys for lookups (50% exist, 50% don't)
    for (size_t i = 0; i < num_ops / 2; i++) {
        lookup_keys[i] = keys[i % num_keys];  // These keys exist
    }
    
    // ─── BENCHMARK: INSERTS ───
    Timer t1;
    for (size_t i = 0; i < num_keys; i++) {
        map.set(keys[i], "value");
    }
    double insert_time = t1.elapsed_ns();
    double insert_per_op = insert_time / num_keys;
    
    // ─── BENCHMARK: LOOKUPS ───
    size_t found_count = 0;
    Timer t2;
    for (size_t i = 0; i < num_ops; i++) {
        auto result = map.get(lookup_keys[i]);
        if (result) found_count++;
    }
    double lookup_time = t2.elapsed_ns();
    double lookup_per_op = lookup_time / num_ops;
    
    // ─── BENCHMARK: DELETES ───
    Timer t3;
    for (size_t i = 0; i < num_keys / 2; i++) {
        map.delete_key(keys[i]);
    }
    double delete_time = t3.elapsed_ns();
    double delete_per_op = delete_time / (num_keys / 2);
    
    // ─── RESULTS ───
    std::cout << "╔══════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║  " << name << std::endl;
    std::cout << "║  Keys: " << num_keys << "  Ops: " << num_ops << std::endl;
    std::cout << "╠══════════════════════════════════════════════════╣" << std::endl;
    std::cout << "║  INSERT:  " << insert_per_op << " ns/op" 
              << "  (" << (num_keys / (insert_time / 1e9)) << " ops/sec)" << std::endl;
    std::cout << "║  GET:     " << lookup_per_op << " ns/op"
              << "  (" << (num_ops / (lookup_time / 1e9)) << " ops/sec)" << std::endl;
    std::cout << "║  DELETE:  " << delete_per_op << " ns/op" << std::endl;
    std::cout << "║  Found:   " << found_count << " / " << num_ops << std::endl;
    std::cout << "╚══════════════════════════════════════════════════╝" << std::endl;
}

int main() {
    constexpr size_t NUM_KEYS = 100000;
    constexpr size_t NUM_OPS = 1000000;
    
    std::cout << "Comparing HashMap versions..." << std::endl;
    std::cout << std::endl;
    
    // Note: std::unordered_map benchmark would require separate code
    // since it has a different API. We'll focus on our two versions.
    
    benchmark<KVStoreV2>("HashMapV2 — Basic Open Addressing", NUM_KEYS, NUM_OPS);
    benchmark<KVStoreV3>("HashMapV3 — SIMD Swiss Table",       NUM_KEYS, NUM_OPS);
    
    return 0;
}