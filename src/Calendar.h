#pragma once

//
//
//

#include <vector>
#include <set>

// kptodo: change to gregorian_types.hpp (?)
// includes all types plus i/o
#include <boost/date_time/gregorian/gregorian.hpp>
// no i/o just types
//#include "boost/date_time/gregorian/gregorian_types.hpp"

class Calendar
{
public:
  
  Calendar(unsigned short year = 0);
  ~Calendar();

  //
  // kptodo add a start year / end year to iter over years (?)
  // unknown if that would break the api
  //
  
  // Note: calling this function has the side-effect of updating mAllHolidays
  void setYear(unsigned short year);

  void setStartMonth(unsigned short month) { mStartMonth = month; }
  void setEndMonth(unsigned short month) { mEndMonth = month; }
  void setStartDay(unsigned short day) { mStartDay = day; }
  void setEndDay(unsigned short day) { mEndDay = day; }

  // Construct a list of urls for WebDataRetriever to loop over
  void generateUrls(std::vector<std::string>& urlList,
                    const std::string& endpoint,
                    const std::string& key,
                    const std::string& symbol,
                    const std::string& interval);

private:

  // Inserts market holidays into mHolidays
  void addAllMarketHolidays();

  // Fills mAllHolidays with the actual date of the holiday for the given year
  void generateAllHolidaysForYear();
  
  // Some holidays change year by year, so need to recalc 4 each year
  std::vector<boost::gregorian::year_based_generator*> mHolidays;

  // Set for all market holidays this year
  std::set<boost::gregorian::date> mAllHolidays;

  // Year in which to generate the holidays for
  unsigned short mYear;

  // WebDataRetriever will set these based on desired time span
  unsigned short mStartMonth;
  unsigned short mEndMonth;
  unsigned short mStartDay;
  unsigned short mEndDay;
};
