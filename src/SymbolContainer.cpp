#include "SymbolContainer.h"
#include "Json.hpp"
#include <utility>
#include <iostream>
#include <fstream>

//=============================================================================
//=============================================================================
namespace
{
  // Scope the namespace of the parser
  using json = nlohmann::json; 
}

//=============================================================================
//=============================================================================
void SymbolContainer::loadFromJson(const std::string& filename)
{
  // kptodo how 2 handle paths?
  std::ifstream inputFile(filename);
  if (!inputFile.is_open())
  {
    // kptodo how to handle this situation?
    std::cout << "Unable to open file: " << filename << std::endl;
    std::cout << "Returning..." << std::endl;
    return;
  }

  // Should this be in a try block (?)
  const json responseAsJson = json::parse(inputFile);

  // Something's wrong with the json file itself if it status is != ok
  const std::string status = responseAsJson["status"];
  if (status != "ok")
  {
    const std::string reason = responseAsJson["message"];
    std::cout << "\nError: API status returned: " << status
              << "\nWith message: " << reason << std::endl;
    return;
  }

  // Now that we know the file is valid, start walking through the values
  const json meta = responseAsJson["meta"];
  const std::string symbol = meta["symbol"];
  
  const json values = responseAsJson["values"];
  SymbolData valuesContainer;
  for (const auto& value : values)
  {
    const std::string timestamp = value["datetime"];
    const std::string openStr = value["open"];
    const std::string highStr = value["high"];
    const std::string lowStr = value["low"];
    const std::string closeStr = value["close"];
    const std::string volumeStr = value["volume"];

    // kptodo check precision / set (?)
    // Typecast to numbers
    const double open = std::atof(openStr.c_str());
    const double high = std::atof(highStr.c_str());
    const double low = std::atof(lowStr.c_str());
    const double close = std::atof(closeStr.c_str());
    const double volume = std::atof(volumeStr.c_str());

    // Create the time series object
    StockTimeSeriesData record(open, high, low, close, volume);

    // Save local
    valuesContainer.emplace(timestamp, record);
  }

  // Insert into mSymbols
  insertSymbol(symbol, valuesContainer);
}

//=============================================================================
//=============================================================================
void SymbolContainer::insertSymbol(const std::string& symbol, SymbolData& data)
{
  const auto& symbolsIt = mSymbols.find(symbol);

  // No key for this symbol => create
  if (symbolsIt == mSymbols.end())
  {
#if DEBUG
    std::cout << "\nNo mapped data for symbol: " << symbol << ", creating"
              << std::endl;
#endif
    
    mSymbols.emplace(symbol, std::move(data));
  }

  // Key exists => move new values into this key
  else
  {
#if DEBUG
    std::cout << "\nEntry found for symbol: " << symbol << ", adding records"
              << std::endl;
#endif
    
    for (auto it = data.begin(); it != data.end(); ++it)
    {
      mSymbols[symbol].emplace(it->first, std::move(it->second));
    }
  }
}
