//
//
//

#include "WebDataRetriever.h"
#include "SymbolContainer.h"
#include <iostream>
#include <set>

//=============================================================================
// Compiler complaining about unused function params
//=============================================================================
int main(/*int argc, char* argv[]*/)
{
  WebDataRetriever w;
  SymbolContainer symbols;
  
  // kptodo endpoint probably doesn't need 2b set
  w.setEndpoint("http://api.twelvedata.com");
  w.setSymbol("AAPL");
  w.setInterval("1day");

  // Year
  w.setStartYear(2010);
  w.setEndYear(2010);

  // Month
  w.setStartMonth(1);
  w.setEndMonth(12);

  // Day (no set end day => dedeuce how many days are in this month and do all)
  w.setStartDay(1);

  // Send all the requests for the given date ranges
  w.sendRangeOfRequests();

  return 0;
}
