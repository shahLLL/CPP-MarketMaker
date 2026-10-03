#include "../headers/stock-map.hpp"

void StockMap::addStockSymbol(const UInt16 stockLocate, const Byte* stockSymbol) noexcept {
    auto [it, inserted] = stockMap.try_emplace(stockLocate);
    std::memcpy(it->second.symbol, stockSymbol, SYMBOL_SIZE);
};

const Bool StockMap::addOrder(const UInt16 stockLocate, const UInt64 id, 
    const Side side, const UInt32 quantity, const UInt32 price) noexcept {
    return stockMap[stockLocate].orderBook.addOrder(id, side, quantity, price);
};

void StockMap::cancelOrExecuteOrder(const UInt16 stockLocate, const UInt64 id, const UInt32 quantity) noexcept {
    stockMap[stockLocate].orderBook.cancelOrExecuteOrder(id, quantity);
};

void StockMap::deleteOrder(const UInt16 stockLocate, const UInt64 id) noexcept {
    stockMap[stockLocate].orderBook.deleteOrder(id);
};

const Bool StockMap::replaceOrder(const UInt16 stockLocate, const UInt64 prevId, const UInt64 newId, 
    const UInt32 quantity, const UInt32 price) noexcept {
    return stockMap[stockLocate].orderBook.replaceOrder(prevId, newId, quantity, price);
};