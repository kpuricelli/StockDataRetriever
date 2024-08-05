//
//
//

//=============================================================================
// Tell Catch2 to provide main()
//=============================================================================
#define CATCH_CONFIG_MAIN
#include "catch_amalgamated.hpp"
#include "../src/Calendar.h"
#include "../src/SymbolContainer.h"
#include "../src/StockTimeSeriesData.h"

//=============================================================================
// Test case: Calendar default constructor should do nothing
//=============================================================================
TEST_CASE("Calendar", "[DefaultConstructor]")
{
  Calendar calendar;

  // Validate the assumption that the Calendar constructor default initalizes
  // it's internal dates to 0
  REQUIRE(!calendar.isStartYearSet());
  REQUIRE(!calendar.isEndYearSet());  
  REQUIRE(!calendar.isStartMonthSet());
  REQUIRE(!calendar.isEndMonthSet());
  REQUIRE(!calendar.isStartDaySet());
  REQUIRE(!calendar.isEndDaySet());

  //===========================================================================
  // Section: fixed market holidays
  //===========================================================================
  SECTION("Calendar should insert 0 market holidays with no provided year")
  {
    REQUIRE(calendar.getNumberOfFixedMarketHolidays() == 0);
  }
}

//=============================================================================
// Test case: Calendar generates correct holidays for given year: 2023
//=============================================================================
TEST_CASE("Calendar", "[GetMarketHolidaysForYear:2023]")
{
  using namespace boost::gregorian;
  
  // Year: 2023
  const unsigned short startYear = 2023;
  const unsigned short endYear = 2023;

  // Month: 12
  const unsigned short startMonth = 12;
  const unsigned short endMonth = 12;

  // Day: 1
  const unsigned short startDay = 1;
  const unsigned short endDay = 1;

  Calendar calendar(startYear, endYear, startMonth, endMonth, startDay, endDay);

  // Validate the assumption that the Calendar constructor initalizes its
  // internal dates to be non-default constructed
  REQUIRE(calendar.isStartYearSet());
  REQUIRE(calendar.isEndYearSet());  
  REQUIRE(calendar.isStartMonthSet());
  REQUIRE(calendar.isEndMonthSet());
  REQUIRE(calendar.isStartDaySet());
  REQUIRE(calendar.isEndDaySet());

  // Validate the assumption that the Calendar constructor set the correct
  // dates given to the constructor
  REQUIRE(calendar.getStartYear() == startYear);
  REQUIRE(calendar.getEndYear() == endYear);  
  REQUIRE(calendar.getStartMonth() == startMonth);
  REQUIRE(calendar.getEndMonth() == endMonth);
  REQUIRE(calendar.getStartDay() == startDay);
  REQUIRE(calendar.getEndDay() == endDay);
  
  //===========================================================================
  // Section: Check the holiday dates based on a given year
  //===========================================================================
  SECTION("Calendar should provide market holiday dates for given year: 2023")
  {
    // Fill the std::set with this year's holidays
    calendar.addAllMarketHolidays(startYear);

    // Grab the std::set with this year's holidays
    const auto& marketHolidaysForYear = calendar.getMarketHolidaysForYear();

    // Market holidays for the year 2023:
    
    // 2023-01-02 (New Year's Day) (observed)
    const date newYearsDay = from_string("2023-01-02");
    // 2023-01-16 (MLK Day)
    const date mlkDay = from_string("2023-01-16");
    // 2023-02-20 (President's Day)
    const date presidentsDay = from_string("2023-02-20");
    // 2023-04-07 (Good Friday)
    const date goodFriday = from_string("2023-04-07");
    // 2023-05-29 (Memorial Day)
    const date memorialDay = from_string("2023-05-29");
    // 2023-06-19 (Juneteenth)
    const date juneteenth = from_string("2023-06-19");
    // 2023-07-04 (US Independence Day)
    const date usIndependenceDay = from_string("2023-07-04");
    // 2023-09-04 (Labor Day)
    const date laborDay = from_string("2023-09-04");
    // 2023-11-23 (Thanksgiving Day)
    const date thanksgivingDay = from_string("2023-11-23");
    // 2023-12-25 (Christmas Day)
    const date christmasDay = from_string("2023-12-25");
    
    // Assert the market holidays for the given year
    REQUIRE(marketHolidaysForYear.contains(newYearsDay));
    REQUIRE(marketHolidaysForYear.contains(mlkDay));
    REQUIRE(marketHolidaysForYear.contains(presidentsDay));
    REQUIRE(marketHolidaysForYear.contains(goodFriday)); 
    REQUIRE(marketHolidaysForYear.contains(memorialDay));
    REQUIRE(marketHolidaysForYear.contains(juneteenth));
    REQUIRE(marketHolidaysForYear.contains(usIndependenceDay));
    REQUIRE(marketHolidaysForYear.contains(laborDay));
    REQUIRE(marketHolidaysForYear.contains(thanksgivingDay));
    REQUIRE(marketHolidaysForYear.contains(christmasDay));

    // Should be 10 holidays (includes Juneteenth)
    REQUIRE(marketHolidaysForYear.size() == 10);
  }
}

