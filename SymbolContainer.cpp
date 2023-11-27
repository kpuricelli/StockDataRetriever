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
    std::cout << "\nNo mapped data for symbol: " << symbol << ", creating"
              << std::endl;
    
    mSymbols.emplace(symbol, std::move(data));
  }
  else
  {
    std::cout << "\nEntry found for symbol: " << symbol << ", adding records"
              << std::endl;

    for (auto it = data.begin(); it != data.end(); ++it)
    {
      mSymbols[symbol].emplace(it->first, std::move(it->second));
    }
  }
}
