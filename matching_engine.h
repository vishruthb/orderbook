#pragma once
#include "order.h"
#include "order_book.h"
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <thread>
#include <optional>

class MatchingEngine {
public:
    MatchingEngine(OrderBook &book);
    ~MatchingEngine();                      // Destructor to join the thread

    void start();
    void stop();
    void addOrder(Order order);
    
private:
    OrderBook &orderBook;
    std::queue<Order> incomingOrders;
    std::mutex ordersMutex;
    std::condition_variable ordersCV;
    std::atomic<bool> running;              // Check if the matching engine is running
    std::thread workerThread;

    void run();
    void processOrder(const Order &order);
    void matchOrders();
};