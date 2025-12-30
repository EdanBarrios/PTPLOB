#ifndef ORDER_H
#define ORDER_H

#include <cstdint>
#include <iostream>

enum class Side { Buy, Sell };

struct Order {
    uint64_t id;
    double price;
    uint32_t quantity;
    Side side;
};

// TODO: Add a timestamp for time priority (nanoseconds)
#endif