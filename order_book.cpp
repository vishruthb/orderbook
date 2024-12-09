#include "order_book.h"
#include <algorithm> 
#include <iostream>

void OrderBook::addOrder(const Order &order) {
    std::lock_guard<std::mutex> lock(bookMutex);
    if (order.type == OrderType::BUY) {
        bids[order.price].push_back(order);
    } else {
        asks[order.price].push_back(order);
    }
}

std::optional<Trade> OrderBook::matchOne() {
    std::lock_guard<std::mutex> lock(bookMutex);
    if (bids.empty() || asks.empty()) {
        return std::nullopt;
    }

    auto bestBid = bids.begin();
    auto bestAsk = asks.begin();

    if (bestBid->first < bestAsk->first) {
        return std::nullopt;
    }

    auto &bidOrders = bestBid->second;
    auto &askOrders = bestAsk->second;

    auto &buyer = bidOrders.front();
    auto &seller = askOrders.front();

    int tradeQuantity = std::min(buyer.quantity, seller.quantity);
    double tradePrice = bestBid->first;

    buyer.quantity -= tradeQuantity;
    seller.quantity -= tradeQuantity;

    Trade t{tradePrice, tradeQuantity, buyer, seller};

    // Log the trade to the output stream
    out << "Trade executed: " << t.quantity
        << " units @ " << t.price
        << " (Buyer ID: " << t.buyer.id
        << ", Seller ID: " << t.seller.id << ")\n";

    if (buyer.quantity == 0) {
        bidOrders.erase(bidOrders.begin());
    }
    if (seller.quantity == 0) {
        askOrders.erase(askOrders.begin());
    }

    removeEmptyLevels();
    return t;
}

void OrderBook::removeEmptyLevels() {
    for (auto it = bids.begin(); it != bids.end();) {
        if (it->second.empty()) {
            it = bids.erase(it);
        } else {
            ++it;
        }
    }

    for (auto it = asks.begin(); it != asks.end();) {
        if (it->second.empty()) {
            it = asks.erase(it);
        } else {
            ++it;
        }
    }
}
