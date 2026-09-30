#pragma once
#include <cstddef>
#include <atomic>
#include <new>
#include "types.hpp"
#include "itch-parser.hpp"

template <SizeT exponent>
class SPSCQueue{
    alignas(std::hardware_destructive_interference_size) std::atomic<SizeT> pushCursor{0};
    alignas(std::hardware_destructive_interference_size) SizeT cachedPushCursor{0};
    alignas(std::hardware_destructive_interference_size) std::atomic<SizeT> popCursor{0};
    alignas(std::hardware_destructive_interference_size) SizeT cachedPopCursor{0};
    
    static constexpr SizeT SPSCQueueMaxBytes = SizeT{1} << 20;
    static constexpr SizeT capacity = SizeT{1} << exponent; // Capacity must be power of 2 to enusre efficent increment.
    static constexpr SizeT mask = capacity - 1;
    static_assert(exponent < sizeof(SizeT) * 8 - 1, "SPSCQUEUE: EXPONENT CAPACITY EXCEEDED");
    static_assert(capacity * sizeof(T) <= SPSCQueueMaxBytes, "SPSCQUEUE: CAPACITY EXCEEDED");
    ByteContainer buf[capacity];

    public:
        SPSCQueue() = default;
        ~SPSCQueue() = default;
        bool enqueue(ITCHParser& itchParser) {
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
        };

        bool dequeue(ByteContainer& popedVal) {
            SizeT popCursorSpot = popCursor.load(std::memory_order_relaxed);
            if(cachedPushCursor == popCursorSpot) { 
                cachedPushCursor = pushCursor.load(std::memory_order_acquire);
                // Empty
                if(cachedPushCursor == popCursorSpot) return false;
            } 
            popedVal = buf[popCursorSpot];
            popCursor.store((popCursorSpot + 1) & (mask), std::memory_order_release);
            return true;
        };

};