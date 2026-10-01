#pragma once
#include <array>
#include <unordered_map>
#include "types.hpp"
#include "pool-allocator.hpp"

// Constant Expressions
inline constexpr SizeT TICK_MAX = 2'000'000'000; // Max value of security in cents.
inline constexpr SizeT TICK_MIN = 0; // Min value of security in cents.
inline constexpr SizeT PRICE_LADDER_CAPACITY = TICK_MAX - TICK_MIN;
inline constexpr SizeT BITMAP_CAPACITY = (PRICE_LADDER_CAPACITY % 64) == 0 ? 
    (PRICE_LADDER_CAPACITY/64) : (PRICE_LADDER_CAPACITY/64) + 1;
inline constexpr Int32 NULL_CURSOR = -1;

// Structs
struct Order final {
    UInt64 id;
    UInt64 price;
    UInt64 quantity;
    Order* prev;
    Order* next;
};

struct PriceLevel final {
    UInt64 quantity;
    Order* head = nullptr;
    Order* tail = nullptr;
};

struct Locator final {
    UInt64 price;
    UInt64 quantity;
    Order* node;
    Side side;
};

// OrderBook Class
class OrderBook final {
    std::array<Int32, PRICE_LADDER_CAPACITY> priceLadder;
    Int32 bestBidCursor = NULL_CURSOR;
    Int32 bestAskCursor = NULL_CURSOR;
    UInt64 bitMap[BITMAP_CAPACITY];
    std::unordered_map<UInt64, Locator> orderLocator;
    PoolAllocator<Order> orderPool;

    // Internal Helper Functions
    Int32 cursorSeekUp(const Int32& inputCursor) const noexcept;
    Int32 cursorSeekDown(const Int32& inputCursor) const noexcept;
    void removeFromBitMap(const Int32& inputCursor) noexcept;
    void addToBitMap(const Int32& inputCursor) noexcept;
    bool checkBitMap(const Int32& inputCurosr) noexcept;

    public:
        // Constructor & Destructor
        OrderBook() = default;
        ~OrderBook() = default;

        // Book Modifyers
        void addOrder();
        void executeOrder();
        void cancelOrder();
        void deleteOrder();
        void replaceOrder();

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