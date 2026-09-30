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
    ByteContainer byteContainer{};
    while(itchParser.hasNext()) {
        itchParser.parseNext(byteContainer);
        std::cout << static_cast<Alpha>(byteContainer[0]) << std::endl;
        itchParser.increment();
    }
    return 0;
}