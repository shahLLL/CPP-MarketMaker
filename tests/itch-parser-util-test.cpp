#include <catch2/catch_test_macros.hpp>
#include "../headers/itch-parser-util.hpp"

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
    REQUIRE(ITCHParserUtil::getMessageType('S') == 12);
    REQUIRE(ITCHParserUtil::getMessageType('R') == 39);
    REQUIRE(ITCHParserUtil::getMessageType('H') == 25);
    REQUIRE(ITCHParserUtil::getMessageType('Y') == 20);
    REQUIRE(ITCHParserUtil::getMessageType('L') == 26);
    REQUIRE(ITCHParserUtil::getMessageType('V') == 35);
    REQUIRE(ITCHParserUtil::getMessageType('W') == 12);
    REQUIRE(ITCHParserUtil::getMessageType('K') == 28);
    REQUIRE(ITCHParserUtil::getMessageType('J') == 35);
    REQUIRE(ITCHParserUtil::getMessageType('h') == 21);
    REQUIRE(ITCHParserUtil::getMessageType('A') == 36);
    REQUIRE(ITCHParserUtil::getMessageType('F') == 40);
    REQUIRE(ITCHParserUtil::getMessageType('E') == 31);
    REQUIRE(ITCHParserUtil::getMessageType('C') == 36);
    REQUIRE(ITCHParserUtil::getMessageType('X') == 23);
    REQUIRE(ITCHParserUtil::getMessageType('D') == 19);
    REQUIRE(ITCHParserUtil::getMessageType('U') == 35);
    REQUIRE(ITCHParserUtil::getMessageType('P') == 44);
    REQUIRE(ITCHParserUtil::getMessageType('Q') == 40);
    REQUIRE(ITCHParserUtil::getMessageType('B') == 19);
    REQUIRE(ITCHParserUtil::getMessageType('I') == 50);
    REQUIRE(ITCHParserUtil::getMessageType('O') == 48);

    REQUIRE(ITCHParserUtil::getMessageType('r') == ITCHParserUtil::NULL_MESSAGE_SIGNAL);
    REQUIRE(ITCHParserUtil::getMessageType('Z') == ITCHParserUtil::NULL_MESSAGE_SIGNAL);
    REQUIRE(ITCHParserUtil::getMessageType('3') == ITCHParserUtil::NULL_MESSAGE_SIGNAL);
    REQUIRE(ITCHParserUtil::getMessageType('?') == ITCHParserUtil::NULL_MESSAGE_SIGNAL);
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
    
    ITCHParserUtil::endianSwap(byteContainer1, test1, 0, 1);
    ITCHParserUtil::endianSwap(byteContainer2, test2, 0, 3);
    ITCHParserUtil::endianSwap(byteContainer3, test3, 0, 5);
    ITCHParserUtil::endianSwap(byteContainer4, test4, 0, 7);

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

    ITCHParserUtil::directCopy(byteContainer, test, 0, 7);
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

    ITCHParserUtil::parseSystemEventMessage(byteContainer, test);
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

    ITCHParserUtil::parseStockDirectory(byteContainer, test);
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

TEST_CASE("PARSER TESTCASE #6", "[parseStockTradingAction]") {
    ByteContainer byteContainer = ByteContainer{};
    Byte test[] = {
        Byte{0x48}, Byte{0x05}, Byte{0x2C},
        Byte{0x00}, Byte{0x07}, Byte{0x00}, 
        Byte{0x00}, Byte{0x3A}, Byte{0xDE}, 
        Byte{0x68}, Byte{0xB1}, Byte{0x41}, 
        Byte{0x41}, Byte{0x50}, Byte{0x4C}, 
        Byte{0x20}, Byte{0x20}, Byte{0x20},
        Byte{0x20}, Byte{0x54}, Byte{0x00},
        Byte{0x4D}, Byte{0x56}, Byte{0x49},
        Byte{0x20}
    };

    ITCHParserUtil::parseStockTradingAction(byteContainer, test);
    directCompare(byteContainer, test, 0, 0);
    endianCompare(byteContainer, test, 1, 2);
    endianCompare(byteContainer, test, 3, 4);
    endianCompare(byteContainer, test, 5, 10);
    directCompare(byteContainer, test, 11, 24);
}

