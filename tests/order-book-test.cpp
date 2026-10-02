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

TEST_CASE("ORDERBOOK TESTCASE #4", "[replaceOrder]") {
    OrderBook orderBook = OrderBook();
    UInt32 testPriceOld1 = 20;
    UInt32 testQuantityOld1 = 12;
    UInt32 testPriceNew1 = 40;
    UInt32 testQuantityNew1 = 40;
    REQUIRE(orderBook.getQuantity(testPriceOld1) == 0);
    REQUIRE(orderBook.getQuantity(testPriceNew1) == 0);
    orderBook.addOrder(1, Side::BUY, testQuantityOld1, testPriceOld1);
    REQUIRE(orderBook.getQuantity(testPriceOld1) == testQuantityOld1);
    REQUIRE(orderBook.getQuantity(testPriceNew1) == 0);
    orderBook.replaceOrder(1, 3, testQuantityNew1, testPriceNew1);
    REQUIRE(orderBook.getQuantity(testPriceOld1) == 0);
    REQUIRE(orderBook.getQuantity(testPriceNew1) == testQuantityNew1);

    UInt32 testPriceOld2 = 80;
    UInt32 testQuantityOld2 = 42;
    UInt32 testPriceNew2 = 90;
    UInt32 testQuantityNew2 = 14;
    REQUIRE(orderBook.getQuantity(testPriceOld2) == 0);
    REQUIRE(orderBook.getQuantity(testPriceNew2) == 0);
    orderBook.addOrder(2, Side::SELL, testQuantityOld2, testPriceOld2);
    REQUIRE(orderBook.getQuantity(testPriceOld2) == testQuantityOld2);
    REQUIRE(orderBook.getQuantity(testPriceNew2) == 0);
    orderBook.replaceOrder(2, 4, testQuantityNew2, testPriceNew2);
    REQUIRE(orderBook.getQuantity(testPriceOld2) == 0);
    REQUIRE(orderBook.getQuantity(testPriceNew2) == testQuantityNew2);
}

TEST_CASE("ORDERBOOK TESTCASE #5", "[getBestBid]") {
    OrderBook orderBook = OrderBook();
    UInt32 price1 = 20;
    UInt32 price2 = 10;
    UInt32 price3 = 30;
    REQUIRE(orderBook.getBestBid() == NULL_CURSOR);
    orderBook.addOrder(1, Side::BUY, 1, price1);
    REQUIRE(orderBook.getBestBid() == price1);
    orderBook.addOrder(2, Side::BUY, 1, price2);
    REQUIRE(orderBook.getBestBid() == price1);
    orderBook.addOrder(3, Side::BUY, 1, price3);
    REQUIRE(orderBook.getBestBid() == price3);
}

TEST_CASE("ORDERBOOK TESTCASE #6", "[getBestAsk]") {
    OrderBook orderBook = OrderBook();
    UInt32 price1 = 50;
    UInt32 price2 = 80;
    UInt32 price3 = 30;
    REQUIRE(orderBook.getBestAsk() == NULL_CURSOR);
    orderBook.addOrder(1, Side::SELL, 1, price1);
    REQUIRE(orderBook.getBestAsk() == price1);
    orderBook.addOrder(2, Side::SELL, 1, price2);
    REQUIRE(orderBook.getBestAsk() == price1);
    orderBook.addOrder(3, Side::SELL, 1, price3);
    REQUIRE(orderBook.getBestAsk() == price3);
}
