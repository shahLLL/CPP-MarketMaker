#include <catch2/catch_test_macros.hpp>
#include "../headers/stock-map.hpp"

TEST_CASE("STOCKMAP TESTCASE #1", "[addStock]") {
    const UInt16 stockLocate1 = 1;
    const UInt16 stockLocate2 = 2;
    const UInt16 stockLocate3 = 3;

    const Byte stockSymbol1[] = {
        Byte{0x84}, 
        Byte{0x83}, 
        Byte{0x76}, 
        Byte{0x65},
        Byte{0x00}, 
        Byte{0x00},
        Byte{0x00}, 
        Byte{0x00}
    };
    const Byte stockSymbol2[] = {
        Byte{0x77}, 
        Byte{0x83}, 
        Byte{0x70}, 
        Byte{0x84},
        Byte{0x00}, 
        Byte{0x00},
        Byte{0x00}, 
        Byte{0x00}
    };
    const Byte stockSymbol3[] = {
        Byte{0x65}, 
        Byte{0x77}, 
        Byte{0x90}, 
        Byte{0x78},
        Byte{0x00}, 
        Byte{0x00},
        Byte{0x00}, 
        Byte{0x00}
    };
    StockMap stockMap = StockMap();

    REQUIRE(!stockMap.containsStock(stockLocate1));
    stockMap.addStock(stockLocate1, stockSymbol1);
    REQUIRE(stockMap.containsStock(stockLocate1));
    REQUIRE(!stockMap.containsStock(stockLocate2));
    stockMap.addStock(stockLocate2, stockSymbol2);
    REQUIRE(stockMap.containsStock(stockLocate2));
    REQUIRE(!stockMap.containsStock(stockLocate3));
    stockMap.addStock(stockLocate3, stockSymbol3);
    REQUIRE(stockMap.containsStock(stockLocate3));
}

TEST_CASE("STOCKMAP TESTCASE #2", "[addOrder]") {
    const UInt16 stockLocate1 = 1;
    UInt32 testPrice1 = 50'020;
    UInt32 testQuantity1 = 12;
    UInt32 testPrice2 = 50'200;
    UInt32 testQuantity2 = 14;
    UInt32 testPrice3 = 50'025;
    UInt32 testQuantity3 = 10;
    UInt32 testPrice4 = 20'025;
    UInt32 testPrice5 = 350'025;

    const Byte stockSymbol1[] = {
        Byte{0x84}, 
        Byte{0x83}, 
        Byte{0x76}, 
        Byte{0x65},
        Byte{0x00}, 
        Byte{0x00},
        Byte{0x00}, 
        Byte{0x00}
    };
    StockMap stockMap = StockMap();

    REQUIRE(!stockMap.addOrder(stockLocate1, 1, Side::BUY, testQuantity1, testPrice1));
    REQUIRE(!stockMap.addOrder(stockLocate1, 2, Side::BUY, testQuantity1, testPrice1));
    REQUIRE(!stockMap.addOrder(stockLocate1, 3, Side::SELL, testQuantity2, testPrice2));
    REQUIRE(!stockMap.addOrder(stockLocate1, 4, Side::BUY, testQuantity3, testPrice3));

    stockMap.addStock(stockLocate1, stockSymbol1);

    REQUIRE(stockMap.addOrder(stockLocate1, 1, Side::BUY, testQuantity1, testPrice1));
    REQUIRE(stockMap.addOrder(stockLocate1, 2, Side::BUY, testQuantity1, testPrice1));
    REQUIRE(stockMap.addOrder(stockLocate1, 3, Side::SELL, testQuantity2, testPrice2));
    REQUIRE(stockMap.addOrder(stockLocate1, 4, Side::BUY, testQuantity3, testPrice3));

    REQUIRE(!stockMap.addOrder(stockLocate1, 5, Side::BUY, testQuantity3, testPrice4));
    REQUIRE(!stockMap.addOrder(stockLocate1, 6, Side::SELL, testQuantity3, testPrice4));
    REQUIRE(!stockMap.addOrder(stockLocate1, 7, Side::BUY, testQuantity3, testPrice5));
    REQUIRE(!stockMap.addOrder(stockLocate1, 8, Side::SELL, testQuantity3, testPrice5));
}
