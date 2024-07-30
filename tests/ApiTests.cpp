//
//
//

//=============================================================================
// Tell Catch2 to provide main()
//=============================================================================
#define CATCH_CONFIG_MAIN
#include "catch_amalgamated.hpp"
#include "../src/Calendar.h"

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

    REQUIRE(marketHolidaysForYear.size() == 10);
  }
}

// kptodo
// Note: this has been randomly tested with the files in referenceJson/
//       would be nice to test a larger dataset though
//=============================================================================
// Test case: Compare programatically downloaded json to ones saved off the
//            API website itself
//=============================================================================
// TEST_CASE("WebDataRetriever", "[CompareJsonToReference]")

// kptodo
//=============================================================================
// Test case: Ensure we don't send more requests / min than 8 (API time limit)
//=============================================================================
// TEST_CASE("WebDataRetriever", "[SendRangeOfRequests]")
