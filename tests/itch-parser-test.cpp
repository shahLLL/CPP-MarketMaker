#include <catch2/catch_test_macros.hpp>
#include "../headers/itch-parser.hpp"

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

TEST_CASE("PARSER TESTCASE #3", "[parseSystemEventMessage]") {
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
    REQUIRE(byteContainer[0] == std::byte{0x53});
    REQUIRE(byteContainer[1] == Byte{0x2A});
    REQUIRE(byteContainer[2] == Byte{0x00});
    REQUIRE(byteContainer[3] == Byte{0xE9});
    REQUIRE(byteContainer[4] == Byte{0x03});
    REQUIRE(byteContainer[5] == Byte{0xF4});
    REQUIRE(byteContainer[6] == Byte{0x01});
    REQUIRE(byteContainer[7] == Byte{0x00});
    REQUIRE(byteContainer[8] == Byte{0x00});
    REQUIRE(byteContainer[9] == Byte{0x00});
    REQUIRE(byteContainer[10] == Byte{0x00});
    REQUIRE(byteContainer[11] == Byte{0x4F});
}
