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
// Expected results
//=============================================================================
static constexpr int numExpectedTimestamps = 7;

// kptodo
//void checkResponseStatus(const WebDataRetriever& webData);

//=============================================================================
//=============================================================================
TEST_CASE( "API Test", "[AAPL]" )
{
  // kptodo
  WebDataRetriever w;
  SymbolContainer symbols;
  
  // Randomly selected a small data set
  w.setEndpoint("http://api.twelvedata.com");
  w.setSymbol("AAPL");
  w.setInterval("1h");
  w.setStartDate("2022-02-01 09:30:00");
  w.setEndDate("2022-02-01 15:30:00");
  w.sendRequest();
  //checkResponseStatus(w);
  w.parseResponse(symbols);

  auto AAPLIter = symbols.getSymbols().find("AAPL");
  const auto numAAPLTimestamps = AAPLIter->second.size();
  
  REQUIRE( numAAPLTimestamps == 7 );
}
