#pragma once

//
//
//

#include <vector>
#include <set>
#include <boost/date_time/gregorian/gregorian.hpp>

class Calendar
{
public:

  // If mEndDay is == 0, calendar will calculate the end of month day for
  // the current month
  // Essentially if no end day is set, will generate a url for each day of
  // the current month
  Calendar(unsigned short startYear = 0,
           unsigned short endYear = 0,
           unsigned short startMonth = 0,
           unsigned short endMonth = 0,
           unsigned short startDay = 0,
           unsigned short endDay = 0);
  ~Calendar();


  // Setters for calendar time spans
  void setStartYear(unsigned short year) { mStartYear = year; }
  void setEndYear(unsigned short year) { mEndYear = year; }
  void setStartMonth(unsigned short month) { mStartMonth = month; }
  void setEndMonth(unsigned short month) { mEndMonth = month; }
  void setStartDay(unsigned short day) { mStartDay = day; }
  void setEndDay(unsigned short day) { mEndDay = day; }

  // Getters for calendar time spans
  unsigned short getStartYear() const { return mStartYear; }
  unsigned short getEndYear() const { return mEndYear; }  
  unsigned short getStartMonth() const { return mStartMonth; }
  unsigned short getEndMonth() const { return mEndMonth; }
  unsigned short getStartDay() const { return mStartDay; }
  unsigned short getEndDay() const { return mEndDay; }

  // Validate dates for calendar are non-default constructed
  bool isStartYearSet() const { return mStartYear != 0; }
  bool isEndYearSet() const { return mEndYear != 0; }  
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
  // I liked when this was private, but need to expose for testing purposes
  // Inserts market holidays into mHolidays
  void addAllMarketHolidays(unsigned short year);

  // kptodo
  // This should probably be removed, but exposed for testing purposes
  size_t getNumberOfFixedMarketHolidays() const { return mHolidays.size(); }

  // kptodo
  // This should probably be removed, but exposed for testing purposes
  const std::set<boost::gregorian::date>& getMarketHolidaysForYear() const
    { return mMarketHolidaysForYear; }

private:

  // Resets mHolidays and mMarketHolidaysForYear
  void resetHolidays();

  // Calculate the date of Good Friday for this year
  boost::gregorian::date calculateGoodFriday(unsigned short year);

  // Calculates the actual dates for the given holidays based on the year
  void generateAllHolidaysForYear(unsigned short year);
  
  // Container for market holidays regardless of year
  std::vector<std::unique_ptr<
                boost::gregorian::year_based_generator>> mHolidays;

  // Container for market holidays based on the current calendar year
  std::set<boost::gregorian::date> mMarketHolidaysForYear;

  // WebDataRetriever will set these based on desired time span
  unsigned short mStartYear;
  unsigned short mEndYear;
  unsigned short mStartMonth;
  unsigned short mEndMonth;
  unsigned short mStartDay;
  unsigned short mEndDay;
};