TEST_CASE("PARSER TESTCASE #7", "[parseRegSHORestriction]") {
    ByteContainer byteContainer = ByteContainer{};
    Byte test[] = {
        Byte{0x59}, Byte{0x02}, Byte{0x00},
        Byte{0x00}, Byte{0x0C}, Byte{0x00}, 
        Byte{0x00}, Byte{0x00}, Byte{0x08}, 
        Byte{0x4A}, Byte{0xEA}, Byte{0x4D}, 
        Byte{0x53}, Byte{0x46}, Byte{0x54}, 
        Byte{0x20}, Byte{0x20}, Byte{0x20}, 
        Byte{0x20}, Byte{0x31}
    };

    ITCHParserUtil::parseRegSHORestriction(byteContainer, test);
    directCompare(byteContainer, test, 0, 0);
    endianCompare(byteContainer, test, 1, 2);
    endianCompare(byteContainer, test, 3, 4);
    endianCompare(byteContainer, test, 5, 10);
    directCompare(byteContainer, test, 11, 19);
}

TEST_CASE("PARSER TESTCASE #8", "[parseMarketParticipantPosition]") {
    ByteContainer byteContainer = ByteContainer{};
    Byte test[] = {
        Byte{0x4C}, Byte{0x20}, Byte{0x00},
        Byte{0x00}, Byte{0x03}, Byte{0x00}, 
        Byte{0x00}, Byte{0xCE}, Byte{0x07}, 
        Byte{0xF2}, Byte{0x34}, Byte{0x42}, 
        Byte{0x41}, Byte{0x52}, Byte{0x43},
        Byte{0x54}, Byte{0x53}, Byte{0x4C}, 
        Byte{0x41}, Byte{0x20}, Byte{0x20}, 
        Byte{0x20}, Byte{0x20}, Byte{0x59},
        Byte{0x4E}, Byte{0x41}
    };

    ITCHParserUtil::parseMarketParticipantPosition(byteContainer, test);
    directCompare(byteContainer, test, 0, 0);
    endianCompare(byteContainer, test, 1, 2);
    endianCompare(byteContainer, test, 3, 4);
    endianCompare(byteContainer, test, 5, 10);
    directCompare(byteContainer, test, 11, 25);
}

TEST_CASE("PARSER TESTCASE #9", "[parseMWCBDeclineLevelMessage]") {
    ByteContainer byteContainer = ByteContainer{};
    Byte test[] = {
        Byte{0x56}, Byte{0x00}, Byte{0x00},
        Byte{0x00}, Byte{0x01}, Byte{0x00}, 
        Byte{0x00}, Byte{0x0C}, Byte{0xE0}, 
        Byte{0x7F}, Byte{0x23}, Byte{0x00},
        Byte{0x00}, Byte{0x00}, Byte{0x10},
        Byte{0x4C}, Byte{0x53}, Byte{0x3C},
        Byte{0x00}, Byte{0x00}, Byte{0x00},
        Byte{0x00}, Byte{0x12}, Byte{0xA0},
        Byte{0x5F}, Byte{0x20}, Byte{0x00},
        Byte{0x00}, Byte{0x00}, Byte{0x00},
        Byte{0x14}, Byte{0xF4}, Byte{0x6B},
        Byte{0x04}, Byte{0x00}
    };

    ITCHParserUtil::parseMWCBDeclineLevelMessage(byteContainer, test);
    directCompare(byteContainer, test, 0, 0);
    endianCompare(byteContainer, test, 1, 2);
    endianCompare(byteContainer, test, 3, 4);
    endianCompare(byteContainer, test, 5, 10);
    endianCompare(byteContainer, test, 11, 18);
    endianCompare(byteContainer, test, 19, 26);
    endianCompare(byteContainer, test, 27, 34);
}

TEST_CASE("PARSER TESTCASE #10", "[parseMWCBStatusMessage]") {
    ByteContainer byteContainer = ByteContainer{};
    Byte test[] = {
        Byte{0x57}, Byte{0x05}, Byte{0x2C},
        Byte{0x01}, Byte{0xC8}, Byte{0x00},
        Byte{0x00}, Byte{0xCE}, Byte{0x07},
        Byte{0xF2}, Byte{0x34}, Byte{0x31}
    };

    ITCHParserUtil::parseMWCBStatusMessage(byteContainer, test);
    directCompare(byteContainer, test, 0, 0);
    endianCompare(byteContainer, test, 1, 2);
    endianCompare(byteContainer, test, 3, 4);
    endianCompare(byteContainer, test, 5, 10);
    directCompare(byteContainer, test, 11, 11);
}

