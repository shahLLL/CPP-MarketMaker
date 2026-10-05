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
    const Double midPrice;
    const Double microPrice;
    const Double imbalance;
    const Int32 bestBid;
    const Int32 bestAsk;
    const UInt16 numberOfBidOrders;
    const UInt16 numberOfAskOrders;
    const UInt16 stockLocate;
};

// StockMap Class
class StockMap final {

    // StockContainer Struct
    struct StockContainer final { Alpha symbol[SYMBOL_SIZE + 1]; OrderBook orderBook; };

    // Member Variables
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
        // Constructor & Destructor
        StockMap() = default;
        ~StockMap() = default;

        // Methods
        const Bool containsStock(const UInt16 stockLocate) noexcept;
        void addStock(const UInt16 stockLocate, const Byte* stockSymbol) noexcept;
        const Bool addOrder(const UInt16 stockLocate, const UInt64 id, const Side side, const UInt32 quantity, const UInt32 price) noexcept;
        const Bool cancelOrExecuteOrder(const UInt16 stockLocate, const UInt64 id, const UInt32 quantity) noexcept;
        const Bool deleteOrder(const UInt16 stockLocate, const UInt64 id) noexcept;
        const Bool replaceOrder(const UInt16 stockLocate, const UInt64 prevId, const UInt64 newId, const UInt32 quantity, const UInt32 price) noexcept;
        const std::vector<SymbolData> getPerSymbolData() const noexcept;
        const Bool processEntry(ByteContainer& byteContainer, Bool* isOrderBookMessage = nullptr);
};