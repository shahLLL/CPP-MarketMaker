#pragma once
#include "types.hpp"

inline constexpr Int8 NULL_MESSAGE_SIGNAL = -1;

// Return size of message given messageType
const Int8 getMessageType(Alpha messageType);

// ITCH Parser class
class ITCHParser {
    public:
    ITCHParser() = default;
    ~ITCHParser() = default;
};