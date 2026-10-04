#pragma once
#include <unordered_map>
#include <cstring>
#include <stdexcept>
#include <vector>

#include "types.hpp"
#include "order-book.hpp"

// Constant Expressions
inline constexpr SizeT SYMBOL_SIZE = 8;
inline constexpr Alpha NULL_ALPHA = '\0';

// Symbol Data Struct
struct SymbolData final {
    const Alpha symbol[SYMBOL_SIZE + 1];
    const Int32 bestBid;
    const Int32 bestAsk;
    const Int32 midPrice;
    const UInt16 stockLocate;
};

class StockMap final {
    struct StockContainer final { Alpha symbol[SYMBOL_SIZE + 1]; OrderBook orderBook; };
    std::unordered_map<UInt16, StockContainer> stockMap;

    // Helper Functions
    Alpha extractAlpha(const Byte byte) { return static_cast<Alpha>(byte); }
    template <typename T>
    T convertBytes(const Byte* bytePtr) { 
        T rtn;
        std::memcpy(&rtn, bytePtr, sizeof(T));
        return rtn;
    }
    
    public:
        StockMap() = default;
        ~StockMap() = default;
        const Bool containsStock(const UInt16 stockLocate) noexcept;
        void addStock(const UInt16 stockLocate, const Byte* stockSymbol) noexcept;
        const Bool addOrder(const UInt16 stockLocate, const UInt64 id, const Side side, const UInt32 quantity, const UInt32 price) noexcept;
        void cancelOrExecuteOrder(const UInt16 stockLocate, const UInt64 id, const UInt32 quantity) noexcept;
        void deleteOrder(const UInt16 stockLocate, const UInt64 id) noexcept;
        const Bool replaceOrder(const UInt16 stockLocate, const UInt64 prevId, const UInt64 newId, const UInt32 quantity, const UInt32 price) noexcept;
        const std::vector<SymbolData> getPerSymbolData() const noexcept;
        void processEntry(ByteContainer& byteContainer);
};