TEST_CASE("PARSER TESTCASE #11", "[parseQuotingPeriodUpdate]") {
    ByteContainer byteContainer = ByteContainer{};
    Byte test[] = {
        Byte{0x4B}, Byte{0x05}, Byte{0x2C},
        Byte{0x01}, Byte{0xC8}, Byte{0x00}, 
        Byte{0x00}, Byte{0xCE}, Byte{0x07},
        Byte{0xF2}, Byte{0x34}, Byte{0x4E},
        Byte{0x56}, Byte{0x44}, Byte{0x41},
        Byte{0x20}, Byte{0x20}, Byte{0x20},
        Byte{0x20}, Byte{0x00}, Byte{0x00},
        Byte{0x85}, Byte{0x98}, Byte{0x41},
        Byte{0x00}, Byte{0x02}, Byte{0x49},
        Byte{0xF0}
    };

    ITCHParserUtil::parseQuotingPeriodUpdate(byteContainer, test);
    directCompare(byteContainer, test, 0, 0);
    endianCompare(byteContainer, test, 1, 2);
    endianCompare(byteContainer, test, 3, 4);
    endianCompare(byteContainer, test, 5, 10);
    directCompare(byteContainer, test, 11, 18);
    endianCompare(byteContainer, test, 19, 22);
    directCompare(byteContainer, test, 23, 23);
    endianCompare(byteContainer, test, 24, 27);
}

TEST_CASE("PARSER TESTCASE #12", "[parseLULDAuctionCollar]") {
    ByteContainer byteContainer = ByteContainer{};
    Byte test[] = {
        Byte{0x4A}, Byte{0x05}, Byte{0x2C},
        Byte{0x01}, Byte{0xC8}, Byte{0x00},
        Byte{0x00}, Byte{0xCE}, Byte{0x07},
        Byte{0xF2}, Byte{0x34}, Byte{0x41},
        Byte{0x41}, Byte{0x50}, Byte{0x4C},
        Byte{0x20}, Byte{0x20}, Byte{0x20},
        Byte{0x20}, Byte{0x00}, Byte{0x1A},
        Byte{0xB3}, Byte{0xF0}, Byte{0x00},
        Byte{0x1D}, Byte{0x5F}, Byte{0x88},
        Byte{0x00}, Byte{0x18}, Byte{0x08}, 
        Byte{0x58}, Byte{0x00}, Byte{0x00}, 
        Byte{0x00}, Byte{0x02}
    };

    ITCHParserUtil::parseLULDAuctionCollar(byteContainer, test);
    directCompare(byteContainer, test, 0, 0);
    endianCompare(byteContainer, test, 1, 2);
    endianCompare(byteContainer, test, 3, 4);
    endianCompare(byteContainer, test, 5, 10);
    directCompare(byteContainer, test, 11, 18);
    endianCompare(byteContainer, test, 19, 22);
    endianCompare(byteContainer, test, 23, 26);
    endianCompare(byteContainer, test, 27, 30);
    endianCompare(byteContainer, test, 31, 34);
}

TEST_CASE("PARSER TESTCASE #13", "[parseOperationalHalt]") {
    ByteContainer byteContainer = ByteContainer{};
    Byte test[] = {
        Byte{0x68}, Byte{0x05}, Byte{0x2C},
        Byte{0x01}, Byte{0xC8}, Byte{0x00},
        Byte{0x00}, Byte{0xCE}, Byte{0x07},
        Byte{0xF2}, Byte{0x34}, Byte{0x41}, 
        Byte{0x4D}, Byte{0x5A}, Byte{0x4E},
        Byte{0x20}, Byte{0x20}, Byte{0x20},
        Byte{0x20}, Byte{0x51}, Byte{0x48}
    };

    ITCHParserUtil::parseOperationalHalt(byteContainer, test);
    directCompare(byteContainer, test, 0, 0);
    endianCompare(byteContainer, test, 1, 2);
    endianCompare(byteContainer, test, 3, 4);
    endianCompare(byteContainer, test, 5, 10);
    directCompare(byteContainer, test, 11, 20);
}

