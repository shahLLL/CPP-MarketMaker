#include <iostream>
#include "headers/types.hpp"
#include "headers/itch-parser.hpp"

int main(int argc, char* argv[]) {
    if(argc != 2) {
        std::cout << "Usage: mm <inputfile>" << std::endl;
        return 1;
    }

    std::cout << "CPP Market Maker" << std::endl;
    const char* filePath = "./data/itch12kSample.bin";
    ITCHParser itchParser = ITCHParser(filePath);
    ITCHMessage itchMessage = ITCHMessage{};
    while(itchParser.hasNext()) {
        itchParser.getNext(itchMessage);
        std::cout << itchMessage.messageType << std::endl;
        itchParser.increment();
    }
    return 0;
}