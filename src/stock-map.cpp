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

void StockMap::cancelOrExecuteOrder(const UInt16 stockLocate, const UInt64 id, const UInt32 quantity) noexcept {
    stockMap[stockLocate].orderBook.cancelOrExecuteOrder(id, quantity);
};

void StockMap::deleteOrder(const UInt16 stockLocate, const UInt64 id) noexcept {
    stockMap[stockLocate].orderBook.deleteOrder(id);
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

void StockMap::processEntry(ByteContainer& byteContainer) const noexcept {
    std::cout << static_cast<Alpha>(byteContainer[0]) << std::endl;
};