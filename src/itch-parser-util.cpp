#include "../headers/itch-parser-util.hpp"

[[nodiscard]] const Int8 ITCHParserUtil::getMessageType(Alpha messageType) {
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
        default: return ITCHParserUtil::NULL_MESSAGE_SIGNAL;
    }
}

[[nodiscard]] const Bool ITCHParserUtil::validMessageType(Alpha messageType) {
    switch(messageType) {
        case 'S': return false;
        case 'R': return true;
        case 'H': return false;
        case 'Y': return false;
        case 'L': return false;
        case 'V': return false;
        case 'W': return false;
        case 'K': return false;
        case 'J': return false;
        case 'h': return false;
        case 'A': return true;
        case 'F': return true;
        case 'E': return true;
        case 'C': return true;
        case 'X': return true;
        case 'D': return true;
        case 'U': return true;
        case 'P': return false;
        case 'Q': return false;
        case 'B': return false;
        case 'I': return false;
        case 'O': return false;
        default: return false;
    }
}

void ITCHParserUtil::endianSwap(ByteContainer& byteContainer, Byte* bytePtr, SizeT head, SizeT tail) noexcept {
    SizeT end = tail;
    while(head <= end) {
        byteContainer[head] = *(bytePtr + tail);
        head = head + 1;
        tail = tail - 1;
    }
}

void ITCHParserUtil::directCopy(ByteContainer& byteContainer, Byte* bytePtr, SizeT head, SizeT tail) noexcept {
    for(int i = head; i <= tail; i++) { byteContainer[i] = *(bytePtr + i); }
}

void ITCHParserUtil::parseSystemEventMessage(ByteContainer& byteContainer, Byte* bytePtr) noexcept {
    byteContainer[0] = *bytePtr;
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 1, 2);
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 3, 4);
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 5, 10);
    byteContainer[11] = *(bytePtr + 11);
}

void ITCHParserUtil::parseStockDirectory(ByteContainer& byteContainer, Byte* bytePtr) noexcept {
    byteContainer[0] = *bytePtr;
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 1, 2);
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 3, 4);
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 5, 10);
    ITCHParserUtil::directCopy(byteContainer, bytePtr, 11, 20);
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 21, 24);
    ITCHParserUtil::directCopy(byteContainer, bytePtr, 25, 33);
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 34, 37);
    byteContainer[38] = *(bytePtr + 38);
}

void ITCHParserUtil::parseStockTradingAction(ByteContainer& byteContainer, Byte* bytePtr) noexcept {
    byteContainer[0] = *bytePtr;
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 1, 2);
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 3, 4);
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 5, 10);
    ITCHParserUtil::directCopy(byteContainer, bytePtr, 11, 24);
}

void ITCHParserUtil::parseRegSHORestriction(ByteContainer& byteContainer, Byte* bytePtr) noexcept {
    byteContainer[0] = *bytePtr;
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 1, 2);
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 3, 4);
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 5, 10);
    ITCHParserUtil::directCopy(byteContainer, bytePtr, 11, 19);
}

void ITCHParserUtil::parseMarketParticipantPosition(ByteContainer& byteContainer, Byte* bytePtr) noexcept {
    byteContainer[0] = *bytePtr;
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 1, 2);
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 3, 4);
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 5, 10);
    ITCHParserUtil::directCopy(byteContainer, bytePtr, 11, 25);

}

void ITCHParserUtil::parseMWCBDeclineLevelMessage(ByteContainer& byteContainer, Byte* bytePtr) noexcept {
    byteContainer[0] = *bytePtr;
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 1, 2);
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 3, 4);
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 5, 10);
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 11, 18);
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 19, 26);
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 27, 34);
}

void ITCHParserUtil::parseMWCBStatusMessage(ByteContainer& byteContainer, Byte* bytePtr) noexcept {
    byteContainer[0] = *bytePtr;
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 1, 2);
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 3, 4);
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 5, 10);
    byteContainer[11] = *(bytePtr + 11);
}

void ITCHParserUtil::parseQuotingPeriodUpdate(ByteContainer& byteContainer, Byte* bytePtr) noexcept {
    byteContainer[0] = *bytePtr;
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 1, 2);
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 3, 4);
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 5, 10);
    ITCHParserUtil::directCopy(byteContainer, bytePtr, 11, 18);
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 19, 22);
    byteContainer[23] = *(bytePtr + 23);
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 24, 27);   
}

void ITCHParserUtil::parseLULDAuctionCollar(ByteContainer& byteContainer, Byte* bytePtr) noexcept {
    byteContainer[0] = *bytePtr;
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 1, 2);
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 3, 4);
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 5, 10);
    ITCHParserUtil::directCopy(byteContainer, bytePtr, 11, 18);
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 19, 22);
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 23, 26);
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 27, 30);
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 31, 34);
}

void ITCHParserUtil::parseOperationalHalt(ByteContainer& byteContainer, Byte* bytePtr) noexcept {
    byteContainer[0] = *bytePtr;
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 1, 2);
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 3, 4);
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 5, 10);
    ITCHParserUtil::directCopy(byteContainer, bytePtr, 11, 20);
}

