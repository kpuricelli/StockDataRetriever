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

  // Typing is hard...
  using SymbolData = std::map<std::string, StockTimeSeriesData>;

  // Get the entire mSymbols container
  const std::map<std::string, SymbolData>& getSymbols() const
    { return mSymbols; }

  // Get the map for a specific symbol
  const SymbolData& getSymbolData(const std::string& symbol)
    { return mSymbols[symbol]; }

  // Load SymbolData into mSymbols from json files
  void loadFromJson(const std::string& filename);
  
  // Insert a <Symbol, StockTimeSeriesData> pair into mSymbols
  void insertSymbol(const std::string& symbol, SymbolData& data);

private:

  // Container which holds all <Symbol, SymbolData> pairs
  std::map<std::string, SymbolData> mSymbols;
};