TEST_CASE("PARSER TESTCASE #14", "[parseAddOrderMessage]") {
    ByteContainer byteContainer = ByteContainer{};
    Byte test[] = {
        Byte{0x41}, Byte{0x05}, Byte{0x2C},
        Byte{0x01}, Byte{0xC8}, Byte{0x00},
        Byte{0x00}, Byte{0xCE}, Byte{0x07},
        Byte{0xF2}, Byte{0x34}, Byte{0x00},
        Byte{0x00}, Byte{0x00}, Byte{0x02},
        Byte{0x4C}, Byte{0xBC}, Byte{0x6B},
        Byte{0x4A}, Byte{0x42}, Byte{0x00},
        Byte{0x00}, Byte{0x01}, Byte{0xF4},
        Byte{0x4D}, Byte{0x53}, Byte{0x46},
        Byte{0x54}, Byte{0x20}, Byte{0x20},
        Byte{0x20}, Byte{0x20}, Byte{0x00},
        Byte{0x40}, Byte{0x2A}, Byte{0x88}
    };

    ITCHParserUtil::parseAddOrderMessage(byteContainer, test);
    directCompare(byteContainer, test, 0, 0);
    endianCompare(byteContainer, test, 1, 2);
    endianCompare(byteContainer, test, 3, 4);
    endianCompare(byteContainer, test, 5, 10);
    endianCompare(byteContainer, test, 11, 18);
    directCompare(byteContainer, test, 19, 19);
    endianCompare(byteContainer, test, 20, 23);
    directCompare(byteContainer, test, 24, 31);
    endianCompare(byteContainer, test, 32, 35);
}

TEST_CASE("PARSER TESTCASE #15", "[parseAddOrderMPIDAttributionMessage]") {
    ByteContainer byteContainer = ByteContainer{};
    Byte test[] = {
        Byte{0x46}, Byte{0x05}, Byte{0x2C},
        Byte{0x01}, Byte{0xC8}, Byte{0x00}, 
        Byte{0x00}, Byte{0xCE}, Byte{0x07}, 
        Byte{0xF2}, Byte{0x34}, Byte{0x00},
        Byte{0x00}, Byte{0x00}, Byte{0x02},
        Byte{0x4C}, Byte{0xBC}, Byte{0x6B},
        Byte{0x4A}, Byte{0x42}, Byte{0x00},
        Byte{0x00}, Byte{0x01}, Byte{0xF4},
        Byte{0x4D}, Byte{0x53}, Byte{0x46},
        Byte{0x54}, Byte{0x20}, Byte{0x20},
        Byte{0x20}, Byte{0x20}, Byte{0x00},
        Byte{0x40}, Byte{0x2A}, Byte{0x88},
        Byte{0x41}, Byte{0x42}, Byte{0x43},
        Byte{0x44}
    };

    ITCHParserUtil::parseAddOrderMPIDAttributionMessage(byteContainer, test);
    directCompare(byteContainer, test, 0, 0);
    endianCompare(byteContainer, test, 1, 2);
    endianCompare(byteContainer, test, 3, 4);
    endianCompare(byteContainer, test, 5, 10);
    endianCompare(byteContainer, test, 11, 18);
    directCompare(byteContainer, test, 19, 19);
    endianCompare(byteContainer, test, 20, 23);
    directCompare(byteContainer, test, 24, 31);
    endianCompare(byteContainer, test, 32, 35);
    directCompare(byteContainer, test, 36, 39);
}

