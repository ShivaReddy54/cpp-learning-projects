// kv_store_v4.cpp
// Multi-threaded KV store with per-core sharding + spinlocks

#define BENCHMARK_MODE // because we are using V3 here

#include <iostream>
#include <string>
#include <optional>
#include <thread>
#include <vector>
#include <atomic>
#include <cstdint>

// Copy HashMapV3 class definition here (or include from header)
#include "../V3/kv_store_v3.cpp"

// ─── SPINLOCK ───
// A spinlock is the simplest kind of lock.
// When a thread tries to acquire it and it's already locked,
// the thread "spins" (loops) waiting for it to become unlocked.
// 
// Why spinlock instead of std::mutex?
// - std::mutex calls the OS kernel when there's contention (~1-5 microseconds)
// - Spinlock just loops in user-space (~10-50 nanoseconds if lock is held briefly)
// - For short critical sections (like our hash map operations), spinlock is much faster

class Spinlock{
private:
    std::atomic<bool> locked_{false};
    
public:
    void lock(){
        // Try to acquire the lock
        // exchange(true, acquire) atomically:
        //   - Returns the OLD value of locked_
        //   - Sets locked_ to true
        // If old value was false, we got the lock!
        // If old value was true, someone else holds it — spin (loop)
        
        while(locked_.exchange(true, std::memory_order_acquire)){
            // We didn't get the lock. Spin-wait.
            
            // _mm_pause() (or __builtin_ia32_pause on GCC) tells the CPU:
            // "I'm spinning, please give the other core a chance"
            // It's a hint that improves performance on hyperthreaded CPUs
            #ifdef _MSC_VER
                _mm_pause();  // Intrinsic for PAUSE instruction
            #else
                __builtin_ia32_pause();
            #endif
        }
        // We got the lock!
    }
    
    void unlock(){
        // Release the lock
        locked_.store(false, std::memory_order_release);
    }
    
    // RAII wrapper — automatically unlocks when scope ends
    // This prevents forgetting to unlock (which would cause deadlock)
    class Guard{
        Spinlock& lock_;
    public:
        explicit Guard(Spinlock& l) : lock_(l) { lock_.lock(); }
        ~Guard() { lock_.unlock(); }
    };
};

// ─── SHARD ───
// Each shard is an independent hash map with its own lock
// 
// IMPORTANT: alignas(64) ensures each Shard starts at a 64-byte boundary
// This prevents false sharing between shards on different CPU cores

struct alignas(64) Shard{
    KVStoreV3 map;           // The hash map for this shard
    Spinlock lock;           // Lock for this shard ONLY
    // Padding is automatic due to alignas(64) — the struct is padded to
    // at least 64 bytes, so it occupies its own cache line(s)
};

// ─── SHARDED KV STORE ───
class KVStoreV4 {
private:
    std::vector<std::unique_ptr<Shard>> shards_;
    size_t num_shards_;
    
    // Determine which shard a key belongs to
    // We use the top bits of the hash for shard selection
    // This is separate from the H1/H2 split used inside each shard's hash map
    size_t get_shard_index(const std::string& key) const{
        // Simple hash for shard selection
        uint64_t h = 0;
        for(char c : key){
            h = h * 31 + static_cast<uint64_t>(c);
        }
        // Use top bits for shard, so shard selection doesn't interfere
        // with the hash map's internal H1/H2 split
        return h % num_shards_;
    }

public:
    explicit KVStoreV4(size_t num_shards = 8, size_t per_shard_capacity = 1024)
        : num_shards_(num_shards)
    {
        // Number of shards should equal number of CPU cores for best performance
        shards_.resize(num_shards_);
        for(size_t i = 0; i < num_shards_; i++){
            // Each shard's hash map starts with per_shard_capacity
            // The Shard struct has alignas(64) — vector allocation
            // should respect this, but we should verify
        }
    }

    // ─── SET ───
    void set(const std::string& key, const std::string& value){
        size_t shard_idx = get_shard_index(key);
        
        // Lock ONLY this shard. Other shards remain unlocked.
        // Other threads can access other shards simultaneously!
        Spinlock::Guard guard(shards_[shard_idx].lock);
        
        shards_[shard_idx].map.set(key, value);
    }

    // ─── GET ───
    std::optional<std::string> get(const std::string& key) const{
        size_t shard_idx = get_shard_index(key);
        
        // const cast needed because Spinlock::lock() is not const
        // This is safe because we're protecting the shard's internal state
        Spinlock::Guard guard(const_cast<Spinlock&>(shards_[shard_idx].lock));
        
        return shards_[shard_idx].map.get(key);
    }

