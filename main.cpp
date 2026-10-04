#include <iostream>
#include <thread>
#include <atomic>
#include "headers/types.hpp"
#include "headers/itch-parser.hpp"
#include "headers/spsc-queue.hpp"
#include "headers/stock-map.hpp"

void displayResults(StockMap& stockMap) noexcept {
    std::cout << "CPP Market Maker" << std::endl;
    for(auto& stockSymbol: stockMap.getPerSymbolData()) {
        std::cout << "----------------------------" << std::endl;
        std::cout << "STOCK: " << stockSymbol.symbol << std::endl;
        std::cout << "STOCK LOCATE: " << stockSymbol.stockLocate << std::endl;
        std::cout << "BEST BID: " << stockSymbol.bestBid << std::endl;
        std::cout << "BEST ASK: " << stockSymbol.bestAsk << std::endl;
        std::cout << "MIDPRICE: " << stockSymbol.midPrice << std::endl;
        std::cout << "----------------------------" << std::endl;
    }
}

void parseFile(ITCHParser& itchParser, StockMap& stockMap) {
    SPSCQueue<10> spscQueue = SPSCQueue<10>(); 
    std::atomic<bool> done{false};

    std::thread producerThread([&] {
        while(itchParser.hasNext()) {
            if(spscQueue.enqueue(itchParser)) { itchParser.increment(); }
            else { std::this_thread::yield(); }
        }
        done.store(true, std::memory_order_release);
    });

    std::thread consumerThread([&] {
        while((!done.load(std::memory_order_acquire))) {
            spscQueue.dequeue(stockMap);
        }
        while(spscQueue.dequeue(stockMap)) {}
    });

    producerThread.join();
    consumerThread.join();
}

int main(int argc, char* argv[]) {
    if(argc != 2) {
        std::cout << "Usage: mm <inputfile>" << std::endl;
        return 1;
    }
    
    ITCHParser itchParser = ITCHParser(argv[1]);
    StockMap stockMap = StockMap();
    parseFile(itchParser, stockMap);
    displayResults(stockMap);

    return 0;
}