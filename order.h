#pragma once
#include <string>

enum class OrderType {BUY, SELL};

struct Order {
    OrderType type;             // BUY or SELL
    std::string instrument;     // ticker symbol
    double price;               // price per unit of instrument
    int quantity;               // number of units to buy or sell
    int id;                     // unique id for the order

    Order(OrderType type, std::string instrument, double price, int quantity, int id) : 
        type(type), instrument(instrument), price(price), quantity(quantity), id(id) {}

    Order() : type(OrderType::BUY), instrument(""), price(0.0), quantity(0), id(0) {} // Default constructor
};