TEST_CASE("PARSER TESTCASE #16", "[parseOrderExecutedMessage]") {
    ByteContainer byteContainer = ByteContainer{};
    Byte test[] = {
        Byte{0x45}, Byte{0x05}, Byte{0x2C},
        Byte{0x01}, Byte{0xC8}, Byte{0x00},
        Byte{0x00}, Byte{0x01}, Byte{0x54},
        Byte{0x02}, Byte{0x34}, Byte{0x00},
        Byte{0x00}, Byte{0x00}, Byte{0x02},
        Byte{0x72}, Byte{0x03}, Byte{0xDB},
        Byte{0x4A}, Byte{0x00}, Byte{0x00},
        Byte{0x01}, Byte{0xF4}, Byte{0x00},
        Byte{0x00}, Byte{0x00}, Byte{0x00},
        Byte{0x07}, Byte{0x5B}, Byte{0xCD},
        Byte{0x15}
    };

    ITCHParserUtil::parseOrderExecutedMessage(byteContainer, test);
    directCompare(byteContainer, test, 0, 0);
    endianCompare(byteContainer, test, 1, 2);
    endianCompare(byteContainer, test, 3, 4);
    endianCompare(byteContainer, test, 5, 10);
    endianCompare(byteContainer, test, 11, 18);
    endianCompare(byteContainer, test, 19, 22);
    endianCompare(byteContainer, test, 23, 30);
}

TEST_CASE("PARSER TESTCASE #17", "[parseOrderExecutedWithPriceMessage]") {
    ByteContainer byteContainer = ByteContainer{};
    Byte test[] = {
        Byte{0x45}, Byte{0x05}, Byte{0x2C},
        Byte{0x01}, Byte{0xC8}, Byte{0x00},
        Byte{0x00}, Byte{0x01}, Byte{0x54},
        Byte{0x02}, Byte{0x34}, Byte{0x00},
        Byte{0x00}, Byte{0x00}, Byte{0x02},
        Byte{0x72}, Byte{0x03}, Byte{0xDB},
        Byte{0x4A}, Byte{0x00}, Byte{0x00},
        Byte{0x01}, Byte{0xF4}, Byte{0x00},
        Byte{0x00}, Byte{0x00}, Byte{0x00},
        Byte{0x07}, Byte{0x5B}, Byte{0xCD},
        Byte{0x15}, Byte{0x59}, Byte{0x00}, 
        Byte{0x40}, Byte{0x2A}, Byte{0x88}
    };

    ITCHParserUtil::parseOrderExecutedWithPriceMessage(byteContainer, test);
    directCompare(byteContainer, test, 0, 0);
    endianCompare(byteContainer, test, 1, 2);
    endianCompare(byteContainer, test, 3, 4);
    endianCompare(byteContainer, test, 5, 10);
    endianCompare(byteContainer, test, 11, 18);
    endianCompare(byteContainer, test, 19, 22);
    endianCompare(byteContainer, test, 23, 30);
    directCompare(byteContainer, test, 31, 31);
    endianCompare(byteContainer, test, 32, 35);
}

TEST_CASE("PARSER TESTCASE #18", "[parseOrderCancelMessage]") {
    ByteContainer byteContainer = ByteContainer{};
    Byte test[] = {
       Byte{0x58}, Byte{0x05}, Byte{0x2C},
       Byte{0x01}, Byte{0xC8}, Byte{0x00},
       Byte{0x00}, Byte{0x01}, Byte{0x54},
       Byte{0x02}, Byte{0x34}, Byte{0x00},
       Byte{0x00}, Byte{0x00}, Byte{0x02},
       Byte{0x72}, Byte{0x03}, Byte{0xDB},
       Byte{0x4A}, Byte{0x00}, Byte{0x00}, 
       Byte{0x01}, Byte{0xF4}
    };

    ITCHParserUtil::parseOrderCancelMessage(byteContainer, test);
    directCompare(byteContainer, test, 0, 0);
    endianCompare(byteContainer, test, 1, 2);
    endianCompare(byteContainer, test, 3, 4);
    endianCompare(byteContainer, test, 5, 10);
    endianCompare(byteContainer, test, 11, 18);
    endianCompare(byteContainer, test, 19, 22);
}

