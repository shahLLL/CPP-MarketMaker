#include "../headers/order-book.hpp"

Int32 OrderBook::cursorSeekUp(const Int32 inputCursor) const noexcept {
    Int32 cursor = inputCursor + 1;
    if (cursor < 0 || cursor >= PRICE_LADDER_CAPACITY) return NULL_CURSOR;
    Int32 idx = cursor >> 6;
    Int32 bit = cursor & 63;

    UInt64 mask = ~0ULL << bit;
    UInt64 word = bitmap[idx] & mask;

    while (word == 0) {
        if (++idx >= BITMAP_CAPACITY) return NULL_CURSOR;
        word = bitmap[idx];
    }

    Int32 trailingZeros = __builtin_ctzll(word);
    Int32 result = (idx << 6) + trailingZeros;
    if(result >= PRICE_LADDER_CAPACITY) return NULL_CURSOR;
    return result;
}

Int32 OrderBook::cursorSeekDown(const Int32 inputCursor) const noexcept {
    Int32 cursor = inputCursor - 1;
    if (cursor < 0) return NULL_CURSOR;

    Int32 idx = cursor >> 6;
    Int32 bit = cursor & 63;
    UInt64 word = bitmap[idx] & ((bit == 63) ? ~0ULL : ((1ULL << (bit + 1)) - 1));

    while (word == 0) {
        if (--idx < 0) return NULL_CURSOR;
        word = bitmap[idx];
    }

    Int32 leadingZeros = __builtin_clzll(word);
    return (idx << 6) + (63 - leadingZeros);
}

void OrderBook::removeFromBitmap(const Int32 inputCursor) noexcept { bitmap[inputCursor >> 6] &= ~(1ULL << (inputCursor & 63)); }

void OrderBook::addToBitmap(const Int32 inputCursor) noexcept { bitmap[inputCursor >> 6] |= 1ULL << (inputCursor & 63); }

Bool OrderBook::checkBitmap(const Int32 inputCursor) noexcept{ return ((bitmap[inputCursor >> 6] & (1ULL << (inputCursor & 63))) != 0); }

void OrderBook::addOrder(const UInt64 id, const Side side, const UInt32 quantity, const UInt32 price) noexcept {
    Int32 cursor = price - TICK_MIN;
    if((side == Side::BUY) && ((bestBidCursor == NULL_CURSOR) || (cursor > bestBidCursor))) { bestBidCursor = cursor; }
    if((side == Side::SELL) && ((bestAskCursor == NULL_CURSOR) || (cursor < bestAskCursor))) { bestAskCursor = cursor; }
    if(!checkBitmap(cursor)) {
        addToBitmap(cursor);
        priceLadder[cursor] = quantity;
    } else {
        priceLadder[cursor] = priceLadder[cursor] + quantity;
    }
    map[id] = OrderData{ price, quantity, side };
}

void OrderBook::cancelOrExecuteOrder(const UInt64 id, const UInt32 quantity) noexcept {
    OrderData &orderData = map[id];
    Int32 cursor = orderData.price - TICK_MIN;
    Side side = orderData.side;
    orderData.quantity = orderData.quantity - quantity;
    priceLadder[cursor] = priceLadder[cursor] - quantity;

    if(orderData.quantity == 0) { map.erase(id); }
    if(priceLadder[cursor] == 0) {
        removeFromBitmap(cursor);
        if((side == Side::BUY) && (bestBidCursor == cursor)) { bestBidCursor = cursorSeekDown(cursor); }
        if((side == Side::SELL) && (bestAskCursor == cursor)) { bestAskCursor = cursorSeekUp(cursor); }
    }
}

void OrderBook::deleteOrder(const UInt64 id) noexcept {
    OrderData &orderData = map[id];
    Int32 cursor = orderData.price - TICK_MIN;
    Side side = orderData.side;
    priceLadder[cursor] = priceLadder[cursor] - orderData.quantity;
    
    map.erase(id);
    if(priceLadder[cursor] == 0) {
        removeFromBitmap(cursor);
        if((side == Side::BUY) && (bestBidCursor == cursor)) { bestBidCursor = cursorSeekDown(cursor); }
        if((side == Side::SELL) && (bestAskCursor == cursor)) { bestAskCursor = cursorSeekUp(cursor); }
    }
}

void OrderBook::replaceOrder(const UInt64 prevId, const UInt64 newId, const UInt32 quantity, const UInt32 price) noexcept {
    Side side = map[prevId].side;
    deleteOrder(prevId);
    addOrder(newId, side, quantity, price);
}