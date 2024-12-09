#include "matching_engine.h"

MatchingEngine::MatchingEngine(OrderBook &book) : orderBook(book), running(false) {}

MatchingEngine::~MatchingEngine() {
    if (running) {
        stop();
    }
}

void MatchingEngine::start() {
    running = true;
    workerThread = std::thread(&MatchingEngine::run, this);
}

void MatchingEngine::stop() {
    {
        std::lock_guard<std::mutex> lock(ordersMutex);
        running = false;
    }
    ordersCV.notify_all();
    if (workerThread.joinable()) {
        workerThread.join();
    }
}

void MatchingEngine::addOrder(Order order) {
    {
        std::lock_guard<std::mutex> lock(ordersMutex);
        incomingOrders.push(order);
    }
    ordersCV.notify_one();
}

void MatchingEngine::run() {
    while (true) {
        Order currentOrder;
        {
            std::unique_lock<std::mutex> lock(ordersMutex);
            ordersCV.wait(lock, [this]() {
                return !incomingOrders.empty() || !running;
            });

            if (!running && incomingOrders.empty()) 
                break;

            currentOrder = incomingOrders.front();
            incomingOrders.pop();
        }

        processOrder(currentOrder);
        matchOrders();
    }
}

void MatchingEngine::processOrder(const Order &order) {
    orderBook.addOrder(order);
}

void MatchingEngine::matchOrders() {
    while (true) {
        auto trade = orderBook.matchOne();
        if (!trade) break;
    }
}