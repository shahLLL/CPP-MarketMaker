#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>
#include <algorithm>
#include "headers/types.hpp"
#include "headers/itch-parser.hpp"
#include "headers/spsc-queue.hpp"
#include "headers/stock-map.hpp"

// Constant Expressions
inline constexpr Double TEN_EXP_NEGATIVE_NINE = 1e-9;
inline constexpr Double TEN_EXP_THREE = 1e3;
inline constexpr Double TEN_EXP_SIX = 1e6;
inline constexpr Double POINT_FIVE = 0.5;
inline constexpr Double POINT_SEVEN_FIVE = 0.75;
inline constexpr Double POINT_NINE = 0.9;
inline constexpr Double POINT_NINE_NINE = 0.99;
inline constexpr Double POINT_NINE_NINE_NINE = 0.999;
inline constexpr SizeT EXPECTED_NUMBER_OF_ENTRIES = 12012;

void displayResults(const LatencyVector& latencies, const SizeT numberOfEntries, const Double elapsedTime, const Double throughput) noexcept {
    std::cout << "============================================" << std::endl;
    std::cout << "  Itch Market Replay Benchmark  (" << numberOfEntries << " ops)" << std::endl;
    std::cout << "============================================" << std::endl;
    std::cout << "  Elapsed:     " << elapsedTime * TEN_EXP_THREE << " ms" << std::endl;
    std::cout << "  Throughput:  " << throughput / TEN_EXP_SIX   << " M ops/sec" << std::endl;
    std::cout << "--------------------------------------------" << std::endl;
    std::cout << "  Latency (per op, including chrono overhead ~10 ns):" << std::endl;
    std::cout << "    p50  : " << latencies[numberOfEntries * POINT_FIVE]  << " ns" << std::endl;
    std::cout << "    p75  : " << latencies[numberOfEntries * POINT_SEVEN_FIVE]  << " ns" << std::endl;
    std::cout << "    p90  : " << latencies[numberOfEntries * POINT_NINE]  << " ns" << std::endl;
    std::cout << "    p99  : " << latencies[numberOfEntries * POINT_NINE_NINE]  << " ns" << std::endl;
    std::cout << "    p99.9: " << latencies[numberOfEntries * POINT_NINE_NINE_NINE] << " ns" << std::endl;
    std::cout << "============================================" << std::endl;
}

void parseFile(char* filePath, SizeT& numberOfEntries, TimePointVector& startTimes, TimePointVector& endTimes) {
    ITCHParser itchParser = ITCHParser(filePath);
    StockMap stockMap = StockMap();
    SPSCQueue<1> spscQueue = SPSCQueue<1>(); 
    std::atomic<bool> done{false};
    TimePoint startTime;
    TimePoint endTime;
    Bool isOrderBookMessageProd;
    Bool isOrderBookMessageCons;

    std::thread producerThread([&] {
        while(itchParser.hasNext()) {
            if(spscQueue.enqueue(itchParser, &startTime, &isOrderBookMessageProd)) { 
                if(isOrderBookMessageProd) startTimes.push_back(startTime);
                itchParser.increment(); 
            }
            else { std::this_thread::yield(); }
        }
        done.store(true, std::memory_order_release);
    });

    std::thread consumerThread([&] {
        while((!done.load(std::memory_order_acquire))) {
            if(spscQueue.dequeue(stockMap, &endTime, &isOrderBookMessageCons)) {
                if(isOrderBookMessageCons) {
                    endTimes.push_back(endTime);
                    numberOfEntries = numberOfEntries + 1;
                }
            }
        }
        while(spscQueue.dequeue(stockMap, &endTime, &isOrderBookMessageCons)) {
            if(isOrderBookMessageCons) {
                endTimes.push_back(endTime);
                numberOfEntries = numberOfEntries + 1;
            }
        }
    });

    producerThread.join();
    consumerThread.join();
}

const LatencyVector getLatencies(TimePointVector& startTimes, TimePointVector& endTimes, SizeT numberOfEntries) noexcept {
    LatencyVector latencies;
    latencies.reserve(numberOfEntries);

    for(int i = 0; i < numberOfEntries; i++) {
        latencies.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(endTimes[i] - startTimes[i]).count());
    }

    std::sort(latencies.begin(), latencies.end());
    return latencies;
}

int main(int argc, char* argv[]) {
    if(argc != 2) {
        std::cout << "Usage: bench <inputfile>" << std::endl;
        return 1;
    }

    TimePointVector startTimes;
    startTimes.reserve(EXPECTED_NUMBER_OF_ENTRIES);
    TimePointVector endTimes;
    endTimes.reserve(EXPECTED_NUMBER_OF_ENTRIES);
    SizeT numberOfEntries = 0;

    const auto wallStart = std::chrono::high_resolution_clock::now();
    parseFile(argv[1], numberOfEntries, startTimes, endTimes);
    const auto wallEnd = std::chrono::high_resolution_clock::now();

    std::cout << startTimes.size() << std::endl;
    std::cout << endTimes.size() << std::endl;
    std::cout << numberOfEntries << std::endl;
    if(startTimes.size() != numberOfEntries) { throw std::runtime_error("START TIMES NOT SAME AS NUMBER OF ENTRIES"); }
    if(endTimes.size() != numberOfEntries) { throw std::runtime_error("END TIMES NOT SAME AS NUMBER OF ENTRIES"); }

    const Double elapsedTime = std::chrono::duration_cast<std::chrono::nanoseconds>(wallEnd - 
        wallStart).count() * TEN_EXP_NEGATIVE_NINE;
    const Double throughput = numberOfEntries / elapsedTime;
    const LatencyVector latencies = getLatencies(startTimes, endTimes, numberOfEntries);
    displayResults(latencies, numberOfEntries, elapsedTime, throughput);

    return 0;
}