//=============================================================================
// Test case: Calendar generates correct holidays for given year: 2020
// Note: 2020 was chosen because there are only 4 mondays in May
//       (checking Memorial Day), and
//       4 July falls on a Saturday, so observed day is 3 July
//=============================================================================
TEST_CASE("Calendar", "[GetMarketHolidaysForYear:2020]")
{
  using namespace boost::gregorian;
  
  // Year: 2020
  const unsigned short startYear = 2020;
  const unsigned short endYear = 2020;

  // Month: 12
  const unsigned short startMonth = 12;
  const unsigned short endMonth = 12;

  // Day: 1
  const unsigned short startDay = 1;
  const unsigned short endDay = 1;

  Calendar calendar(startYear, endYear, startMonth, endMonth, startDay, endDay);

  // Validate the assumption that the Calendar constructor initalizes its
  // internal dates to be non-default constructed
  REQUIRE(calendar.isStartYearSet());
  REQUIRE(calendar.isEndYearSet());  
  REQUIRE(calendar.isStartMonthSet());
  REQUIRE(calendar.isEndMonthSet());
  REQUIRE(calendar.isStartDaySet());
  REQUIRE(calendar.isEndDaySet());

  // Validate the assumption that the Calendar constructor set the correct
  // dates given to the constructor
  REQUIRE(calendar.getStartYear() == startYear);
  REQUIRE(calendar.getEndYear() == endYear);  
  REQUIRE(calendar.getStartMonth() == startMonth);
  REQUIRE(calendar.getEndMonth() == endMonth);
  REQUIRE(calendar.getStartDay() == startDay);
  REQUIRE(calendar.getEndDay() == endDay);
  
  //===========================================================================
  // Section: Check the holiday dates based on a given year
  //===========================================================================
  SECTION("Calendar should provide market holiday dates for year: 2020")
  {
    // Fill the std::set with this year's holidays
    calendar.addAllMarketHolidays(startYear);

    // Grab the std::set with this year's holidays
    const auto& marketHolidaysForYear = calendar.getMarketHolidaysForYear();

    // Market holidays for the year 2020:
    
    // 2020-01-01 (New Year's Day)
    const date newYearsDay = from_string("2020-01-01");
    // 2020-01-20 (MLK Day)
    const date mlkDay = from_string("2020-01-20");
    // 2020-02-17 (President's Day)
    const date presidentsDay = from_string("2020-02-17");
    // 2023-04-07 (Good Friday)
    const date goodFriday = from_string("2020-04-10");
    // 2020-05-25 (Memorial Day)
    const date memorialDay = from_string("2020-05-25");
    // 2020-07-03 (US Independence Day) (observed)
    const date usIndependenceDay = from_string("2020-07-03");
    // 2020-09-07 (Labor Day)
    const date laborDay = from_string("2020-09-07");
    // 2020-11-26 (Thanksgiving Day)
    const date thanksgivingDay = from_string("2020-11-26");
    // 2020-12-25 (Christmas Day)
    const date christmasDay = from_string("2020-12-25");
    
    // Assert the market holidays for the given year
    REQUIRE(marketHolidaysForYear.contains(newYearsDay));
    REQUIRE(marketHolidaysForYear.contains(mlkDay));
    REQUIRE(marketHolidaysForYear.contains(presidentsDay));
    REQUIRE(marketHolidaysForYear.contains(goodFriday));    
    REQUIRE(marketHolidaysForYear.contains(memorialDay));
    REQUIRE(marketHolidaysForYear.contains(usIndependenceDay));
    REQUIRE(marketHolidaysForYear.contains(laborDay));
    REQUIRE(marketHolidaysForYear.contains(thanksgivingDay));
    REQUIRE(marketHolidaysForYear.contains(christmasDay));

    // Should be 9 holidays (no Juneteenth)
    REQUIRE(marketHolidaysForYear.size() == 9);
  }
}


// kptodo
// this is probably a more feasible test if extracted into a timing class
//=============================================================================
// Test case: Ensure we don't send more requests / min than 8 (API time limit)
//=============================================================================
// TEST_CASE("WebDataRetriever", "[SendRangeOfRequests]")


