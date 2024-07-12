//
//
//

#include "WebDataRetriever.h"
#include "SymbolContainer.h"
#include <iostream>
#include <set>

// kptodo
#include <boost/date_time/gregorian/gregorian.hpp> //include all types plus i/o
// or
//#include "boost/date_time/gregorian/gregorian_types.hpp" //no i/o just types


// kptodo rip this out into a more formal testing structure
//static constexpr int debugPrintTimestamps = 0;
//static constexpr unsigned long numExpectedTimestamps = 7;
//static constexpr int numExpectedTimestamps2 = 10;
//static constexpr int numExpectedQQQTimestamps = 7;

//=============================================================================
// Compiler complaining about unused function params
//=============================================================================
int main(/*int argc, char* argv[]*/)
{
  // kptodo looking into boost datetime iters
  using namespace boost::gregorian;

  // kptodo year by year
  unsigned short year = 2024;

  //
  // kptodo
  // good friday..unsure how to calculate that yet (stub below)
  //
  // Market half days:
  // 3 July (1pm est)
  // 29 November (1pm est)
  // 24 December (0930 est)
  //
  
  // Holidays change year by year, so need to recalc 4 each year
  std::vector<year_based_generator*> holidays;

  //
  // Fixed holidays
  //
  
  // Western NY
  holidays.push_back(new partial_date(1, Jan));

  // Juneteenth
  holidays.push_back(new partial_date(19, Jun));

  // US Independence Day
  holidays.push_back(new partial_date(4, Jul));

  // Christmas Day
  holidays.push_back(new partial_date(25, Dec));

  //
  // Rotating holidays
  //
  
  // nth_day_of_week_in_month is way too long
  typedef nth_day_of_the_week_in_month nth_dow;
    
  // MLK Day
  holidays.push_back(new nth_dow(nth_dow::third, Monday, Jan));
  
  // President's Day
  holidays.push_back(new nth_dow(nth_dow::third, Monday, Feb));

  // kptodo good friday

  // Memorial Day
  holidays.push_back(new nth_dow(nth_dow::fourth, Monday, May));

  // US Labor Day
  holidays.push_back(new nth_dow(nth_dow::first, Monday, Sep));
  
  // Thanksgiving
  holidays.push_back(new nth_dow(nth_dow::fourth, Thursday, Nov)); 

  std::set<date> all_holidays;
    
  for(std::vector<year_based_generator*>::iterator it = holidays.begin();
      it != holidays.end(); ++it)
  {
    all_holidays.insert((*it)->get_date(year));
  }

  // kptodo rm
  for (std::set<date>::iterator it = all_holidays.begin();
       it != all_holidays.end(); ++it)
    std::cout << to_iso_extended_string(*it) << std::endl;
    
  std::cout << "Number Holidays: " << all_holidays.size() << std::endl;

#if 0
  // For each month
  for (unsigned short month = 1; month <= 12; ++month)
  {
    // Use the calendar to get the last day of the month
    auto eom_day = gregorian_calendar::end_of_month_day(year, month);
    date endOfMonth(year, month, eom_day);

    // For each day
    day_iterator ditr(date(year, month, 1));
    for (; ditr <= endOfMonth; ++ditr)
    {
      // Skip all weekends
      if ((*ditr).day_of_week() == Saturday || (*ditr).day_of_week() == Sunday)
	continue;

      
      std::cout << to_iso_extended_string(*ditr) << std::endl;
    }
  }
#endif
  // kptodo
  // do this after date time stuff
  // move all this to some testing file
#if 0
  WebDataRetriever w;
  SymbolContainer symbols;
  
  // Randomly selected a small data set
  w.setEndpoint("http://api.twelvedata.com");
  w.setSymbol("AAPL");
  w.setInterval("1h");
  w.setStartDate("2022-02-01%2009:30:00");
  w.setEndDate("2022-02-01%2015:30:00");
  w.sendRequest();
  std::string filename;
  w.getFileName(filename);
  //w.writeResponse2File();
  //w.parseResponse(symbols);
#endif
  
  // kptodo
#if 0
  
  // First test: insert symbol timestamps into empty container
  auto AAPLIter = symbols.getSymbols().find("AAPL");
  const auto numAAPLTimestamps = AAPLIter->second.size();
  std::cout << "\nTest 1: inserting new symbol with 7 timestamps"
            << std::endl;
  std::cout << "Expected: " << numExpectedTimestamps << std::endl;
  std::cout << "Parsed: " << numAAPLTimestamps << std::endl;

  if (numAAPLTimestamps != numExpectedTimestamps)
    std::cout << "Test result: Fail!" << std::endl;
  else
    std::cout << "Test result: Pass!" << std::endl;
  
  // Randomly selected another small data set (7 days later; 3 timestamps)
  w.setStartDate("2022-02-08 09:30:00");
  w.setEndDate("2022-02-08 11:30:00");
  w.sendRequest();
  w.parseResponse(symbols);

  // Second test: insert same symbol into non-empty map
  AAPLIter = symbols.getSymbols().find("AAPL");
  const int numAAPLTimestamps2 = AAPLIter->second.size();
  std::cout << "\nTest 2: inserting duplicate symbol with 3 new timestamps"
            << std::endl;
  std::cout << "Expected: " << numExpectedTimestamps2 << std::endl;
  std::cout << "Parsed: " << numAAPLTimestamps2 << std::endl;

  if (numAAPLTimestamps2 != numExpectedTimestamps2)
    std::cout << "Test result: Fail!" << std::endl;
  else
    std::cout << "Test result: Pass!" << std::endl;

  // Randomly selected another small data set (5 timestamps)
  w.setSymbol("QQQ");
  w.setStartDate("2023-09-11 09:30:00");
  w.setEndDate("2023-09-11 15:30:00");
  w.sendRequest();
  w.parseResponse(symbols);

  // Third test: insert a different symbol into empty map
  auto QQQIter = symbols.getSymbols().find("QQQ");
  const int numQQQTimestamps = QQQIter->second.size();
  std::cout << "\nTest 3: inserting 1 new symbol with 7 timestamps"
            << std::endl;
  std::cout << "Expected: " << numExpectedQQQTimestamps << std::endl;
  std::cout << "Parsed: " << numQQQTimestamps << std::endl;

  if (numQQQTimestamps != numExpectedQQQTimestamps)
    std::cout << "Test result: Fail!" << std::endl;
  else
    std::cout << "Test result: Pass!" << std::endl;


  
  
  // kptodo create a counting machine
  for (int day = 7; day <= 14; day+=7)
  {
    // kptodo upd8 enddate
    std::string endDate = "2022-01-" + std::to_string(day) + " 15:30:00";
    w.setEndDate(endDate);

    // kptodo rm
    std::cout << endDate << std::endl;
    w.sendRequest();
    w.parseResponse(symbols);
  }

  // Some extra debugging stuff to compare the timestamps themselves
  if (debugPrintTimestamps)
  {
    // AAPL
    std::cout << "\nAAPL Timestamps" << std::endl;
    AAPLIter = symbols.getSymbols().find("AAPL");
    if (AAPLIter == symbols.getSymbols().end())
    {
      std::cout << "No AAPL" << std::endl;
    }
    else
    {
      std::cout << "Number of timestamps: "
                << AAPLIter->second.size() << std::endl;

      for (const auto& tstamps : AAPLIter->second)
      {
        std::cout << tstamps.first << std::endl;
        tstamps.second.debugPrint();
      }
    }

    std::cout << std::endl;
    
    // QQQ
    std::cout << "\nQQQ Timestamps" << std::endl;
    QQQIter = symbols.getSymbols().find("QQQ");
    if (QQQIter == symbols.getSymbols().end())
    {
      std::cout << "No QQQ" << std::endl;
    }
    else
    {
      std::cout << "Number of timestamps: "
                << QQQIter->second.size() << std::endl;

      for (const auto& tstamps : QQQIter->second)
      {
        std::cout << tstamps.first << std::endl;
        tstamps.second.debugPrint();
      }
    }

  }

  // kptodo rm
  std::cout << std::endl;
#endif
  return 0;
}
