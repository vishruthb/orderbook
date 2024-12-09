#include "parser.h"
#include <sstream>

std::optional<Order> Parser::parseLine(const std::string &line) {
    std::istringstream iss(line);
    std::string typeStr, instr;
    double price;
    int quantity, id;

    if (!(iss >> typeStr >> instr >> price >> quantity >> id)) {
        return std::nullopt;
    }

    OrderType type;
    if (typeStr == "BUY") type = OrderType::BUY;
    else if (typeStr == "SELL") type = OrderType::SELL;
    else return std::nullopt;

    return Order(type, instr, price, quantity, id);
}
