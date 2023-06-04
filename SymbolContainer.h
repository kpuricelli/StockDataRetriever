#pragma once

//
//
//

#include "StockTimeSeriesData.h"
#include <string>
#include <map>

class SymbolContainer
{
public:

  void insertSymbol(const std::string& symbol,
                    std::map<std::string, StockTimeSeriesData>& data);

  using SymbolData = std::map<std::string, StockTimeSeriesData>;
  const std::map<std::string, SymbolData>& getSymbols() const
  { return mSymbols; }

private:

  // Symbol, data
  std::map<std::string, SymbolData> mSymbols;
};
