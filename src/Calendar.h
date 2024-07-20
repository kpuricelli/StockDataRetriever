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

  // If mEndDay is == 0, calendar will calculate the end of month day for
  // the current month
  // Essentially if no end day is set, will generate a url for each day of
  // the current month
  Calendar(unsigned short year = 0, unsigned short endDay = 0);
  ~Calendar();

  //
  // kptodo add a start year / end year to iter over years (?)
  // unknown if that would break the api
  //
  
  // Note: calling this function has the side-effect of inserting the market
  // holidays for the given year into mAllHolidays
  void setYear(unsigned short year);

  void setStartMonth(unsigned short month) { mStartMonth = month; }
  void setEndMonth(unsigned short month) { mEndMonth = month; }
  void setStartDay(unsigned short day) { mStartDay = day; }
  void setEndDay(unsigned short day) { mEndDay = day; }

  std::string getYear() { return std::to_string(mYear); }
  std::string getMonth() { return std::to_string(mStartMonth); }
  std::string getDay() { return std::to_string(mStartDay); }


  // Construct a list of urls for WebDataRetriever to loop over
  void generateUrls(std::vector<std::string>& urlList,
                    const std::string& endpoint,
                    const std::string& key,
                    const std::string& symbol,
                    const std::string& interval);

private:

  // Inserts market holidays into mHolidays
  void addAllMarketHolidays();

  // Calculates the actual dates for the given holidays based on the year
  void generateAllHolidaysForYear();
  
  // Container for market holidays regardless of year
  std::vector<boost::gregorian::year_based_generator*> mHolidays;

  // Container for market holidays based on the current calendar year
  std::set<boost::gregorian::date> mAllHolidays;

  // Year in which to generate the holidays for
  unsigned short mYear;

  // WebDataRetriever will set these based on desired time span
  unsigned short mStartMonth;
  unsigned short mEndMonth;
  unsigned short mStartDay;
  unsigned short mEndDay;
};
