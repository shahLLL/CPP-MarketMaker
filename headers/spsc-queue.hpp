#pragma once
#include <cstddef>
#include <atomic>
#include <new>
#include "types.hpp"
#include "itch-parser.hpp"
#include "stock-map.hpp"

// Constant Expressions
inline constexpr SizeT CACHE_LINE_SIZE = 64;

// SPSCQueue Template Class
template <SizeT exponent>
class SPSCQueue final {

    // Static Constants
    static constexpr SizeT SPSCQueueMaxBytes = SizeT{1} << 20;
    static constexpr SizeT capacity = SizeT{1} << exponent; // Capacity must be power of 2 to enusre efficent increment.
    static constexpr SizeT mask = capacity - 1;

    // Static Assertions
    static_assert(exponent < sizeof(SizeT) * 8 - 1, "SPSCQUEUE: EXPONENT CAPACITY EXCEEDED");
    static_assert(capacity * sizeof(ByteContainer) <= SPSCQueueMaxBytes, "SPSCQUEUE: CAPACITY EXCEEDED");

    // Member Variables
    alignas(CACHE_LINE_SIZE) std::atomic<SizeT> pushCursor{0};
    alignas(CACHE_LINE_SIZE) SizeT cachedPushCursor{0};
    alignas(CACHE_LINE_SIZE) std::atomic<SizeT> popCursor{0};
    alignas(CACHE_LINE_SIZE) SizeT cachedPopCursor{0};
    ByteContainer buf[capacity];

    public:
        // Constructor & Destructor
        SPSCQueue() = default;
        ~SPSCQueue() = default;

        // Methods
        const Bool enqueue(ITCHParser& itchParser) {
            SizeT pushCursorSpot = pushCursor.load(std::memory_order_relaxed);
            SizeT incrementOne = (pushCursorSpot + 1) & (mask);

            if(cachedPopCursor == incrementOne) { 
            cachedPopCursor = popCursor.load(std::memory_order_acquire);
            // Full
            if(cachedPopCursor == incrementOne) return false;
            }

            itchParser.parseNext(buf[pushCursorSpot]);
            pushCursor.store(incrementOne, std::memory_order_release);
            return true;
        }

        const Bool dequeue(StockMap& stockMap) {
            SizeT popCursorSpot = popCursor.load(std::memory_order_relaxed);

            if(cachedPushCursor == popCursorSpot) { 
                cachedPushCursor = pushCursor.load(std::memory_order_acquire);
                // Empty
                if(cachedPushCursor == popCursorSpot) return false;
            }
 
            stockMap.processEntry(buf[popCursorSpot]);
            popCursor.store((popCursorSpot + 1) & (mask), std::memory_order_release);
            return true;
        }
};