#pragma once
#include "types.hpp"

inline constexpr Int8 NULL_MESSAGE_SIGNAL = -1;

// Return size of message given messageType
[[nodiscard]] const Int8 getMessageType(Alpha messageType);

/* Parser Helper Functions */

void parseSystemEventMessage(ByteContainer& byteContainer, Byte* bytePtr) noexcept;
void parseStockDirectory(ByteContainer& byteContainer, Byte* bytePtr) noexcept;
void parseStockTradingAction(ByteContainer& byteContainer, Byte* bytePtr) noexcept;
void parseRegSHORestriction(ByteContainer& byteContainer, Byte* bytePtr) noexcept;
void parseMarketParticipantPosition(ByteContainer& byteContainer, Byte* bytePtr) noexcept;
void parseMWCBDeclineLevelMessage(ByteContainer& byteContainer, Byte* bytePtr) noexcept;
void parseMWCBStatusMessage(ByteContainer& byteContainer, Byte* bytePtr) noexcept;
void parseQuotingPeriodUpdate(ByteContainer& byteContainer, Byte* bytePtr) noexcept;
void parseLULDAuctionCollar(ByteContainer& byteContainer, Byte* bytePtr) noexcept;
void parseOperationalHalt(ByteContainer& byteContainer, Byte* bytePtr) noexcept;
void parseAddOrderMessage(ByteContainer& byteContainer, Byte* bytePtr) noexcept;
void parseAddOrderMPIDAttributionMessage(ByteContainer& byteContainer, Byte* bytePtr) noexcept;
void parseOrderExecutedMessage(ByteContainer& byteContainer, Byte* bytePtr) noexcept;
void parseOrderExecutedWithPriceMessage(ByteContainer& byteContainer, Byte* bytePtr) noexcept;
void parseOrderCancelMessage(ByteContainer& byteContainer, Byte* bytePtr) noexcept;
void parseOrderDeleteMessage(ByteContainer& byteContainer, Byte* bytePtr) noexcept;
void parseOrderReplaceMessage(ByteContainer& byteContainer, Byte* bytePtr) noexcept;
void parseTradeMessage(ByteContainer& byteContainer, Byte* bytePtr) noexcept;
void parseCrossTradeMessage(ByteContainer& byteContainer, Byte* bytePtr) noexcept;
void parseBrokenTradeMessage(ByteContainer& byteContainer, Byte* bytePtr) noexcept;
void parseNOIIMessage(ByteContainer& byteContainer, Byte* bytePtr) noexcept;
void parseDLWCRPD(ByteContainer& byteContainer, Byte* bytePtr) noexcept;

// ITCH Parser class
class ITCHParser {
    public:
    ITCHParser() = default;
    ~ITCHParser() = default;
};