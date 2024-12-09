#include <iostream>
#include <fstream>
#include <string>

#include "parser.h"
#include "matching_engine.h"
#include "order_book.h"

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cout << "Usage: " << argv[0] << " <input_file> <output_file>" << std::endl;
        return 1;
    }
    
    std::string inputFilename = argv[1];
    std::string outputFilename = argv[2];

    std::ifstream inputFile(inputFilename);
    if (!inputFile) {
        std::cerr << "Error: could not open file " << inputFilename << std::endl;
        return 1;
    }

    std::ofstream outputFile(outputFilename);
    if (!outputFile) {
        std::cerr << "Error: could not open output file " << outputFilename << std::endl;
        return 1;
    }

    // Pass the output file stream to the OrderBook
    OrderBook orderBook(outputFile);
    MatchingEngine matchingEngine(orderBook);
    Parser parser;

    matchingEngine.start();

    std::string line;
    while (std::getline(inputFile, line)) {
        auto order = parser.parseLine(line);
        if (order.has_value() && order->id != -1) {
            matchingEngine.addOrder(*order);
        } else {
            std::cerr << "Error: could not parse line: " << line << std::endl;
        }
    }

    matchingEngine.stop();

    std::cout << "Trades have been recorded in: " << outputFilename << std::endl;

    return 0;
}
