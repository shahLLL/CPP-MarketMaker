#include <catch2/catch_test_macros.hpp>
#include "../headers/stock-map.hpp"

const SymbolData symbolDataFromStockLocate(const std::vector<SymbolData> symbolDataVec, const UInt16 stockLocate) {
    for(auto& symbolData: symbolDataVec) { if(symbolData.stockLocate == stockLocate) return symbolData; }
    throw std::runtime_error("NO MATCHING STOCK LOCATE");
}

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

TEST_CASE("STOCKMAP TESTCASE #3", "[replaceOrder]") {
    const UInt16 stockLocate1 = 1;
    UInt32 testPriceOld1 = 50'020;
    UInt32 testQuantityOld1 = 12;
    UInt32 testPriceNew1 = 50'040;
    UInt32 testQuantityNew1 = 40;
    UInt32 testPriceOld2 = 50'080;
    UInt32 testQuantityOld2 = 42;
    UInt32 testPriceNew2 = 50'090;
    UInt32 testQuantityNew2 = 14;

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
    stockMap.addStock(stockLocate1, stockSymbol1);

    REQUIRE(stockMap.addOrder(stockLocate1, 1, Side::BUY, testQuantityOld1, testPriceOld1));
    REQUIRE(stockMap.replaceOrder(stockLocate1, 1, 3, testQuantityNew1, testPriceNew1));
    REQUIRE(!stockMap.replaceOrder(stockLocate1, 3, 5, testQuantityNew1, 19'000));
    REQUIRE(!stockMap.replaceOrder(stockLocate1, 3, 5, testQuantityNew1, 419'000));

    REQUIRE(stockMap.addOrder(stockLocate1, 2, Side::SELL, testQuantityOld2, testPriceOld2));
    REQUIRE(stockMap.replaceOrder(stockLocate1, 2, 4, testQuantityNew2, testPriceNew2));
    REQUIRE(!stockMap.replaceOrder(stockLocate1, 4, 6, testQuantityNew2, 10'000));
    REQUIRE(!stockMap.replaceOrder(stockLocate1, 4, 6, testQuantityNew2, 400'000));
}

TEST_CASE("STOCKMAP TESTCASE #4", "[getPerSymbolData]") {
    const UInt16 stockLocate1 = 1;
    const UInt16 stockLocate2 = 2;
    const UInt16 stockLocate3 = 3;
    const UInt32 price1 = 50'500;
    const UInt32 price2 = 50'100;

    const Byte stockSymbol1[] = { Byte{'T'}, Byte{'S'}, Byte{'L'}, Byte{'A'},
                              Byte{' '}, Byte{' '}, Byte{' '}, Byte{' '} };
    const Byte stockSymbol2[] = { Byte{'M'}, Byte{'S'}, Byte{'F'}, Byte{'T'},
                              Byte{' '}, Byte{' '}, Byte{' '}, Byte{' '} };
    const Byte stockSymbol3[] = { Byte{'A'}, Byte{'M'}, Byte{'Z'}, Byte{'N'},
                              Byte{' '}, Byte{' '}, Byte{' '}, Byte{' '} };

    StockMap stockMap = StockMap();
    stockMap.addStock(stockLocate1, stockSymbol1);
    stockMap.addStock(stockLocate2, stockSymbol2);
    stockMap.addStock(stockLocate3, stockSymbol3);

    REQUIRE(stockMap.addOrder(stockLocate1, 1, Side::SELL, 1, price1));
    REQUIRE(stockMap.addOrder(stockLocate2, 2, Side::BUY, 4, price2));
    REQUIRE(stockMap.addOrder(stockLocate3, 3, Side::SELL, 1, price1));
    REQUIRE(stockMap.addOrder(stockLocate3, 4, Side::BUY, 4, price2));

    std::vector<SymbolData> result = stockMap.getPerSymbolData();
    REQUIRE(result.size() == 3);
    SymbolData symbolData1 = symbolDataFromStockLocate(result, stockLocate1);
    SymbolData symbolData2 = symbolDataFromStockLocate(result, stockLocate2);
    SymbolData symbolData3 = symbolDataFromStockLocate(result, stockLocate3);

    REQUIRE(std::string_view(symbolData1.symbol, SYMBOL_SIZE) == "TSLA    ");
    REQUIRE(symbolData1.bestBid == NULL_CURSOR);
    REQUIRE(symbolData1.bestAsk == price1);
    REQUIRE(symbolData1.midPrice == price1);
    REQUIRE(std::string_view(symbolData2.symbol, SYMBOL_SIZE) == "MSFT    ");
    REQUIRE(symbolData2.bestBid == price2);
    REQUIRE(symbolData2.bestAsk == NULL_CURSOR);
    REQUIRE(symbolData2.midPrice == price2);
    REQUIRE(std::string_view(symbolData3.symbol, SYMBOL_SIZE) == "AMZN    ");
    REQUIRE(symbolData3.bestBid == price2);
    REQUIRE(symbolData3.bestAsk == price1);
    REQUIRE(symbolData3.midPrice == (price1 + price2)/2);
}