void ITCHParserUtil::parseAddOrderMessage(ByteContainer& byteContainer, Byte* bytePtr) noexcept {
    byteContainer[0] = *bytePtr;
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 1, 2);
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 3, 4);
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 5, 10);
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 11, 18);
    byteContainer[19] = *(bytePtr + 19);
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 20, 23);
    ITCHParserUtil::directCopy(byteContainer, bytePtr, 24, 31);
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 32, 35);
}

void ITCHParserUtil::parseAddOrderMPIDAttributionMessage(ByteContainer& byteContainer, Byte* bytePtr) noexcept {
    byteContainer[0] = *bytePtr;
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 1, 2);
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 3, 4);
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 5, 10);
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 11, 18);
    byteContainer[19] = *(bytePtr + 19);
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 20, 23);
    ITCHParserUtil::directCopy(byteContainer, bytePtr, 24, 31);
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 32, 35);
    ITCHParserUtil::directCopy(byteContainer, bytePtr, 36, 39);    
}

void ITCHParserUtil::parseOrderExecutedMessage(ByteContainer& byteContainer, Byte* bytePtr) noexcept {
    byteContainer[0] = *bytePtr;
    ITCHParserUtil:: endianSwap(byteContainer, bytePtr, 1, 2);
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 3, 4);
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 5, 10);
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 11, 18);
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 19, 22);
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 23, 30);
}

void ITCHParserUtil::parseOrderExecutedWithPriceMessage(ByteContainer& byteContainer, Byte* bytePtr) noexcept {
    byteContainer[0] = *bytePtr;
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 1, 2);
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 3, 4);
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 5, 10);
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 11, 18);
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 19, 22);
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 23, 30);
    byteContainer[31] = *(bytePtr + 31);
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 32, 35);
}

void ITCHParserUtil::parseOrderCancelMessage(ByteContainer& byteContainer, Byte* bytePtr) noexcept {
    byteContainer[0] = *bytePtr;
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 1, 2);
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 3, 4);
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 5, 10);
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 11, 18);
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 19, 22);
}

void ITCHParserUtil::parseOrderDeleteMessage(ByteContainer& byteContainer, Byte* bytePtr) noexcept {
    byteContainer[0] = *bytePtr;
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 1, 2);
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 3, 4);
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 5, 10);
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 11, 18);
}

void ITCHParserUtil::parseOrderReplaceMessage(ByteContainer& byteContainer, Byte* bytePtr) noexcept {
    byteContainer[0] = *bytePtr;
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 1, 2);
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 3, 4);
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 5, 10);
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 11, 18);
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 19, 26);
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 27, 30);
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 31, 34);
}

void ITCHParserUtil::parseTradeMessage(ByteContainer& byteContainer, Byte* bytePtr) noexcept {
    byteContainer[0] = *bytePtr;
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 1, 2);
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 3, 4);
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 5, 10);
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 11, 18);
    byteContainer[19] = *(bytePtr + 19);
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 20, 23);
    ITCHParserUtil::directCopy(byteContainer, bytePtr, 24, 31);
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 32, 35);
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 36, 43);
}

void ITCHParserUtil::parseCrossTradeMessage(ByteContainer& byteContainer, Byte* bytePtr) noexcept {
    byteContainer[0] = *bytePtr;
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 1, 2);
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 3, 4);
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 5, 10);
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 11, 18);
    ITCHParserUtil::directCopy(byteContainer, bytePtr, 19, 26);
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 27, 30);
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 31, 38);
    byteContainer[39] = *(bytePtr + 39);
}

void ITCHParserUtil::parseBrokenTradeMessage(ByteContainer& byteContainer, Byte* bytePtr) noexcept {
    byteContainer[0] = *bytePtr;
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 1, 2);
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 3, 4);
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 5, 10);
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 11, 18);
}

void ITCHParserUtil::parseNOIIMessage(ByteContainer& byteContainer, Byte* bytePtr) noexcept {
    byteContainer[0] = *bytePtr;
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 1, 2);
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 3, 4);
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 5, 10);
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 11, 18);
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 19, 26);
    byteContainer[27] = *(bytePtr + 27);
    ITCHParserUtil::directCopy(byteContainer, bytePtr, 28, 35);
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 36, 39);
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 40, 43);
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 44, 47);
    byteContainer[48] = *(bytePtr + 48);
    byteContainer[49] = *(bytePtr + 49);
}

void ITCHParserUtil::parseDLWCRPD(ByteContainer& byteContainer, Byte* bytePtr) noexcept {
    byteContainer[0] = *bytePtr;
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 1, 2);
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 3, 4);
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 5, 10);
    ITCHParserUtil::directCopy(byteContainer, bytePtr, 11, 19);
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 20, 23);
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 24, 27);
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 28, 31);
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 32, 39);
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 40, 43);
    ITCHParserUtil::endianSwap(byteContainer, bytePtr, 44, 47);
}