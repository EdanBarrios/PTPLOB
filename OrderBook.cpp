#include "OrderBook.h"
#include <iomanip>

void OrderBook::addOrder(Order order) {
    if (order.side == Side::Buy) {
        // This map uses std::greater
        LimitLevel& level = bids[order.price];
        level.price = order.price;
        level.totalVolume += order.quantity;
        level.orders.push_back(order);
        
        // Save the iterator and location
        orderLookup[order.id] = std::prev(level.orders.end());
        locationLookup[order.id] = {order.price, Side::Buy};
    } 
    else {
        // This map uses std::less (default)
        LimitLevel& level = asks[order.price];
        level.price = order.price;
        level.totalVolume += order.quantity;
        level.orders.push_back(order);
        
        // Save the iterator and location
        orderLookup[order.id] = std::prev(level.orders.end());
        locationLookup[order.id] = {order.price, Side::Sell};
    }
    match();
}

void OrderBook::match() {
    // TODO: Implementare la logica del "Break of Structure"
    while (!bids.empty() && !asks.empty()) {
        auto bestBidIt = bids.begin(); // Highest Buy
        auto bestAskIt = asks.begin(); // Lowest Sell

        // Check for crossed book
        if (bestBidIt->first < bestAskIt->first) break;

        auto& bidOrders = bestBidIt->second.orders;
        auto& askOrders = bestAskIt->second.orders;

        while (!bidOrders.empty() && !askOrders.empty()) {
            Order& bid = bidOrders.front();
            Order& ask = askOrders.front();

            uint32_t matchQty = std::min(bid.quantity, ask.quantity);

            // LOG TRADE: In production, this would trigger a callback to a Gateway
            //std::cout << "[FILL] Price: " << ask.price << " | Qty: " << matchQty << std::endl;

            bid.quantity -= matchQty;
            ask.quantity -= matchQty;
            bestBidIt->second.totalVolume -= matchQty;
            bestAskIt->second.totalVolume -= matchQty;

            if (bid.quantity == 0) bidOrders.pop_front();
            if (ask.quantity == 0) askOrders.pop_front();
        }

        // Remove empty price levels to keep map search space lean
        if (bidOrders.empty()) bids.erase(bestBidIt);
        if (askOrders.empty()) asks.erase(bestAskIt);
    }
}

void OrderBook::cancelOrder(uint64_t orderId) {
    if (orderLookup.find(orderId) == orderLookup.end()) return;

    auto it = orderLookup[orderId];
    auto loc = locationLookup[orderId];

    if (loc.side == Side::Buy) {
        bids[loc.price].totalVolume -= it->quantity;
        bids[loc.price].orders.erase(it);
        if (bids[loc.price].orders.empty()) bids.erase(loc.price);
    } else {
        asks[loc.price].totalVolume -= it->quantity;
        asks[loc.price].orders.erase(it);
        if (asks[loc.price].orders.empty()) asks.erase(loc.price);
    }

    orderLookup.erase(orderId);
    locationLookup.erase(orderId);
}

void OrderBook::display() {
    std::cout << "\n===========================================" << std::endl;
    std::cout << "   ASKS (Sellers)    |    BIDS (Buyers)    " << std::endl;
    std::cout << "  Price      Qty     |     Price     Qty    " << std::endl;
    std::cout << "-------------------------------------------" << std::endl;

    // Print Asks (High to Low so the lowest ask is near the spread)
    for (auto it = asks.rbegin(); it != asks.rend(); ++it) {
        std::cout << std::fixed << std::setprecision(2) 
                  << std::setw(8) << it->first << " | " << std::setw(5) << it->second.totalVolume 
                  << "    |                   " << std::endl;
    }

    std::cout << "------------------SPREAD-------------------" << std::endl;

    // Print Bids (High to Low)
    for (auto const& [price, level] : bids) {
        std::cout << "                     |  " << std::fixed << std::setprecision(2) 
                  << std::setw(8) << price << " | " << std::setw(5) << level.totalVolume << std::endl;
    }
    std::cout << "===========================================\n" << std::endl;
}