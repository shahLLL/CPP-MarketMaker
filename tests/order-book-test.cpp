#include <catch2/catch_test_macros.hpp>
#include "../headers/order-book.hpp"

TEST_CASE("ORDERBOOK TESTCASE #1", "[addOrder]") {
    OrderBook orderBook = OrderBook();
    UInt32 testPrice1 = 50'020;
    UInt32 testQuantity1 = 12;
    UInt32 testPrice2 = 50'200;
    UInt32 testQuantity2 = 14;
    UInt32 testPrice3 = 50'025;
    UInt32 testQuantity3 = 10;
    UInt32 testPrice4 = 20'025;
    UInt32 testPrice5 = 350'025;
    REQUIRE(orderBook.getQuantity(testPrice1) == 0);
    REQUIRE(orderBook.addOrder(1, Side::BUY, testQuantity1, testPrice1));
    REQUIRE(orderBook.getQuantity(testPrice1) == testQuantity1);
    REQUIRE(orderBook.addOrder(2, Side::BUY, testQuantity1, testPrice1));
    REQUIRE(orderBook.getQuantity(testPrice1) == 2*testQuantity1);
    REQUIRE(orderBook.getQuantity(testPrice2) == 0);
    REQUIRE(orderBook.addOrder(3, Side::SELL, testQuantity2, testPrice2));
    REQUIRE(orderBook.getQuantity(testPrice2) == testQuantity2);
    REQUIRE(orderBook.getQuantity(testPrice3) == 0);
    REQUIRE(orderBook.addOrder(4, Side::BUY, testQuantity3, testPrice3));
    REQUIRE(orderBook.getQuantity(testPrice3) == testQuantity3);

    REQUIRE(!orderBook.addOrder(5, Side::BUY, testQuantity3, testPrice4));
    REQUIRE(!orderBook.addOrder(6, Side::SELL, testQuantity3, testPrice4));
    REQUIRE(!orderBook.addOrder(7, Side::BUY, testQuantity3, testPrice5));
    REQUIRE(!orderBook.addOrder(8, Side::SELL, testQuantity3, testPrice5));
}

TEST_CASE("ORDERBOOK TESTCASE #2", "[cancelOrExecuteOrder]") {
    OrderBook orderBook = OrderBook();
    UInt32 testPrice1 = 50'020;
    UInt32 testQuantity1 = 12;
    REQUIRE(orderBook.getQuantity(testPrice1) == 0);
    REQUIRE(orderBook.addOrder(1, Side::BUY, testQuantity1, testPrice1));
    REQUIRE(orderBook.getQuantity(testPrice1) == testQuantity1);
    orderBook.cancelOrExecuteOrder(1, 4);
    REQUIRE(orderBook.getQuantity(testPrice1) == testQuantity1 - 4);
    orderBook.cancelOrExecuteOrder(1, 8);
    REQUIRE(orderBook.getQuantity(testPrice1) == 0);

    UInt32 testPrice2 = 50'030;
    UInt32 testQuantity2 = 15;
    REQUIRE(orderBook.getQuantity(testPrice2) == 0);
    REQUIRE(orderBook.addOrder(2, Side::SELL, testQuantity2, testPrice2));
    REQUIRE(orderBook.getQuantity(testPrice2) == testQuantity2);
    orderBook.cancelOrExecuteOrder(2, 5);
    REQUIRE(orderBook.getQuantity(testPrice2) == testQuantity2 - 5);
    orderBook.cancelOrExecuteOrder(2, 10);
    REQUIRE(orderBook.getQuantity(testPrice2) == 0);
}

TEST_CASE("ORDERBOOK TESTCASE #3", "[deleteOrder]") {
    OrderBook orderBook = OrderBook();
    UInt32 testPrice1 = 50'020;
    UInt32 testQuantity1 = 12;
    REQUIRE(orderBook.getQuantity(testPrice1) == 0);
    REQUIRE(orderBook.addOrder(1, Side::BUY, testQuantity1, testPrice1));
    REQUIRE(orderBook.getQuantity(testPrice1) == testQuantity1);
    orderBook.deleteOrder(1);
    REQUIRE(orderBook.getQuantity(testPrice1) == 0);

    UInt32 testPrice2 = 50'030;
    UInt32 testQuantity2 = 15;
    REQUIRE(orderBook.getQuantity(testPrice2) == 0);
    REQUIRE(orderBook.addOrder(2, Side::SELL, testQuantity2, testPrice2));
    REQUIRE(orderBook.getQuantity(testPrice2) == testQuantity2);
    orderBook.deleteOrder(2);
    REQUIRE(orderBook.getQuantity(testPrice2) == 0);
}

