#include <catch2/catch_test_macros.hpp>
#include "../headers/itch-parser.hpp"

// Helper Functions
void directCompare(ByteContainer& byteContainer, Byte* bytePtr, SizeT head, SizeT tail) {
    for(int i = head; i <= tail; i++) { REQUIRE(byteContainer[i] == *(bytePtr + i)); }
}

void endianCompare(ByteContainer& byteContainer, Byte* bytePtr, SizeT head, SizeT tail) {
    SizeT end = tail;
    while(head <= end) {
        REQUIRE(byteContainer[head] == *(bytePtr + tail));
        head = head + 1;
        tail = tail - 1;
    }
}

// Test Cases
TEST_CASE("PARSER TESTCASE #1", "[getMessageType]") {
    REQUIRE(getMessageType('S') == 12);
    REQUIRE(getMessageType('R') == 39);
    REQUIRE(getMessageType('H') == 25);
    REQUIRE(getMessageType('Y') == 20);
    REQUIRE(getMessageType('L') == 26);
    REQUIRE(getMessageType('V') == 35);
    REQUIRE(getMessageType('W') == 12);
    REQUIRE(getMessageType('K') == 28);
    REQUIRE(getMessageType('J') == 35);
    REQUIRE(getMessageType('h') == 21);
    REQUIRE(getMessageType('A') == 36);
    REQUIRE(getMessageType('F') == 40);
    REQUIRE(getMessageType('E') == 31);
    REQUIRE(getMessageType('C') == 36);
    REQUIRE(getMessageType('X') == 23);
    REQUIRE(getMessageType('D') == 19);
    REQUIRE(getMessageType('U') == 35);
    REQUIRE(getMessageType('P') == 44);
    REQUIRE(getMessageType('Q') == 40);
    REQUIRE(getMessageType('B') == 19);
    REQUIRE(getMessageType('I') == 50);
    REQUIRE(getMessageType('O') == 48);

    REQUIRE(getMessageType('r') == NULL_MESSAGE_SIGNAL);
    REQUIRE(getMessageType('Z') == NULL_MESSAGE_SIGNAL);
    REQUIRE(getMessageType('3') == NULL_MESSAGE_SIGNAL);
    REQUIRE(getMessageType('?') == NULL_MESSAGE_SIGNAL);
}

TEST_CASE("PARSER TESTCASE #2", "[endianSwap]") {
    ByteContainer byteContainer1 = ByteContainer{};
    ByteContainer byteContainer2 = ByteContainer{};
    ByteContainer byteContainer3 = ByteContainer{};
    ByteContainer byteContainer4 = ByteContainer{};

    Byte test1[] = {Byte{0x00}, Byte{0x64}};
    Byte test2[] = {Byte{0x00}, Byte{0x2A}, Byte{0x03}, Byte{0xE9}};
    Byte test3[] = {Byte{0x52}, Byte{0x00}, Byte{0x64}, 
        Byte{0x01}, Byte{0xF4}, Byte{0x64}};
    Byte test4[] = {Byte{0x59}, Byte{0x43}, Byte{0x4E}, Byte{0x41},
        Byte{0x50}, Byte{0x4E}, Byte{0x4E}, Byte{0x31}};
    
    endianSwap(byteContainer1, test1, 0, 1);
    endianSwap(byteContainer2, test2, 0, 3);
    endianSwap(byteContainer3, test3, 0, 5);
    endianSwap(byteContainer4, test4, 0, 7);

    REQUIRE(byteContainer1[0] == Byte{0x64});
    REQUIRE(byteContainer1[1] == Byte{0x00});

    REQUIRE(byteContainer2[0] == Byte{0xE9});
    REQUIRE(byteContainer2[1] == Byte{0x03});
    REQUIRE(byteContainer2[2] == Byte{0x2A});
    REQUIRE(byteContainer2[3] == Byte{0x00});

    REQUIRE(byteContainer3[0] == Byte{0x64});
    REQUIRE(byteContainer3[1] == Byte{0xF4});
    REQUIRE(byteContainer3[2] == Byte{0x01});
    REQUIRE(byteContainer3[3] == Byte{0x64});
    REQUIRE(byteContainer3[4] == Byte{0x00});
    REQUIRE(byteContainer3[5] == Byte{0x52});

    REQUIRE(byteContainer4[0] == Byte{0x31});
    REQUIRE(byteContainer4[1] == Byte{0x4E});
    REQUIRE(byteContainer4[2] == Byte{0x4E});
    REQUIRE(byteContainer4[3] == Byte{0x50});
    REQUIRE(byteContainer4[4] == Byte{0x41});
    REQUIRE(byteContainer4[5] == Byte{0x4E});
    REQUIRE(byteContainer4[6] == Byte{0x43});
    REQUIRE(byteContainer4[7] == Byte{0x59});

}

