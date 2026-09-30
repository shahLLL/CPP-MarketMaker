#pragma once
#include <cstdint>
#include <cstddef>
#include <array>

constexpr std::size_t BYTE_CONTAINER_SIZE = 50;

// Type Aliases
using Alpha = char;
using Int8 = std::int8_t;
using Int = int;
using Bool = bool;
using SizeT = std::size_t;
using Byte = std::byte;
using ByteContainer = std::array<Byte, BYTE_CONTAINER_SIZE>;
using VoidPtr = void*;
using CharPtr = char*;

// Itch Message Struct
struct ITCHMessage { Alpha messageType; Byte* data; };