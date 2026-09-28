#include "../headers/itch-parser.hpp"

[[nodiscard]] const Int8 getMessageType(Alpha messageType) {
    switch(messageType) {
        case 'S': return 12;
        case 'R': return 39;
        case 'H': return 25;
        case 'Y': return 20;
        case 'L': return 26;
        case 'V': return 35;
        case 'W': return 12;
        case 'K': return 28;
        case 'J': return 35;
        case 'h': return 21;
        case 'A': return 36;
        case 'F': return 40;
        case 'E': return 31;
        case 'C': return 36;
        case 'X': return 23;
        case 'D': return 19;
        case 'U': return 35;
        case 'P': return 44;
        case 'Q': return 40;
        case 'B': return 19;
        case 'I': return 50;
        case 'O': return 48;
        default: return NULL_MESSAGE_SIGNAL;
    }
}

void endianSwap(ByteContainer& byteContainer, Byte* bytePtr, SizeT head, SizeT tail) noexcept {
    SizeT end = tail;
    while(head <= end) {
        byteContainer[head] = *(bytePtr + tail);
        head = head + 1;
        tail = tail - 1;
    }
}

void parseSystemEventMessage(ByteContainer& byteContainer, Byte* bytePtr) noexcept {
    byteContainer[0] = *bytePtr;
    endianSwap(byteContainer, bytePtr, 1, 2);
    endianSwap(byteContainer, bytePtr, 3, 4);
    endianSwap(byteContainer, bytePtr, 5, 10);
    byteContainer[11] = *(bytePtr + 11);
}