TEST_CASE("PARSER TESTCASE #3", "[directCopy]") {
    ByteContainer byteContainer = ByteContainer{};
    Byte test[] = {
        Byte{0x53}, Byte{0x00}, 
        Byte{0x2A}, Byte{0x03}, 
        Byte{0xE9}, Byte{0x00}, 
        Byte{0x00}, Byte{0x00}
    };

    directCopy(byteContainer, test, 0, 7);
    REQUIRE(byteContainer[0] == Byte{0x53});
    REQUIRE(byteContainer[1] == Byte{0x00});
    REQUIRE(byteContainer[2] == Byte{0x2A});
    REQUIRE(byteContainer[3] == Byte{0x03});
    REQUIRE(byteContainer[4] == Byte{0xE9});
    REQUIRE(byteContainer[5] == Byte{0x00});
    REQUIRE(byteContainer[6] == Byte{0x00});
    REQUIRE(byteContainer[7] == Byte{0x00});
}

TEST_CASE("PARSER TESTCASE #4", "[parseSystemEventMessage]") {
    ByteContainer byteContainer = ByteContainer{};
    Byte test[] = {
        Byte{0x53},
        Byte{0x00}, Byte{0x2A},
        Byte{0x03}, Byte{0xE9},
        Byte{0x00}, Byte{0x00}, Byte{0x00},
        Byte{0x00}, Byte{0x01}, Byte{0xF4},
        Byte{0x4F}
    };
    parseSystemEventMessage(byteContainer, test);
    directCompare(byteContainer, test, 0, 0);
    endianCompare(byteContainer, test, 1, 2);
    endianCompare(byteContainer, test, 3, 4);
    endianCompare(byteContainer, test, 5, 10);
    directCompare(byteContainer, test, 11, 11);
}

TEST_CASE("PARSER TESTCASE #5", "[parseStockDirectory]") {
    ByteContainer byteContainer = ByteContainer{};
    Byte test[] = {
        Byte{0x52}, Byte{0x00}, Byte{0x64},
        Byte{0x01}, Byte{0xF4}, Byte{0x00}, 
        Byte{0x00}, Byte{0x00}, Byte{0x00}, 
        Byte{0x03}, Byte{0xE8}, Byte{0x41}, 
        Byte{0x41}, Byte{0x50}, Byte{0x4C}, 
        Byte{0x20}, Byte{0x20}, Byte{0x20}, 
        Byte{0x20}, Byte{0x51}, Byte{0x4E},
        Byte{0x00}, Byte{0x00}, Byte{0x00},
        Byte{0x64}, Byte{0x59}, Byte{0x43},
        Byte{0x4E}, Byte{0x41}, Byte{0x50},
        Byte{0x4E}, Byte{0x4E}, Byte{0x31},
        Byte{0x4E}, Byte{0x00}, Byte{0x00}, 
        Byte{0x00}, Byte{0x00}, Byte{0x4E}
    };

    parseStockDirectory(byteContainer, test);
    directCompare(byteContainer, test, 0, 0);
    endianCompare(byteContainer, test, 1, 2);
    endianCompare(byteContainer, test, 3, 4);
    endianCompare(byteContainer, test, 5, 10);
    directCompare(byteContainer, test, 11, 20);
    endianCompare(byteContainer, test, 21, 24);
    directCompare(byteContainer, test, 25, 33);
    endianCompare(byteContainer, test, 34, 37);
    directCompare(byteContainer, test, 38, 38);
}

TEST_CASE("PARSER TESTCASE #5", "[parseStockTradingAction]") {
    ByteContainer byteContainer = ByteContainer{};
    Byte test[] = {
        std::byte{0x48}, std::byte{0x05}, std::byte{0x2C},
        std::byte{0x00}, std::byte{0x07}, std::byte{0x00}, 
        std::byte{0x00}, std::byte{0x3A}, std::byte{0xDE}, 
        std::byte{0x68}, std::byte{0xB1}, std::byte{0x41}, 
        std::byte{0x41}, std::byte{0x50}, std::byte{0x4C}, 
        std::byte{0x20}, std::byte{0x20}, std::byte{0x20},
        std::byte{0x20}, std::byte{0x54}, std::byte{0x00},
        std::byte{0x4D}, std::byte{0x56}, std::byte{0x49},
        std::byte{0x20}
    };

    parseStockTradingAction(byteContainer, test);
    directCompare(byteContainer, test, 0, 0);
    endianCompare(byteContainer, test, 1, 2);
    endianCompare(byteContainer, test, 3, 4);
    endianCompare(byteContainer, test, 5, 10);
    directCompare(byteContainer, test, 11, 24);
}
