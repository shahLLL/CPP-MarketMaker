#include <iostream>
#include <thread>
#include <atomic>
#include "headers/types.hpp"
#include "headers/itch-parser.hpp"
#include "headers/spsc-queue.hpp"

void processorFunction(const ByteContainer& byteContainer) noexcept {
    std::cout << static_cast<Alpha>(byteContainer[0]) << std::endl;
}

int main(int argc, char* argv[]) {
    if(argc != 2) {
        std::cout << "Usage: mm <inputfile>" << std::endl;
        return 1;
    }

    std::cout << "CPP Market Maker" << std::endl;
    const char* filePath = "./data/itch12kSample.bin";
    ITCHParser itchParser = ITCHParser(filePath);
    SPSCQueue<10> spscQueue = SPSCQueue<10>(); 
    ByteContainer byteContainer{};
    std::atomic<bool> done{false};

    std::thread prod([&] {
        while(itchParser.hasNext()) {
            if(spscQueue.enqueue(itchParser)) { itchParser.increment(); }
            else { std::this_thread::yield(); }
        }
        done.store(true, std::memory_order_release);
    });

    std::thread cons([&] {
        while((!done.load(std::memory_order_acquire))) {
            spscQueue.dequeue(processorFunction);
        }
        while(spscQueue.dequeue(processorFunction)) {}
    });

    prod.join();
    cons.join();
    return 0;
}