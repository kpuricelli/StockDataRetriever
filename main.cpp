//
//
//

#include "WebDataRetriever.h"
#include "SymbolContainer.h"
#include <iostream>

static constexpr int debugPrint = 0;

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
  w.setInterval("1min");
  w.setStartDate("2023-06-01 09:30:00");
  w.setEndDate("2023-06-01 15:30:00");

  w.sendRequest();
  if (!w.responseOk())
  {
    std::cout << "Errors in http or curl response with codes: "
              << "httpcode: " << w.getHttpCode() << ", curlcode: "
              << w.getCurlCode() << std::endl;
    return 1;
  }
  w.parseResponse(symbols);

  auto sIt = symbols.getSymbols().find("AAPL");
  if (sIt == symbols.getSymbols().end())
  {
    std::cout << "No AAPL" << std::endl;
  }
  else
  {
    std::cout << "Number of timestamps: " << sIt->second.size() << std::endl;

    if (debugPrint)
    {
      for (const auto& tstamps : sIt->second)
      {
        std::cout << tstamps.first << std::endl;
        tstamps.second.debugPrint();
      }
    }
  }
  return 0;
}
