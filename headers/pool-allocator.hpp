#pragma once

#include <cstddef>
#include <utility>
#include <vector>

#include "types.hpp"

template <typename T>
class PoolAllocator final {
public:
    PoolAllocator() = default;

    PoolAllocator(const PoolAllocator&) = delete;
    PoolAllocator& operator=(const PoolAllocator&) = delete;
    PoolAllocator(PoolAllocator&&) = delete;
    PoolAllocator& operator=(PoolAllocator&&) = delete;

    ~PoolAllocator() { for (T* block : rawBlocks) ::operator delete(block); }

    T* allocate() {
        if (freeList.empty()) growPool();
        T* p = freeList.back();
        freeList.pop_back();
        return p;
    }

    void deallocate(T* p) noexcept {
        freeList.push_back(p);
    }

    template <typename... Args>
    T* create(Args&&... args) {
        T* p = allocate();
        new (p) T(std::forward<Args>(args)...);
        return p;
    }

    void destroy(T* p) noexcept {
        p->~T();
        deallocate(p);
    }

private:
    static constexpr SizeT kBlockGrowth = 4096;
    std::vector<T*> freeList;
    std::vector<T*> rawBlocks;

    void growPool() {
        T* block = static_cast<T*>(::operator new(kBlockGrowth * sizeof(T)));
        rawBlocks.push_back(block);
        freeList.reserve(freeList.size() + kBlockGrowth);
        for (SizeT i = 0; i < kBlockGrowth; ++i) freeList.push_back(block + i);
    }
};