TEST_CASE("PARSER TESTCASE #19", "[parseOrderDeleteMessage]") {
    ByteContainer byteContainer = ByteContainer{};
    Byte test[] = {
       Byte{0x44}, Byte{0x05}, Byte{0x2C}, 
       Byte{0x01}, Byte{0xC8}, Byte{0x00},
       Byte{0x00}, Byte{0x01}, Byte{0x54},
       Byte{0x02}, Byte{0x34}, Byte{0x00},
       Byte{0x00}, Byte{0x00}, Byte{0x02},
       Byte{0x72}, Byte{0x03}, Byte{0xDB},
       Byte{0x4A}
    };

    ITCHParserUtil::parseOrderDeleteMessage(byteContainer, test);
    directCompare(byteContainer, test, 0, 0);
    endianCompare(byteContainer, test, 1, 2);
    endianCompare(byteContainer, test, 3, 4);
    endianCompare(byteContainer, test, 5, 10);
    endianCompare(byteContainer, test, 11, 18);
}

TEST_CASE("PARSER TESTCASE #20", "[parseOrderReplaceMessage]") {
    ByteContainer byteContainer = ByteContainer{};
    Byte test[] = {
      Byte{0x55}, Byte{0x05}, Byte{0x2C},
      Byte{0x01}, Byte{0xC8}, Byte{0x00},
      Byte{0x00}, Byte{0x01}, Byte{0x54},
      Byte{0x02}, Byte{0x34}, Byte{0x00},
      Byte{0x00}, Byte{0x00}, Byte{0x02},
      Byte{0x72}, Byte{0x03}, Byte{0xDB},
      Byte{0x4A}, Byte{0x00}, Byte{0x00},
      Byte{0x00}, Byte{0x08}, Byte{0x41},
      Byte{0x12}, Byte{0xA2}, Byte{0xB2},
      Byte{0x00}, Byte{0x00}, Byte{0x01},
      Byte{0xF4}, Byte{0x00}, Byte{0x40},
      Byte{0x2A}, Byte{0x88}
    };

    ITCHParserUtil::parseOrderReplaceMessage(byteContainer, test);
    directCompare(byteContainer, test, 0, 0);
    endianCompare(byteContainer, test, 1, 2);
    endianCompare(byteContainer, test, 3, 4);
    endianCompare(byteContainer, test, 5, 10);
    endianCompare(byteContainer, test, 11, 18);
    endianCompare(byteContainer, test, 19, 26);
    endianCompare(byteContainer, test, 27, 30);
    endianCompare(byteContainer, test, 31, 34);
}

TEST_CASE("PARSER TESTCASE #21", "[parseTradeMessage]") {
    ByteContainer byteContainer = ByteContainer{};
    Byte test[] = {
      Byte{0x50}, Byte{0x05}, Byte{0x2C},
      Byte{0x01}, Byte{0xC8}, Byte{0x00},
      Byte{0x00}, Byte{0x01}, Byte{0x54},
      Byte{0x02}, Byte{0x34}, Byte{0x00},
      Byte{0x00}, Byte{0x00}, Byte{0x02},
      Byte{0x72}, Byte{0x03}, Byte{0xDB},
      Byte{0x4A}, Byte{0x59}, Byte{0x00},
      Byte{0x00}, Byte{0x01}, Byte{0xF4},
      Byte{0x41}, Byte{0x41}, Byte{0x50},
      Byte{0x4C}, Byte{0x20}, Byte{0x20},
      Byte{0x20}, Byte{0x20}, Byte{0x00},
      Byte{0x40}, Byte{0x2A}, Byte{0x88},
      Byte{0x00}, Byte{0x00}, Byte{0x00},
      Byte{0x00}, Byte{0x07}, Byte{0x5B},
      Byte{0xCD}, Byte{0x15}
    };

    ITCHParserUtil::parseTradeMessage(byteContainer, test);
    directCompare(byteContainer, test, 0, 0);
    endianCompare(byteContainer, test, 1, 2);
    endianCompare(byteContainer, test, 3, 4);
    endianCompare(byteContainer, test, 5, 10);
    endianCompare(byteContainer, test, 11, 18);
    directCompare(byteContainer, test, 19, 19);
    endianCompare(byteContainer, test, 20, 23);
    directCompare(byteContainer, test, 24, 31);
    endianCompare(byteContainer, test, 32, 35);
    endianCompare(byteContainer, test, 36, 43);
}

