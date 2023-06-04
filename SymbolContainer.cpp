#include "SymbolContainer.h"
#include <utility>
#include <iostream>

//=============================================================================
//=============================================================================
void SymbolContainer::
insertSymbol(const std::string& symbol,
             std::map<std::string, StockTimeSeriesData>& data)
{
  const auto& symbolsIt = mSymbols.find(symbol);
  if (symbolsIt == mSymbols.end())
  {
    std::cout << "No mapped data for symbol: " << symbol << ", creating"
              << std::endl;
  }
  else
  {
    std::cout << "Entry found for symbol: " << symbol << ", adding record"
              << std::endl;
  }

  mSymbols.emplace(symbol, std::move(data));
}
