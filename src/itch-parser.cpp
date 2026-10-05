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

void ITCHParser::parseNext(ByteContainer& byteContainer, Bool* isOrderBookMessage) const {
    if(currentPtr >= endPtr) { throw std::runtime_error("PARSENEXT NOT POSSIBLE, FILE EMPTY"); }
    Alpha messageType = static_cast<Alpha>(*currentPtr);
    switch(messageType) {
        case 'S': {
            if(isOrderBookMessage != nullptr) { *isOrderBookMessage = false; }
            ITCHParserUtil::parseSystemEventMessage(byteContainer, currentPtr);
            break;
        }
        case 'R': {
            if(isOrderBookMessage != nullptr) { *isOrderBookMessage = false; }
            ITCHParserUtil::parseStockDirectory(byteContainer, currentPtr); 
            break;
        }
        case 'H': {
            if(isOrderBookMessage != nullptr) { *isOrderBookMessage = false; }
            ITCHParserUtil::parseStockTradingAction(byteContainer, currentPtr);
            break;
        }
        case 'Y': {
            if(isOrderBookMessage != nullptr) { *isOrderBookMessage = false; }
            ITCHParserUtil::parseRegSHORestriction(byteContainer, currentPtr);
            break;
        }
        case 'L': {
            if(isOrderBookMessage != nullptr) { *isOrderBookMessage = false; }
            ITCHParserUtil::parseMarketParticipantPosition(byteContainer, currentPtr);
            break;
        }
        case 'V': {
            if(isOrderBookMessage != nullptr) { *isOrderBookMessage = false; }
            ITCHParserUtil::parseMWCBDeclineLevelMessage(byteContainer, currentPtr);
            break;
        }
        case 'W': {
            if(isOrderBookMessage != nullptr) { *isOrderBookMessage = false; }
            ITCHParserUtil::parseMWCBStatusMessage(byteContainer, currentPtr);
            break;
        }
        case 'K': {
            if(isOrderBookMessage != nullptr) { *isOrderBookMessage = false; }
            ITCHParserUtil::parseQuotingPeriodUpdate(byteContainer, currentPtr);
            break;
        }
        case 'J': {
            if(isOrderBookMessage != nullptr) { *isOrderBookMessage = false; }
            ITCHParserUtil::parseLULDAuctionCollar(byteContainer, currentPtr);
            break;
        }
        case 'h': {
            if(isOrderBookMessage != nullptr) { *isOrderBookMessage = false; }
            ITCHParserUtil::parseOperationalHalt(byteContainer, currentPtr);
            break;
        }
        case 'A': {
            if(isOrderBookMessage != nullptr) { *isOrderBookMessage = true; }
            ITCHParserUtil::parseAddOrderMessage(byteContainer, currentPtr);
            break;
        }
        case 'F': {
            if(isOrderBookMessage != nullptr) { *isOrderBookMessage = true; }
            ITCHParserUtil::parseAddOrderMPIDAttributionMessage(byteContainer, currentPtr);
            break;
        }
        case 'E': {
            if(isOrderBookMessage != nullptr) { *isOrderBookMessage = true; }
            ITCHParserUtil::parseOrderExecutedMessage(byteContainer, currentPtr);
            break;
        }
        case 'C': {
            if(isOrderBookMessage != nullptr) { *isOrderBookMessage = true; }
            ITCHParserUtil::parseOrderExecutedWithPriceMessage(byteContainer, currentPtr);
            break;
        }
        case 'X': {
            if(isOrderBookMessage != nullptr) { *isOrderBookMessage = true; }
            ITCHParserUtil::parseOrderCancelMessage(byteContainer, currentPtr);
            break;
        }
        case 'D': {
            if(isOrderBookMessage != nullptr) { *isOrderBookMessage = true; }
            ITCHParserUtil::parseOrderDeleteMessage(byteContainer, currentPtr);
            break;
        }
        case 'U': {
            if(isOrderBookMessage != nullptr) { *isOrderBookMessage = true; }
            ITCHParserUtil::parseOrderReplaceMessage(byteContainer, currentPtr);
            break;
        }
        case 'P': {
            if(isOrderBookMessage != nullptr) { *isOrderBookMessage = false; }
            ITCHParserUtil::parseTradeMessage(byteContainer, currentPtr);
            break;
        }
        case 'Q': {
            if(isOrderBookMessage != nullptr) { *isOrderBookMessage = false; }
            ITCHParserUtil::parseCrossTradeMessage(byteContainer, currentPtr);
            break;
        }
        case 'B': {
            if(isOrderBookMessage != nullptr) { *isOrderBookMessage = false; }
            ITCHParserUtil::parseBrokenTradeMessage(byteContainer, currentPtr);
            break;
        }
        case 'I': {
            if(isOrderBookMessage != nullptr) { *isOrderBookMessage = false; }
            ITCHParserUtil::parseNOIIMessage(byteContainer, currentPtr);
            break;
        }
        case 'O': {
            if(isOrderBookMessage != nullptr) { *isOrderBookMessage = false; }
            ITCHParserUtil::parseDLWCRPD(byteContainer, currentPtr);
            break;
        }
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