    // ─── DELETE ───
    bool delete_key(const std::string& key){
        size_t shard_idx = get_shard_index(key);
        Spinlock::Guard guard(shards_[shard_idx].lock);
        
        return shards_[shard_idx].map.delete_key(key);
    }

    // ─── COUNT (approximate — no lock) ───
    size_t count() const{
        size_t total = 0;
        for(size_t i = 0; i < num_shards_; i++){
            total += shards_[i].map.count();
        }
        return total;
    }
};

// ─── MULTI-THREADED BENCHMARK ───
void benchmark_multithreaded() {
    constexpr size_t NUM_THREADS = 8;
    constexpr size_t OPS_PER_THREAD = 1000000;
    constexpr size_t NUM_KEYS = 500000;
    
    KVStoreV4 store(NUM_THREADS, 65536);  // 8 shards
    
    // First, populate the store
    std::vector<std::string> keys;
    for(size_t i = 0; i < NUM_KEYS; i++){
        keys.push_back("key_" + std::to_string(i));
    }
    for (const auto& k : keys) {
        store.set(k, "value");
    }
    
    std::cout << "Initial count: " << store.count() << std::endl;
    
    // ─── CONCURRENT GET BENCHMARK ───
    auto start = std::chrono::high_resolution_clock::now();
    
    std::vector<std::thread> threads;
    for(size_t t = 0; t < NUM_THREADS; t++){
        threads.emplace_back([&, t]() {
            size_t found = 0;
            for (size_t i = 0; i < OPS_PER_THREAD; i++) {
                size_t key_idx = (t * OPS_PER_THREAD + i) % NUM_KEYS;
                auto result = store.get(keys[key_idx]);
                if (result) found++;
            }
            std::cout << "Thread " << t << " found: " << found << std::endl;
        });
    }
    
    for(auto& th : threads){
        th.join();
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    double elapsed_ns = std::chrono::duration<double, std::nano>(end - start).count();
    size_t total_ops = NUM_THREADS * OPS_PER_THREAD;
    
    std::cout << "\n=== Concurrent GET Results ===" << std::endl;
    std::cout << "Threads: " << NUM_THREADS << std::endl;
    std::cout << "Total ops: " << total_ops << std::endl;
    std::cout << "Time per GET: " << (elapsed_ns / total_ops) << " ns" << std::endl;
    std::cout << "Throughput: " << (total_ops / (elapsed_ns / 1e9)) << " ops/sec" << std::endl;
    
    // ─── CONCURRENT MIXED R/W BENCHMARK ───
    start = std::chrono::high_resolution_clock::now();
    
    threads.clear();
    for(size_t t = 0; t < NUM_THREADS; t++){
        threads.emplace_back([&, t]() {
            for(size_t i = 0; i < OPS_PER_THREAD; i++){
                size_t key_idx = (t * OPS_PER_THREAD + i) % NUM_KEYS;
                if(i % 10 == 0){
                    // 10% writes
                    store.set(keys[key_idx], "new_value_" + std::to_string(i));
                }else{
                    // 90% reads
                    store.get(keys[key_idx]);
                }
            }
        });
    }
    
    for(auto& th : threads){
        th.join();
    }
    
    end = std::chrono::high_resolution_clock::now();
    elapsed_ns = std::chrono::duration<double, std::nano>(end - start).count();
    
    std::cout << "\n=== Concurrent 90% Read / 10% Write Results ===" << std::endl;
    std::cout << "Time per op: " << (elapsed_ns / total_ops) << " ns" << std::endl;
    std::cout << "Throughput: " << (total_ops / (elapsed_ns / 1e9)) << " ops/sec" << std::endl;
    std::cout << "Final count: " << store.count() << std::endl;
}

int main(){
    // Basic functionality test
    KVStoreV4 store(4);
    
    store.set("Alice", "555-1234");
    store.set("Bob", "555-5678");
    
    std::cout << "GET Alice: " << (*store.get("Alice")) << std::endl;
    std::cout << "GET Bob: " << (*store.get("Bob")) << std::endl;
    
    store.delete_key("Alice");
    std::cout << "GET Alice after delete: " << (store.get("Alice").has_value() ? *store.get("Alice") : "NOT FOUND") << std::endl;
    
    std::cout << "Count: " << store.count() << std::endl;
    
    std::cout << "\n";
    
    // Multi-threaded benchmark
    benchmark_multithreaded();
}