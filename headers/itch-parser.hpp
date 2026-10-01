#pragma once
#include <cstddef>
#include <stdexcept>
#include <utility>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include "types.hpp"

inline constexpr Int8 NULL_MESSAGE_SIGNAL = -1;
inline constexpr SizeT ITCH_INCREMENT = 2;

// Return size of message given messageType
[[nodiscard]] const Int8 getMessageType(Alpha messageType);
// Return true if valid message, false otherwise
[[nodiscard]] const Bool validMessageType(Alpha messageType);

/* Parser Helper Functions */
void endianSwap(ByteContainer& byteContainer, Byte* bytePtr, SizeT head, SizeT tail) noexcept;
void directCopy(ByteContainer& byteContainer, Byte* bytePtr, SizeT head, SizeT tail) noexcept;
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
    VoidPtr mappedData = nullptr;
    SizeT fileSize = 0;
    Byte* currentPtr = nullptr;
    Byte* endPtr = nullptr;

    public:
        explicit ITCHParser(FilePath filePath) {
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

        [[nodiscard]] const Bool hasNext() const noexcept { return currentPtr < endPtr; }

        void parseNext(ByteContainer& byteContainer) const {
            if(currentPtr >= endPtr) { throw std::runtime_error("PARSENEXT NOT POSSIBLE, FILE EMPTY"); }
            Alpha messageType = static_cast<Alpha>(*currentPtr);
            switch(messageType) {
                case 'S': parseSystemEventMessage(byteContainer, currentPtr); break;
                case 'R': parseStockDirectory(byteContainer, currentPtr); break;
                case 'H': parseStockTradingAction(byteContainer, currentPtr); break;
                case 'Y': parseRegSHORestriction(byteContainer, currentPtr); break;
                case 'L': parseMarketParticipantPosition(byteContainer, currentPtr); break;
                case 'V': parseMWCBDeclineLevelMessage(byteContainer, currentPtr); break;
                case 'W': parseMWCBStatusMessage(byteContainer, currentPtr); break;
                case 'K': parseQuotingPeriodUpdate(byteContainer, currentPtr); break;
                case 'J': parseLULDAuctionCollar(byteContainer, currentPtr); break;
                case 'h': parseOperationalHalt(byteContainer, currentPtr); break;
                case 'A': parseAddOrderMessage(byteContainer, currentPtr); break;
                case 'F': parseAddOrderMPIDAttributionMessage(byteContainer, currentPtr); break;
                case 'E': parseOrderExecutedMessage(byteContainer, currentPtr); break;
                case 'C': parseOrderExecutedWithPriceMessage(byteContainer, currentPtr); break;
                case 'X': parseOrderCancelMessage(byteContainer, currentPtr); break;
                case 'D': parseOrderDeleteMessage(byteContainer, currentPtr); break;
                case 'U': parseOrderReplaceMessage(byteContainer, currentPtr); break;
                case 'P': parseTradeMessage(byteContainer, currentPtr); break;
                case 'Q': parseCrossTradeMessage(byteContainer, currentPtr); break;
                case 'B': parseBrokenTradeMessage(byteContainer, currentPtr); break;
                case 'I': parseNOIIMessage(byteContainer, currentPtr); break;
                case 'O': parseDLWCRPD(byteContainer, currentPtr); break;
                default: throw std::runtime_error("PARSENEXT NOT POSSIBLE, UNKNOWN MESSAGE TYPE");
            }
        }

        void increment() {
            Bool incrementOnce = false;
            while(currentPtr < endPtr) {
                Alpha messageChar = static_cast<Alpha>(*currentPtr);
                if((validMessageType(messageChar)) && incrementOnce) break;
                const Int8 messageCode = getMessageType(messageChar);
                if(messageCode == NULL_MESSAGE_SIGNAL) { throw std::runtime_error("ERROR PARSING FILE"); }
                currentPtr = currentPtr + messageCode + ITCH_INCREMENT;
                incrementOnce = true;
            }
        }

        ~ITCHParser() { ::munmap(mappedData, fileSize); }

        // Non-copyable
        ITCHParser(const ITCHParser&) = delete;
        ITCHParser& operator=(const ITCHParser&) = delete;

        // Movable
        ITCHParser(ITCHParser&& other) noexcept
            : mappedData(std::exchange(other.mappedData, nullptr)),
            fileSize(std::exchange(other.fileSize, 0)),
            currentPtr(std::exchange(other.currentPtr, nullptr)),
            endPtr(std::exchange(other.endPtr, nullptr)) {}

        ITCHParser& operator=(ITCHParser&& other) noexcept {
            if (this != &other) {
                this->~ITCHParser();
                mappedData = std::exchange(other.mappedData, nullptr);
                fileSize = std::exchange(other.fileSize, 0);
                currentPtr = std::exchange(other.currentPtr, nullptr);
                endPtr = std::exchange(other.endPtr, nullptr);
            }
            return *this;
        }
};