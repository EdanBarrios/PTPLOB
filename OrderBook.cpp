#include "OrderBook.h"

void OrderBook::addOrder(Order order) {
    if (order.side == Side::Buy) {
        bids[order.price].price = order.price;
        bids[order.price].orders.push_back(order);
        bids[order.price].totalVolume += order.quantity;

        std::cout << "[ADD BUY] Price: " << order.price << " Qty: " << order.quantity << std::endl;
    } else {
        asks[order.price].price = order.price;
        asks[order.price].orders.push_back(order);
        asks[order.price].totalVolume += order.quantity;

        std::cout << "[ADD SELL] Price: " << order.price << " Qty: " << order.quantity << std::endl;
    }
    match();
}

void OrderBook::match() {
    // TODO: Implementare la logica del "Break of Structure"
    // Finché (best bid >= best ask), esegui scambi (trades)
}

void OrderBook::display() {
    std::cout << "\n--- ORDER BOOK (Microgold) ---\n";
    // TODO: Stampare i livelli di prezzo
}