TEST_CASE("PARSER TESTCASE #22", "[parseCrossTradeMessage]") {
    ByteContainer byteContainer = ByteContainer{};
    Byte test[] = {
      Byte{0x51}, Byte{0x05}, Byte{0x2C},
      Byte{0x01}, Byte{0xC8}, Byte{0x00}, 
      Byte{0x00}, Byte{0x01}, Byte{0x54}, 
      Byte{0x02}, Byte{0x34}, Byte{0x00}, 
      Byte{0x00}, Byte{0x00}, Byte{0x00},
      Byte{0x07}, Byte{0x5B}, Byte{0xCD},
      Byte{0x15}, Byte{0x41}, Byte{0x41},
      Byte{0x50}, Byte{0x4C}, Byte{0x20},
      Byte{0x20}, Byte{0x20}, Byte{0x20},
      Byte{0x00}, Byte{0x00}, Byte{0x01},
      Byte{0xF4}, Byte{0x00}, Byte{0x00},
      Byte{0x00}, Byte{0x00}, Byte{0x07},
      Byte{0x5B}, Byte{0xCD}, Byte{0x15},
      Byte{0x4F},
    };

    ITCHParserUtil::parseCrossTradeMessage(byteContainer, test);
    directCompare(byteContainer, test, 0, 0);
    endianCompare(byteContainer, test, 1, 2);
    endianCompare(byteContainer, test, 3, 4);
    endianCompare(byteContainer, test, 5, 10);
    endianCompare(byteContainer, test, 11, 18);
    directCompare(byteContainer, test, 19, 26);
    endianCompare(byteContainer, test, 27, 30);
    endianCompare(byteContainer, test, 31, 38);
    directCompare(byteContainer, test, 39, 39);
}

TEST_CASE("PARSER TESTCASE #23", "[parseBrokenTradeMessage]") {
    ByteContainer byteContainer = ByteContainer{};
    Byte test[] = {
      Byte{0x42}, Byte{0x05}, Byte{0x2C},
      Byte{0x01}, Byte{0xC8}, Byte{0x00},
      Byte{0x00}, Byte{0x01}, Byte{0x54},
      Byte{0x02}, Byte{0x34}, Byte{0x00},
      Byte{0x00}, Byte{0x00}, Byte{0x00},
      Byte{0x07}, Byte{0x5B}, Byte{0xCD},
      Byte{0x15}
    };

    ITCHParserUtil::parseBrokenTradeMessage(byteContainer, test);
    directCompare(byteContainer, test, 0, 0);
    endianCompare(byteContainer, test, 1, 2);
    endianCompare(byteContainer, test, 3, 4);
    endianCompare(byteContainer, test, 5, 10);
    endianCompare(byteContainer, test, 11, 18);
}

TEST_CASE("PARSER TESTCASE #24", "[parseNOIIMessage]") {
    ByteContainer byteContainer = ByteContainer{};
    Byte test[] = {
      Byte{0x49}, Byte{0x05}, Byte{0x2C},
      Byte{0x01}, Byte{0xC8}, Byte{0x00},
      Byte{0x00}, Byte{0x01}, Byte{0x54},
      Byte{0x02}, Byte{0x34}, Byte{0x00},
      Byte{0x00}, Byte{0x00}, Byte{0x00},
      Byte{0x07}, Byte{0x5B}, Byte{0xCD},
      Byte{0x15}, Byte{0x00}, Byte{0x00},
      Byte{0x00}, Byte{0x00}, Byte{0x07},
      Byte{0x5B}, Byte{0xCD}, Byte{0x15},
      Byte{0x4F}, Byte{0x41}, Byte{0x41},
      Byte{0x50}, Byte{0x4C}, Byte{0x20},
      Byte{0x20}, Byte{0x20}, Byte{0x20},
      Byte{0x00}, Byte{0x00}, Byte{0x01},
      Byte{0xF4}, Byte{0x00}, Byte{0x00},
      Byte{0x01}, Byte{0xF4}, Byte{0x00},
      Byte{0x00}, Byte{0x01}, Byte{0xF4},
      Byte{0x4F}, Byte{0x41},
    };

    ITCHParserUtil::parseNOIIMessage(byteContainer, test);
    directCompare(byteContainer, test, 0, 0);
    endianCompare(byteContainer, test, 1, 2);
    endianCompare(byteContainer, test, 3, 4);
    endianCompare(byteContainer, test, 5, 10);
    endianCompare(byteContainer, test, 11, 18);
    endianCompare(byteContainer, test, 19, 26);
    directCompare(byteContainer, test, 27, 35);
    endianCompare(byteContainer, test, 36, 39);
    endianCompare(byteContainer, test, 40, 43);
    endianCompare(byteContainer, test, 44, 47);
    directCompare(byteContainer, test, 48, 49);
}

