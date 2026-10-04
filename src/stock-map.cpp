#include "../headers/stock-map.hpp"

const Bool StockMap::containsStock(const UInt16 stockLocate) noexcept {
    return (stockMap.find(stockLocate) != stockMap.end());
}

void StockMap::addStock(const UInt16 stockLocate, const Byte* stockSymbol) noexcept {
    auto [it, inserted] = stockMap.try_emplace(stockLocate);
    std::memcpy(it->second.symbol, stockSymbol, SYMBOL_SIZE);
};

const Bool StockMap::addOrder(const UInt16 stockLocate, const UInt64 id, 
    const Side side, const UInt32 quantity, const UInt32 price) noexcept {
    if(!containsStock(stockLocate)) return false;
    return stockMap[stockLocate].orderBook.addOrder(id, side, quantity, price);
};

const Bool StockMap::cancelOrExecuteOrder(const UInt16 stockLocate, const UInt64 id, const UInt32 quantity) noexcept {
    if(!containsStock(stockLocate)) return false;
    return stockMap[stockLocate].orderBook.cancelOrExecuteOrder(id, quantity);
};

const Bool StockMap::deleteOrder(const UInt16 stockLocate, const UInt64 id) noexcept {
    if(!containsStock(stockLocate)) return false;
    return stockMap[stockLocate].orderBook.deleteOrder(id);
};

const Bool StockMap::replaceOrder(const UInt16 stockLocate, const UInt64 prevId, const UInt64 newId, 
    const UInt32 quantity, const UInt32 price) noexcept {
    if(!containsStock(stockLocate)) return false;
    return stockMap[stockLocate].orderBook.replaceOrder(prevId, newId, quantity, price);
};

const std::vector<SymbolData> StockMap::getPerSymbolData() const noexcept {
    std::vector<SymbolData> perSymbolData;
    perSymbolData.reserve(stockMap.size());

    for (const auto& [stockLocate, stockContainer] : stockMap) {
        perSymbolData.emplace_back(
            SymbolData {
                {
                    stockContainer.symbol[0], stockContainer.symbol[1],
                    stockContainer.symbol[2], stockContainer.symbol[3],
                    stockContainer.symbol[4], stockContainer.symbol[5],
                    stockContainer.symbol[6], stockContainer.symbol[7],
                    NULL_ALPHA
                },
                stockContainer.orderBook.getBestBid(),
                stockContainer.orderBook.getBestAsk(),
                stockContainer.orderBook.getMidPrice(),
                stockLocate
            }
        );
    }

    return perSymbolData;
};

const Bool StockMap::processEntry(ByteContainer& byteContainer) {
    Alpha messageType = extractAlpha(byteContainer[0]);
    switch (messageType) {
        case 'R': {
            addStock(convertBytes<UInt16>(byteContainer.data() + 1), (byteContainer.data() + 11));
            return true;
        }

        case 'A':
        case 'F': {
            addOrder(
                convertBytes<UInt16>(byteContainer.data() + 1), 
                convertBytes<UInt64>(byteContainer.data() + 11),
                extractAlpha(byteContainer[19]) == 'B' ? Side::BUY : Side::SELL,
                convertBytes<UInt32>(byteContainer.data() + 20),
                convertBytes<UInt32>(byteContainer.data() + 32)
            );
            return true;
        }

        case 'E':
        case 'C':
        case 'X': {
            cancelOrExecuteOrder(
                convertBytes<UInt16>(byteContainer.data() + 1),
                convertBytes<UInt64>(byteContainer.data() + 11),
                convertBytes<UInt32>(byteContainer.data() + 19)
            );
            return true;
        }

        case 'D': {
            deleteOrder(
                convertBytes<UInt16>(byteContainer.data() + 1),
                convertBytes<UInt64>(byteContainer.data() + 11)
            );
            return true;
        }

        case 'U': {
            replaceOrder(
                convertBytes<UInt16>(byteContainer.data() + 1),
                convertBytes<UInt64>(byteContainer.data() + 11),
                convertBytes<UInt64>(byteContainer.data() + 19),
                convertBytes<UInt32>(byteContainer.data() + 27),
                convertBytes<UInt32>(byteContainer.data() + 31)
            );
            return true;
        }
        default: return false;
    }
};