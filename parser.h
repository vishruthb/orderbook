#pragma once
#include "order.h"
#include <string>
#include <optional>

class Parser {
public:
    std::optional<Order> parseLine(const std::string &line);
};
