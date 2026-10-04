#include "../headers/itch-parser.hpp"

ITCHParser::ITCHParser(FilePath filePath) {
    int fileDescriptor = ::open(filePath, O_RDONLY);
    if (fileDescriptor == -1) { throw std::runtime_error("FAILED TO OPEN FILE"); }

    struct stat st{};
    if (::fstat(fileDescriptor, &st) == -1) {
        ::close(fileDescriptor);
        throw std::runtime_error("FSTAT FAILED");
    }
    fileSize = static_cast<SizeT>(st.st_size);

    if (fileSize == 0) {
        ::close(fileDescriptor);
        return;
    }

    mappedData = ::mmap(nullptr, fileSize, PROT_READ, MAP_PRIVATE, fileDescriptor, 0);
    if (mappedData == MAP_FAILED) {
        ::close(fileDescriptor);
        throw std::runtime_error("MMAP FAILED");
    }

    currentPtr = reinterpret_cast<Byte*>(mappedData) + ITCH_INCREMENT;
    endPtr = currentPtr + fileSize;
    increment();
   
    ::close(fileDescriptor);
};

[[nodiscard]] const Bool ITCHParser::hasNext() const noexcept { return currentPtr < endPtr; }

void ITCHParser::parseNext(ByteContainer& byteContainer) const {
    if(currentPtr >= endPtr) { throw std::runtime_error("PARSENEXT NOT POSSIBLE, FILE EMPTY"); }
    Alpha messageType = static_cast<Alpha>(*currentPtr);
    switch(messageType) {
        case 'S': ITCHParserUtil::parseSystemEventMessage(byteContainer, currentPtr); break;
        case 'R': ITCHParserUtil::parseStockDirectory(byteContainer, currentPtr); break;
        case 'H': ITCHParserUtil::parseStockTradingAction(byteContainer, currentPtr); break;
        case 'Y': ITCHParserUtil::parseRegSHORestriction(byteContainer, currentPtr); break;
        case 'L': ITCHParserUtil::parseMarketParticipantPosition(byteContainer, currentPtr); break;
        case 'V': ITCHParserUtil::parseMWCBDeclineLevelMessage(byteContainer, currentPtr); break;
        case 'W': ITCHParserUtil::parseMWCBStatusMessage(byteContainer, currentPtr); break;
        case 'K': ITCHParserUtil::parseQuotingPeriodUpdate(byteContainer, currentPtr); break;
        case 'J': ITCHParserUtil::parseLULDAuctionCollar(byteContainer, currentPtr); break;
        case 'h': ITCHParserUtil::parseOperationalHalt(byteContainer, currentPtr); break;
        case 'A': ITCHParserUtil::parseAddOrderMessage(byteContainer, currentPtr); break;
        case 'F': ITCHParserUtil::parseAddOrderMPIDAttributionMessage(byteContainer, currentPtr); break;
        case 'E': ITCHParserUtil::parseOrderExecutedMessage(byteContainer, currentPtr); break;
        case 'C': ITCHParserUtil::parseOrderExecutedWithPriceMessage(byteContainer, currentPtr); break;
        case 'X': ITCHParserUtil::parseOrderCancelMessage(byteContainer, currentPtr); break;
        case 'D': ITCHParserUtil::parseOrderDeleteMessage(byteContainer, currentPtr); break;
        case 'U': ITCHParserUtil::parseOrderReplaceMessage(byteContainer, currentPtr); break;
        case 'P': ITCHParserUtil::parseTradeMessage(byteContainer, currentPtr); break;
        case 'Q': ITCHParserUtil::parseCrossTradeMessage(byteContainer, currentPtr); break;
        case 'B': ITCHParserUtil::parseBrokenTradeMessage(byteContainer, currentPtr); break;
        case 'I': ITCHParserUtil::parseNOIIMessage(byteContainer, currentPtr); break;
        case 'O': ITCHParserUtil::parseDLWCRPD(byteContainer, currentPtr); break;
        default: throw std::runtime_error("PARSENEXT NOT POSSIBLE, UNKNOWN MESSAGE TYPE");
    }
}

void ITCHParser::increment() {
    Bool incrementOnce = false;
    while(currentPtr < endPtr) {
        Alpha messageChar = static_cast<Alpha>(*currentPtr);
        if((ITCHParserUtil::validMessageType(messageChar)) && incrementOnce) break;
        const Int8 messageCode = ITCHParserUtil::getMessageType(messageChar);
        if(messageCode == ITCHParserUtil::NULL_MESSAGE_SIGNAL) { throw std::runtime_error("ERROR PARSING FILE"); }
        currentPtr = currentPtr + messageCode + ITCH_INCREMENT;
        incrementOnce = true;
    }
}

ITCHParser::~ITCHParser() { ::munmap(mappedData, fileSize); }        

ITCHParser::ITCHParser(ITCHParser&& other) noexcept
    : mappedData(std::exchange(other.mappedData, nullptr)),
    fileSize(std::exchange(other.fileSize, 0)),
    currentPtr(std::exchange(other.currentPtr, nullptr)),
    endPtr(std::exchange(other.endPtr, nullptr)) {}

ITCHParser& ITCHParser::operator=(ITCHParser&& other) noexcept {
    if (this != &other) {
        this->~ITCHParser();
        mappedData = std::exchange(other.mappedData, nullptr);
        fileSize = std::exchange(other.fileSize, 0);
        currentPtr = std::exchange(other.currentPtr, nullptr);
        endPtr = std::exchange(other.endPtr, nullptr);
    }
    return *this;
}