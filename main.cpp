//
//
//

#include "WebDataRetriever.h"
#include "SymbolContainer.h"
#include <iostream>

//=============================================================================
//=============================================================================
int main(int argc, char* argv[])
{
  // kptodo makefile
  // g++ -std=c++11 -Wall WebDataRetriever.cpp StockTimeSeriesData.cpp
  // SymbolContainer.cpp main.cpp -lcurl
  
  WebDataRetriever w;
  SymbolContainer symbols;
  
  // Randomly selected a small data set
  w.setEndpoint("http://api.twelvedata.com");
  w.setSymbol("AAPL");
  w.setInterval("1h");
  w.setStartDate("2023-06-01 15:05:00");
  w.setEndDate("2023-06-02 15:05:00");

  w.sendRequest();
  // kptodo if ok
  w.parseResponse(symbols);

  // kptodo rm
  auto sIt = symbols.getSymbols().find("AAPL");
  if (sIt == symbols.getSymbols().end())
  {
    std::cout << "No AAPL" << std::endl;
  }
  else
  {
    std::cout << "Found AAPL" << std::endl;
    std::cout << "Number of timestamps: " << sIt->second.size() << std::endl;
    for (const auto& tstamps : sIt->second)
    {
      std::cout << tstamps.first << std::endl;
      tstamps.second.debugPrint();
    }
  }
  return 0;
}