TEST_CASE("ORDERBOOK TESTCASE #4", "[replaceOrder]") {
    OrderBook orderBook = OrderBook();
    UInt32 testPriceOld1 = 50'020;
    UInt32 testQuantityOld1 = 12;
    UInt32 testPriceNew1 = 50'040;
    UInt32 testQuantityNew1 = 40;
    REQUIRE(orderBook.getQuantity(testPriceOld1) == 0);
    REQUIRE(orderBook.getQuantity(testPriceNew1) == 0);
    REQUIRE(orderBook.addOrder(1, Side::BUY, testQuantityOld1, testPriceOld1));
    REQUIRE(orderBook.getQuantity(testPriceOld1) == testQuantityOld1);
    REQUIRE(orderBook.getQuantity(testPriceNew1) == 0);
    REQUIRE(orderBook.replaceOrder(1, 3, testQuantityNew1, testPriceNew1));
    REQUIRE(orderBook.getQuantity(testPriceOld1) == 0);
    REQUIRE(orderBook.getQuantity(testPriceNew1) == testQuantityNew1);
    REQUIRE(!orderBook.replaceOrder(3, 5, testQuantityNew1, 19'000));
    REQUIRE(!orderBook.replaceOrder(3, 5, testQuantityNew1, 419'000));

    UInt32 testPriceOld2 = 50'080;
    UInt32 testQuantityOld2 = 42;
    UInt32 testPriceNew2 = 50'090;
    UInt32 testQuantityNew2 = 14;
    REQUIRE(orderBook.getQuantity(testPriceOld2) == 0);
    REQUIRE(orderBook.getQuantity(testPriceNew2) == 0);
    REQUIRE(orderBook.addOrder(2, Side::SELL, testQuantityOld2, testPriceOld2));
    REQUIRE(orderBook.getQuantity(testPriceOld2) == testQuantityOld2);
    REQUIRE(orderBook.getQuantity(testPriceNew2) == 0);
    REQUIRE(orderBook.replaceOrder(2, 4, testQuantityNew2, testPriceNew2));
    REQUIRE(orderBook.getQuantity(testPriceOld2) == 0);
    REQUIRE(orderBook.getQuantity(testPriceNew2) == testQuantityNew2);
    REQUIRE(!orderBook.replaceOrder(4, 6, testQuantityNew2, 10'000));
    REQUIRE(!orderBook.replaceOrder(4, 6, testQuantityNew2, 400'000));
}

TEST_CASE("ORDERBOOK TESTCASE #5", "[getBestBid]") {
    OrderBook orderBook = OrderBook();
    UInt32 price1 = 50'020;
    UInt32 price2 = 50'010;
    UInt32 price3 = 50'030;
    REQUIRE(orderBook.getBestBid() == NULL_CURSOR);
    REQUIRE(orderBook.addOrder(1, Side::BUY, 1, price1));
    REQUIRE(orderBook.getBestBid() == price1);
    REQUIRE(orderBook.addOrder(2, Side::BUY, 1, price2));
    REQUIRE(orderBook.getBestBid() == price1);
    REQUIRE(orderBook.addOrder(3, Side::BUY, 1, price3));
    REQUIRE(orderBook.getBestBid() == price3);
}

TEST_CASE("ORDERBOOK TESTCASE #6", "[getBestAsk]") {
    OrderBook orderBook = OrderBook();
    UInt32 price1 = 50'050;
    UInt32 price2 = 50'080;
    UInt32 price3 = 50'030;
    REQUIRE(orderBook.getBestAsk() == NULL_CURSOR);
    REQUIRE(orderBook.addOrder(1, Side::SELL, 1, price1));
    REQUIRE(orderBook.getBestAsk() == price1);
    REQUIRE(orderBook.addOrder(2, Side::SELL, 1, price2));
    REQUIRE(orderBook.getBestAsk() == price1);
    REQUIRE(orderBook.addOrder(3, Side::SELL, 1, price3));
    REQUIRE(orderBook.getBestAsk() == price3);
}

TEST_CASE("ORDERBOOK TESTCASE #7", "[getMidPrice]") {
    OrderBook orderBook = OrderBook();
    UInt32 price1 = 50'500;
    UInt32 price2 = 50'100;
    REQUIRE(orderBook.getMidPrice() == NULL_CURSOR);
    REQUIRE(orderBook.addOrder(1, Side::SELL, 4, price1));
    REQUIRE(orderBook.getMidPrice() == price1);
    orderBook.deleteOrder(1);
    REQUIRE(orderBook.getMidPrice() == NULL_CURSOR);
    REQUIRE(orderBook.addOrder(2, Side::BUY, 4, price2));
    REQUIRE(orderBook.getMidPrice() == price2);
    REQUIRE(orderBook.addOrder(3, Side::SELL, 4, price1));
    REQUIRE(orderBook.getMidPrice() == (price1 + price2)/2);
}