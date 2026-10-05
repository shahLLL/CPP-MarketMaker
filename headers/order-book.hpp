#pragma once
#include <array>
#include <unordered_map>
#include "types.hpp"

// Constant Expressions
inline constexpr SizeT TICK_MAX = 280'000; // Max value of security in cents.
inline constexpr SizeT TICK_MIN = 50'000; // Min value of security in cents.
inline constexpr SizeT PRICE_LADDER_CAPACITY = TICK_MAX - TICK_MIN;
inline constexpr SizeT BITMAP_CAPACITY = (PRICE_LADDER_CAPACITY % 64) == 0 ? 
    (PRICE_LADDER_CAPACITY/64) : (PRICE_LADDER_CAPACITY/64) + 1;
inline constexpr Int32 NULL_CURSOR = -1;

// OrderBook Class
class OrderBook final {
    
    // OrderData Struct
    struct OrderData final {
        UInt64 price;
        UInt64 quantity;
        Side side;
    };

    // Member Variables
    std::array<UInt64, PRICE_LADDER_CAPACITY> priceLadder;
    Int32 bestBidCursor = NULL_CURSOR;
    Int32 bestAskCursor = NULL_CURSOR;
    UInt16 numberofBidOrders = 0;
    UInt16 numberofAskOrders = 0;
    UInt64 bitmap[BITMAP_CAPACITY]{};
    std::unordered_map<UInt64, OrderData> map;

    // Internal Helper Functions
    Int32 cursorSeekUp(const Int32 inputCursor) const noexcept;
    Int32 cursorSeekDown(const Int32 inputCursor) const noexcept;
    void removeFromBitmap(const Int32 inputCursor) noexcept;
    void addToBitmap(const Int32 inputCursor) noexcept;
    Bool checkBitmap(const Int32 inputCursor) noexcept;
    UInt64 getTotalBidQuantity() const noexcept;
    UInt64 getTotalAskQuantity() const noexcept;

    public:
        // Constructor & Destructor
        OrderBook() = default;
        ~OrderBook() = default;

        // Accessor Methods
        const UInt64 getQuantity(const UInt32 price);
        const Int32 getBestBid() const noexcept;
        const Int32 getBestAsk() const noexcept;
        const UInt16 getNumberOfBidOrders() const noexcept;
        const UInt16 getNumberOfAskOrders() const noexcept;
        const Double getMidPrice() const noexcept;
        const Double getMicroPrice() const noexcept;
        const Double getImbalance() const noexcept;
        

        // Book Modifyers
        const Bool addOrder(const UInt64 id, const Side side, const UInt32 quantity, const UInt32 price) noexcept;
        const Bool cancelOrExecuteOrder(const UInt64 id, const UInt32 quantity) noexcept;
        const Bool deleteOrder(const UInt64 id) noexcept;
        const Bool replaceOrder(const UInt64 prevId, const UInt64 newId, const UInt32 quantity, const UInt32 price) noexcept;
};