#include "OrderBook.h"
#include <chrono>
#include <iostream>

int main() {
    OrderBook mgcBook;
    const int iterations = 1000;

    // WARMUP: Get the CPU out of power-saving mode and prime the cache
    for(int i = 0; i < 100; ++i) {
        mgcBook.addOrder({9999, 2045.00, 1, Side::Buy});
    }

    auto start = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < iterations; ++i) {
        // Use unique IDs to ensure the unordered_map actually has to work
        mgcBook.addOrder({(uint64_t)i, 2045.00, 1, Side::Buy});
    }
    auto end = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    std::cout << "Total Latency: " << duration.count() << " ns" << std::endl;
    std::cout << "Avg Latency per Order: " << duration.count() / iterations << " ns" << std::endl;
}