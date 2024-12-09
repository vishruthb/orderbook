#pragma once
#include <iostream>
#include "order.h"
#include <map>
#include <vector>
#include <mutex>
#include <optional>
#include <ostream>

struct Trade {
    double price;
    int quantity;
    Order buyer;
    Order seller;
};

class OrderBook {
public:
    OrderBook(std::ostream &outStream = std::cout) : out(outStream) {}

    void addOrder(const Order &order);
    std::optional<Trade> matchOne();

private:
    std::map<double, std::vector<Order>, std::greater<double>> bids;
    std::map<double, std::vector<Order>> asks;
    std::mutex bookMutex;
    std::ostream &out;

    void removeEmptyLevels();
};
