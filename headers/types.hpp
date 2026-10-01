#pragma once
#include <cstdint>
#include <cstddef>
#include <array>

constexpr std::size_t BYTE_CONTAINER_SIZE = 50;

// Type Aliases
using Alpha = char;
using Int = int;
using Int8 = std::int8_t;
using Int32 = std::uint32_t;
using UInt64 = std::uint64_t;
using Bool = bool;
using SizeT = std::size_t;
using Byte = std::byte;
using ByteContainer = std::array<Byte, BYTE_CONTAINER_SIZE>;
using VoidPtr = void*;
using FilePath = const char*;

// Enums
enum class Side : Bool { BUY, SELL };