TEST_CASE("PARSER TESTCASE #25", "[parseDLWCRPD]") {
    ByteContainer byteContainer = ByteContainer{};
    Byte test[] = {
      Byte{0x4F}, Byte{0x05}, Byte{0x2C},
      Byte{0x01}, Byte{0xC8}, Byte{0x00},
      Byte{0x00}, Byte{0x01}, Byte{0x54},
      Byte{0x02}, Byte{0x34}, Byte{0x41},
      Byte{0x41}, Byte{0x50}, Byte{0x4C},
      Byte{0x20}, Byte{0x20}, Byte{0x20},
      Byte{0x20}, Byte{0x4E}, Byte{0x00},
      Byte{0x00}, Byte{0x01}, Byte{0xF4},
      Byte{0x00}, Byte{0x00}, Byte{0x01},
      Byte{0xF4}, Byte{0x00}, Byte{0x00},
      Byte{0x01}, Byte{0xF4}, Byte{0x00},
      Byte{0x00}, Byte{0x00}, Byte{0x00},
      Byte{0x07}, Byte{0x5B}, Byte{0xCD},
      Byte{0x15}, Byte{0x00}, Byte{0x00},
      Byte{0x01}, Byte{0xF4}, Byte{0x00},
      Byte{0x00}, Byte{0x01}, Byte{0xF4}
    };

    ITCHParserUtil::parseDLWCRPD(byteContainer, test);
    endianCompare(byteContainer, test, 1, 2);
    endianCompare(byteContainer, test, 3, 4);
    endianCompare(byteContainer, test, 5, 10);
    directCompare(byteContainer, test, 11, 19);
    endianCompare(byteContainer, test, 20, 23);
    endianCompare(byteContainer, test, 24, 27);
    endianCompare(byteContainer, test, 28, 31);
    endianCompare(byteContainer, test, 32, 39);
    endianCompare(byteContainer, test, 40, 43);
    endianCompare(byteContainer, test, 44, 47);
}

TEST_CASE("PARSER TESTCASE #26", "[validMessageType]") {
    REQUIRE(!ITCHParserUtil::validMessageType('S'));
    REQUIRE(ITCHParserUtil::validMessageType('R'));
    REQUIRE(!ITCHParserUtil::validMessageType('H'));
    REQUIRE(!ITCHParserUtil::validMessageType('Y'));
    REQUIRE(!ITCHParserUtil::validMessageType('L'));
    REQUIRE(!ITCHParserUtil::validMessageType('V'));
    REQUIRE(!ITCHParserUtil::validMessageType('W'));
    REQUIRE(!ITCHParserUtil::validMessageType('K'));
    REQUIRE(!ITCHParserUtil::validMessageType('J'));
    REQUIRE(!ITCHParserUtil::validMessageType('h'));
    REQUIRE(ITCHParserUtil::validMessageType('A'));
    REQUIRE(ITCHParserUtil::validMessageType('F'));
    REQUIRE(ITCHParserUtil::validMessageType('E'));
    REQUIRE(ITCHParserUtil::validMessageType('C'));
    REQUIRE(ITCHParserUtil::validMessageType('X'));
    REQUIRE(ITCHParserUtil::validMessageType('D'));
    REQUIRE(ITCHParserUtil::validMessageType('U'));
    REQUIRE(!ITCHParserUtil::validMessageType('P'));
    REQUIRE(!ITCHParserUtil::validMessageType('Q'));
    REQUIRE(!ITCHParserUtil::validMessageType('B'));
    REQUIRE(!ITCHParserUtil::validMessageType('I'));
    REQUIRE(!ITCHParserUtil::validMessageType('O'));

    REQUIRE(!ITCHParserUtil::validMessageType('r'));
    REQUIRE(!ITCHParserUtil::validMessageType('Z'));
    REQUIRE(!ITCHParserUtil::validMessageType('3'));
    REQUIRE(!ITCHParserUtil::validMessageType('?'));
}