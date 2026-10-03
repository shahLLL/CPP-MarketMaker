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
