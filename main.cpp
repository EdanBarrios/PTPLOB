#include "OrderBook.h"
#include <chrono>
#include <iostream>

int main() {
    OrderBook mgcBook;

    // Simulazione di un "Break of Structure" (BOS)
    // 1. Aggiungi alcuni ordini "Limit Sell" (Asks)
    // 2. Aggiungi un ordine "Market Buy" (Buy con prezzo alto)
    // 3. Verifica che match() funzioni correttamente
    auto start = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < 1000; ++i) {
        mgcBook.addOrder({(uint64_t)i, 2045.00, 1, Side::Buy});
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);

    std::cout << "Order Latency: " << duration.count() << " ns" << std::endl;
}