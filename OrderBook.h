#include <iostream>
#include <map>
#include <list>
#include <unordered_map>

#include "Order.h"


struct LimitLevel {
    double price;
    uint32_t totalVolume;
    std::list<Order> orders; // Time priority: head of list is oldest
};

class OrderBook {
private:
    // Bids sorted descending (highest first), Asks sorted ascending (lowest first)
    std::map<double, LimitLevel, std::greater<double>> bids; 
    std::map<double, LimitLevel> asks;
    
    // Quick lookup for cancellations (Order ID -> Iterator to the order in the list)
    // This allows O(1) cancellations.
    // std::unordered_map<uint64_t, std::list<Order>::iterator> orderMap;

public:
    void addOrder(Order order);
    void match(); // The "Break of Structure" logic
    void cancelOrder(uint64_t orderId);
    void display();
};