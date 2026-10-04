#pragma once
#include <cstddef>
#include <stdexcept>
#include <utility>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include "types.hpp"
#include "itch-parser-util.hpp"

// Constant Expressions
inline constexpr SizeT ITCH_INCREMENT = 2;

// ITCH Parser Class
class ITCHParser final {

    // Member Variables
    VoidPtr mappedData = nullptr;
    SizeT fileSize = 0;
    Byte* currentPtr = nullptr;
    Byte* endPtr = nullptr;

    public:
        // Explicit Constructor & Destructor
        ITCHParser(FilePath filePath);
         ~ITCHParser();

        // Methods
        [[nodiscard]] const Bool hasNext() const noexcept;
        void parseNext(ByteContainer& byteContainer) const;
        void increment();

        // Non-copyable
        ITCHParser(const ITCHParser&) = delete;
        ITCHParser& operator=(const ITCHParser&) = delete;

        // Movable
        ITCHParser(ITCHParser&& other) noexcept;
        ITCHParser& operator=(ITCHParser&& other) noexcept;
};