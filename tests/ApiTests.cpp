//
//
//

//=============================================================================
// Tell Catch2 to provide main()
//=============================================================================
#define CATCH_CONFIG_MAIN
#include "catch_amalgamated.hpp"

// kptodo structure (?)
#include "../src/WebDataRetriever.h"
#include "../src/SymbolContainer.h"
#include <iostream>


//=============================================================================
//=============================================================================
TEST_CASE("WebDataRetriever", "[AAPL]")
{
  WebDataRetriever w;
  SymbolContainer symbols;
  
  // Randomly selected a small data set
  w.setEndpoint("http://api.twelvedata.com");
  w.setSymbol("AAPL");
  w.setInterval("1h");
  w.setStartDate("2022-02-01 09:30:00");
  w.setEndDate("2022-02-01 15:30:00");
  w.sendRequest();
  w.parseResponse(symbols);

  // Expected results
  static constexpr int numExpectedTimestamps = 7;
  
  const auto AAPLIter = symbols.getSymbols().find("AAPL");
  const int numAAPLTimestamps = static_cast<int>(AAPLIter->second.size());
  
  REQUIRE(numAAPLTimestamps ==  numExpectedTimestamps);
}
