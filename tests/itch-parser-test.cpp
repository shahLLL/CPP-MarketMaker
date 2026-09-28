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