//=============================================================================
// Test case: ensure we load the json data into the containers correctly
//=============================================================================
TEST_CASE("SymbolContainer", "[loadSymbolsFromJson]")
{
  SymbolContainer symbols;

  // Validate the assumption that the SymbolContainer constructor default
  // initalizes its internal map to empty
  const std::map<std::string, SymbolContainer::SymbolData> symbolData =
    symbols.getSymbols();
  REQUIRE(symbolData.empty());


  //===========================================================================
  // Section: insert new symbol
  //===========================================================================
  SECTION("SymbolContainer should read the given json file (1h interval)")
  {
    const std::string filename = "../referenceJson/AAPL-1h-2023-12-01.json";

    // Parse the json file and insert into the SymbolContainer
    symbols.loadFromJson(filename);

    // At a 1 hour interval, should have 7 AAPL timestamps
    const auto AAPLIter = symbols.getSymbols().find("AAPL");
    const auto numAAPLTimestamps = AAPLIter->second.size();
    REQUIRE(numAAPLTimestamps == 7);
  }

  //===========================================================================
  // Section: insert symbol with key already in map
  //===========================================================================
  SECTION("Insert symbol which already has a key in the map")
  {
    const std::string filename = "../referenceJson/AAPL-1h-2023-12-01.json";

    // Parse the json file and insert into the SymbolContainer
    symbols.loadFromJson(filename);

    // At a 1 hour interval, should have 7 AAPL timestamps
    auto AAPLIter = symbols.getSymbols().find("AAPL");
    auto numAAPLTimestamps = AAPLIter->second.size();
    REQUIRE(numAAPLTimestamps == 7);

    // Second file
    const std::string filename2 = "../referenceJson/AAPL-1h-2023-12-04.json";
    symbols.loadFromJson(filename2);

    // Two days of data at a 1h interval should have 14 timestamps
    AAPLIter = symbols.getSymbols().find("AAPL");
    numAAPLTimestamps = AAPLIter->second.size();
    REQUIRE(numAAPLTimestamps == 14);
  }

  //===========================================================================
  // Section: insert multiple symbols
  //===========================================================================
  SECTION("Insert multiple symbols")
  {
    const std::string filename = "../referenceJson/AAPL-1h-2023-12-01.json";

    // Parse the json file and insert into the SymbolContainer
    symbols.loadFromJson(filename);

    // At a 1 hour interval, should have 7 AAPL timestamps
    const auto AAPLIter = symbols.getSymbols().find("AAPL");
    const auto numAAPLTimestamps = AAPLIter->second.size();

    // Assert first symbol was inserted as expected
    REQUIRE(numAAPLTimestamps == 7);

    // New symbol data file
    const std::string qqqfilename = "../referenceJson/QQQ-1h-2023-12-01.json";

    // Parse the json file and insert into the SymbolContainer
    symbols.loadFromJson(qqqfilename);

    // At a 1 hour interval, should have 7 QQQ timestamps
    const auto QQQIter = symbols.getSymbols().find("QQQ");
    const auto numQQQTimestamps = QQQIter->second.size();

    // Assert both the AAPL and QQQ timestamps coexist
    REQUIRE(numAAPLTimestamps == 7);
    REQUIRE(numQQQTimestamps == 7);
  }

  //===========================================================================
  // Section: check symbol values
  //===========================================================================
  SECTION("Check a symbol's values")
  {
    // For comparing floating point numbers
    using namespace Catch::Matchers;
    constexpr double tolerance = 0.1;

    const std::string filename = "../referenceJson/AAPL-1h-2023-12-01.json";

    // Parse the json file and insert into the SymbolContainer
    symbols.loadFromJson(filename);

    // At a 1 hour interval, should have 7 AAPL timestamps
    const auto AAPLIter = symbols.getSymbols().find("AAPL");
    const auto numAAPLTimestamps = AAPLIter->second.size();

    // Assert first symbol was inserted as expected
    REQUIRE(numAAPLTimestamps == 7);

    // Map of StockTimeSeriesData to check
    const SymbolContainer::SymbolData& aaplMap = symbols.getSymbolData("AAPL");
    
    // Setup some variables to check
    std::string datetime = "2023-12-01 09:30:00";
    double open = 190.3;
    double high = 190.5;
    double low = 189.23;
    double close = 190.06;
    double volume = 8588385.0;

    const StockTimeSeriesData& data0930 = aaplMap.at(datetime);
    REQUIRE_THAT(open, WithinAbs(data0930.getOpenValue(), tolerance));
    REQUIRE_THAT(high, WithinAbs(data0930.getHighValue(), tolerance));
    REQUIRE_THAT(low, WithinAbs(data0930.getLowValue(), tolerance));
    REQUIRE_THAT(close, WithinAbs(data0930.getCloseValue(), tolerance));    
    REQUIRE_THAT(volume, WithinAbs(data0930.getVolume(), tolerance));
    
    // Check a later timestamp's values
    datetime = "2023-12-01 11:30:00";
    open = 190.84;
    high = 191.45;
    low = 190.6;
    close = 191.445;
    volume = 5387598.0;
    const StockTimeSeriesData& data1130 = aaplMap.at(datetime);
    REQUIRE_THAT(open, WithinAbs(data1130.getOpenValue(), tolerance));
    REQUIRE_THAT(high, WithinAbs(data1130.getHighValue(), tolerance));
    REQUIRE_THAT(low, WithinAbs(data1130.getLowValue(), tolerance));
    REQUIRE_THAT(close, WithinAbs(data1130.getCloseValue(), tolerance));
    REQUIRE_THAT(volume, WithinAbs(data1130.getVolume(), tolerance));
  }
}
