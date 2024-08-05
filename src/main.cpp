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

  // Calendar stuff
  w.setStartYear(2006);
  w.setEndYear(2007);
  w.setStartMonth(1);
  w.setEndMonth(12);
  w.setStartDay(1);

  // No set end day => do every day for the month
  // w.setEndDay(29);
  w.sendRangeOfRequests();

  return 0;
}
