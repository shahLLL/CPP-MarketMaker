#pragma once
#include <unordered_map>
#include <cstring>

#include "types.hpp"
#include "order-book.hpp"

inline constexpr SizeT SYMBOL_SIZE = 8;

class StockMap final {
    struct StockContainer { Alpha symbol[8]{}; OrderBook orderBook; };
    std::unordered_map<UInt16, StockContainer> stockMap;
    
    public:
        StockMap() = default;
        ~StockMap() = default;
        void addStockSymbol(const UInt16 stockLocate, Byte* stockSymbol) noexcept;
        const Bool addOrder(const UInt16 stockLocate, const UInt64 id, const Side side, const UInt32 quantity, const UInt32 price) noexcept;
        void cancelOrExecuteOrder(const UInt16 stockLocate, const UInt64 id, const UInt32 quantity) noexcept;
        void deleteOrder(const UInt16 stockLocate, const UInt64 id) noexcept;
        const Bool replaceOrder(const UInt16 stockLocate, const UInt64 prevId, const UInt64 newId, const UInt32 quantity, const UInt32 price) noexcept;
};