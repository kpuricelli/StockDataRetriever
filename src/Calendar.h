#pragma once

//
//
//

#include <vector>
#include <set>

// kptodo
// change to gregorian_types.hpp (?)
// The below includes all types plus i/o
#include <boost/date_time/gregorian/gregorian.hpp>
// No i/o just types:
// #include "boost/date_time/gregorian/gregorian_types.hpp"

class Calendar
{
public:

  // If mEndDay is == 0, calendar will calculate the end of month day for
  // the current month
  // Essentially if no end day is set, will generate a url for each day of
  // the current month
  Calendar(unsigned short year = 0,
           unsigned short startMonth = 0,
           unsigned short endMonth = 0,
           unsigned short startDay = 0,
           unsigned short endDay = 0);
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

  // Getters for calendar dates
  unsigned short getYear() const { return mYear; }
  unsigned short getStartMonth() const { return mStartMonth; }
  unsigned short getEndMonth() const { return mEndMonth; }
  unsigned short getStartDay() const { return mStartDay; }
  unsigned short getEndDay() const { return mEndDay; }

  // Validate dates for calendar are non-default constructed
  bool isYearSet() const { return mYear != 0; }
  bool isStartMonthSet() const { return mStartMonth != 0; }
  bool isEndMonthSet() const { return mEndMonth != 0; }
  bool isStartDaySet() const { return mStartDay != 0; }
  bool isEndDaySet() const { return mEndDay != 0; }

  // Construct a list of urls for WebDataRetriever to loop over
  void generateUrls(std::vector<std::pair<std::string,
                    boost::gregorian::date>>& urlList,
                    const std::string& endpoint,
                    const std::string& key,
                    const std::string& symbol,
                    const std::string& interval);

  // kptodo
  // This should probably be removed, but exposed for testing purposes
  size_t getNumberOfFixedMarketHolidays() const { return mHolidays.size(); }

  // kptodo
  // This should probably be removed, but exposed for testing purposes
  const std::set<boost::gregorian::date>& getMarketHolidaysForYear() const
    { return mMarketHolidaysForYear; }

private:

  // Inserts market holidays into mHolidays
  void addAllMarketHolidays();

  // Calculates the actual dates for the given holidays based on the year
  void generateAllHolidaysForYear();
  
  // Container for market holidays regardless of year
  std::vector<boost::gregorian::year_based_generator*> mHolidays;

  // Container for market holidays based on the current calendar year
  std::set<boost::gregorian::date> mMarketHolidaysForYear;

  // Year in which to generate the holidays for
  unsigned short mYear;

  // WebDataRetriever will set these based on desired time span
  unsigned short mStartMonth;
  unsigned short mEndMonth;
  unsigned short mStartDay;
  unsigned short mEndDay;
};
