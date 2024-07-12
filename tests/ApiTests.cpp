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
SCENARIO("Insert into SymbolContainer")
{
  GIVEN("An empty SymbolContainer")
  {
    WebDataRetriever w;
    SymbolContainer symbolContainer;

    // Validate assumption of the GIVEN clause
    THEN("The container is empty")
    {
      REQUIRE(symbolContainer.getSymbols().size() == 0);
    }
  
    // Randomly selected a small data set (7 timestamps)
    WHEN("The container inserts a new key")
    {
      w.setEndpoint("http://api.twelvedata.com");
      w.setSymbol("AAPL");
      w.setInterval("1h");
      w.setStartDate("2022-02-01%2009:30:00");
      w.setEndDate("2022-02-01%2015:30:00");
      w.sendRequest();
      //w.parseResponse(symbolContainer);

      THEN("1 key added to container")
      {
        REQUIRE(symbolContainer.getSymbols().size() == 1);
      
        AND_THEN("The key is mapped to 7 values")
        {
          const auto AAPLIter = symbolContainer.getSymbols().find("AAPL");
          const int numAAPLTimestamps =
            static_cast<int>(AAPLIter->second.size());
          REQUIRE(numAAPLTimestamps == 7);
        }
      }
    }
  }
}
