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

const UInt64 OrderBook::getQuantity(const UInt32 price) { 
    if(checkBitmap(price - TICK_MIN)) { return priceLadder[price - TICK_MIN]; }
    return 0;
}

const Int32 OrderBook::getBestBid() const noexcept { 
    if(bestBidCursor == NULL_CURSOR) { return NULL_CURSOR; }
    return (bestBidCursor + TICK_MIN); 
}

const Int32 OrderBook::getBestAsk() const noexcept { 
    if(bestAskCursor == NULL_CURSOR) { return NULL_CURSOR; }
    return (bestAskCursor + TICK_MIN); 
}
        
const Int32 OrderBook::getMidPrice() const noexcept {
    if((bestBidCursor == NULL_CURSOR) && (bestAskCursor == NULL_CURSOR)) return NULL_CURSOR;
    if((bestBidCursor == NULL_CURSOR) && (bestAskCursor != NULL_CURSOR)) return bestAskCursor + TICK_MIN;
    if((bestBidCursor != NULL_CURSOR) && (bestAskCursor == NULL_CURSOR)) return bestBidCursor + TICK_MIN;
    return ((bestBidCursor + TICK_MIN) + (bestAskCursor + TICK_MIN)) / 2;
}

const UInt16 OrderBook::getNumberOfBidOrders() const noexcept { return numberofBidOrders; }
const UInt16 OrderBook::getNumberOfAskOrders() const noexcept { return numberofAskOrders; }

const Bool OrderBook::addOrder(const UInt64 id, const Side side, const UInt32 quantity, const UInt32 price) noexcept {
    if((price < TICK_MIN) || (price >= TICK_MAX)) return false;
    Int32 cursor = price - TICK_MIN;

    if((side == Side::BUY) && ((bestBidCursor == NULL_CURSOR) || (cursor > bestBidCursor))) { bestBidCursor = cursor; }
    if((side == Side::SELL) && ((bestAskCursor == NULL_CURSOR) || (cursor < bestAskCursor))) { bestAskCursor = cursor; }
    if(!checkBitmap(cursor)) {
        addToBitmap(cursor);
        priceLadder[cursor] = quantity;
    } else {
        priceLadder[cursor] = priceLadder[cursor] + quantity;
    }
    if(side == Side::BUY) { numberofBidOrders = numberofBidOrders + 1; }
    else { numberofAskOrders = numberofAskOrders + 1; }

    map[id] = OrderData{ price, quantity, side };
    return true;
}

const Bool OrderBook::cancelOrExecuteOrder(const UInt64 id, const UInt32 quantity) noexcept {
    if(map.find(id) == map.end()) return false;
    OrderData &orderData = map[id];
    Int32 cursor = orderData.price - TICK_MIN;
    Side side = orderData.side;

    if((orderData.quantity < quantity) || (priceLadder[cursor] < quantity)) return false;
    orderData.quantity = orderData.quantity - quantity;
    priceLadder[cursor] = priceLadder[cursor] - quantity;

    if(orderData.quantity == 0) { 
        map.erase(id);
        if(side == Side::BUY) { numberofBidOrders = numberofBidOrders - 1; }
        else { numberofAskOrders = numberofAskOrders - 1; }
    }

    if(priceLadder[cursor] == 0) {
        removeFromBitmap(cursor);
        if((side == Side::BUY) && (bestBidCursor == cursor)) { bestBidCursor = cursorSeekDown(cursor); }
        if((side == Side::SELL) && (bestAskCursor == cursor)) { bestAskCursor = cursorSeekUp(cursor); }
    }

    return true;
}

const Bool OrderBook::deleteOrder(const UInt64 id) noexcept {
    if(map.find(id) == map.end()) return false;
    OrderData &orderData = map[id];
    Int32 cursor = orderData.price - TICK_MIN;
    Side side = orderData.side;

    priceLadder[cursor] = priceLadder[cursor] - orderData.quantity;
    map.erase(id);

    if(side == Side::BUY) { numberofBidOrders = numberofBidOrders - 1; }
    else { numberofAskOrders = numberofAskOrders - 1; }
    if(priceLadder[cursor] == 0) {
        removeFromBitmap(cursor);
        if((side == Side::BUY) && (bestBidCursor == cursor)) { bestBidCursor = cursorSeekDown(cursor); }
        if((side == Side::SELL) && (bestAskCursor == cursor)) { bestAskCursor = cursorSeekUp(cursor); }
    }

    return true;
}

const Bool OrderBook::replaceOrder(const UInt64 prevId, const UInt64 newId, const UInt32 quantity, const UInt32 price) noexcept {
    if((price < TICK_MIN) || (price >= TICK_MAX)) return false;
    if(map.find(prevId) == map.end()) return false;
    Side side = map[prevId].side;
    deleteOrder(prevId);
    addOrder(newId, side, quantity, price);
    return true;
}