#pragma once
#include <array>
#include <unordered_map>
#include "types.hpp"

// Constant Expressions
inline constexpr SizeT TICK_MAX = 2'000'000'000; // Max value of security in cents.
inline constexpr SizeT TICK_MIN = 0; // Min value of security in cents.
inline constexpr SizeT PRICE_LADDER_CAPACITY = TICK_MAX - TICK_MIN;
inline constexpr SizeT BITMAP_CAPACITY = (PRICE_LADDER_CAPACITY % 64) == 0 ? 
    (PRICE_LADDER_CAPACITY/64) : (PRICE_LADDER_CAPACITY/64) + 1;
inline constexpr Int32 NULL_CURSOR = -1;

// Structs
struct OrderData final {
    UInt64 price;
    UInt64 quantity;
    Side side;
};

// OrderBook Class
class OrderBook final {
    // Member Variables
    std::array<UInt64, PRICE_LADDER_CAPACITY> priceLadder;
    Int32 bestBidCursor = NULL_CURSOR;
    Int32 bestAskCursor = NULL_CURSOR;
    UInt64 bitmap[BITMAP_CAPACITY];
    std::unordered_map<UInt64, OrderData> map;

    // Internal Helper Functions
    Int32 cursorSeekUp(const Int32& inputCursor) const noexcept;
    Int32 cursorSeekDown(const Int32& inputCursor) const noexcept;
    void removeFromBitmap(const Int32& inputCursor) noexcept;
    void addToBitmap(const Int32& inputCursor) noexcept;
    bool checkBitmap(const Int32& inputCursor) noexcept;

    public:
        // Constructor & Destructor
        OrderBook() = default;
        ~OrderBook() = default;

        // Book Modifyers
        void addOrder(const UInt64& id, const Side side, const UInt32& quantity, const UInt32& price) noexcept;
        void cancelOrExecuteOrder(const UInt64& id, const UInt32& quantity) noexcept;
        void deleteOrder(const UInt64& id) noexcept;
        void replaceOrder(const UInt64& prevId, const UInt64& newId, const UInt32& quantity, const UInt32& price) noexcept;

        // Accessor Methods
        const Int32 getBestBid() const noexcept { return (bestBidCursor + TICK_MIN); }
        const Int32 getBestAsk() const noexcept { return (bestAskCursor + TICK_MIN); }
        const Int32 getMidPrice() const noexcept {
            if((bestBidCursor == NULL_CURSOR) && (bestAskCursor == NULL_CURSOR)) return 0;
            if((bestBidCursor == NULL_CURSOR) && (bestAskCursor != NULL_CURSOR)) return bestAskCursor;
            if((bestBidCursor != NULL_CURSOR) && (bestAskCursor == NULL_CURSOR)) return bestBidCursor;
            return ((bestBidCursor + TICK_MIN) + (bestAskCursor + TICK_MIN)) / 2;
        }
};