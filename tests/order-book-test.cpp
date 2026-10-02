#include <catch2/catch_test_macros.hpp>
#include "../headers/order-book.hpp"

TEST_CASE("ORDERBOOK TESTCASE #1", "[addOrder]") {
    OrderBook orderBook = OrderBook();
    UInt32 testPrice1 = 20;
    UInt32 testQuantity1 = 12;
    UInt32 testPrice2 = 200;
    UInt32 testQuantity2 = 14;
    UInt32 testPrice3 = 25;
    UInt32 testQuantity3 = 10;
    REQUIRE(orderBook.getQuantity(testPrice1) == 0);
    orderBook.addOrder(1, Side::BUY, testQuantity1, testPrice1);
    REQUIRE(orderBook.getQuantity(testPrice1) == testQuantity1);
    orderBook.addOrder(2, Side::BUY, testQuantity1, testPrice1);
    REQUIRE(orderBook.getQuantity(testPrice1) == 2*testQuantity1);
    REQUIRE(orderBook.getQuantity(testPrice2) == 0);
    orderBook.addOrder(3, Side::SELL, testQuantity2, testPrice2);
    REQUIRE(orderBook.getQuantity(testPrice2) == testQuantity2);
    REQUIRE(orderBook.getQuantity(testPrice3) == 0);
    orderBook.addOrder(4, Side::BUY, testQuantity3, testPrice3);
    REQUIRE(orderBook.getQuantity(testPrice3) == testQuantity3);
}

TEST_CASE("ORDERBOOK TESTCASE #2", "[cancelOrExecuteOrder]") {
    OrderBook orderBook = OrderBook();
    UInt32 testPrice1 = 20;
    UInt32 testQuantity1 = 12;
    REQUIRE(orderBook.getQuantity(testPrice1) == 0);
    orderBook.addOrder(1, Side::BUY, testQuantity1, testPrice1);
    REQUIRE(orderBook.getQuantity(testPrice1) == testQuantity1);
    orderBook.cancelOrExecuteOrder(1, 4);
    REQUIRE(orderBook.getQuantity(testPrice1) == testQuantity1 - 4);
    orderBook.cancelOrExecuteOrder(1, 8);
    REQUIRE(orderBook.getQuantity(testPrice1) == 0);

    UInt32 testPrice2 = 30;
    UInt32 testQuantity2 = 15;
    REQUIRE(orderBook.getQuantity(testPrice2) == 0);
    orderBook.addOrder(2, Side::SELL, testQuantity2, testPrice2);
    REQUIRE(orderBook.getQuantity(testPrice2) == testQuantity2);
    orderBook.cancelOrExecuteOrder(2, 5);
    REQUIRE(orderBook.getQuantity(testPrice2) == testQuantity2 - 5);
    orderBook.cancelOrExecuteOrder(2, 10);
    REQUIRE(orderBook.getQuantity(testPrice2) == 0);
}

TEST_CASE("ORDERBOOK TESTCASE #3", "[deleteOrder]") {
    OrderBook orderBook = OrderBook();
    UInt32 testPrice1 = 20;
    UInt32 testQuantity1 = 12;
    REQUIRE(orderBook.getQuantity(testPrice1) == 0);
    orderBook.addOrder(1, Side::BUY, testQuantity1, testPrice1);
    REQUIRE(orderBook.getQuantity(testPrice1) == testQuantity1);
    orderBook.deleteOrder(1);
    REQUIRE(orderBook.getQuantity(testPrice1) == 0);

    UInt32 testPrice2 = 30;
    UInt32 testQuantity2 = 15;
    REQUIRE(orderBook.getQuantity(testPrice2) == 0);
    orderBook.addOrder(2, Side::SELL, testQuantity2, testPrice2);
    REQUIRE(orderBook.getQuantity(testPrice2) == testQuantity2);
    orderBook.deleteOrder(2);
    REQUIRE(orderBook.getQuantity(testPrice2) == 0);
}
