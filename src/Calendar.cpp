#include "Calendar.h"


//=============================================================================
//=============================================================================
namespace
{
  using namespace boost::gregorian;
}

//=============================================================================
//=============================================================================
Calendar::Calendar(unsigned short year)
  : mYear(year)
{
  addAllMarketHolidays();
  generateAllHolidaysForYear();
}

//=============================================================================
//=============================================================================
Calendar::~Calendar()
{
  // kptodo delete the partial_dates in mHolidays
}

//=============================================================================
//=============================================================================
void Calendar::setYear(unsigned short year)
{
  mYear = year;
  addAllMarketHolidays();
  generateAllHolidaysForYear();
}

//=============================================================================
//=============================================================================
void Calendar::generateUrls(std::vector<std::string>& urlList,
                            const std::string& endpoint,
                            const std::string& key,
                            const std::string& symbol,
                            const std::string& interval)
{
  //
  // kptodo
  // For testing: specify the start / end month
  // For gathering all data, want to do all months
  //
  
  // kptodo rm
  std::cout << "Calendar::generateUrls(...)" << std::endl;
  
  // For each month
  for (unsigned short month = mStartMonth; month <= mEndMonth; ++month)
  {
    // kptodo
    // For all days in the month, uncomment
    // Use the calendar to get the last day of the current month
    // auto eom_day = gregorian_calendar::end_of_month_day(mYear, month);

    // kptodo
    // For all days in the month:
    // date endOfMonth(mYear, month, eom_day);
    //
    // For testing (may not be the actual end of month):
    date endOfMonth(mYear, month, mEndDay);
    
    //
    // kptodo
    // For testing: specify the start / end day
    // For gathering all data, want to do all days
    //
    
    // For each day
    day_iterator ditr(date(mYear, month, mStartDay));
    for (; ditr <= endOfMonth; ++ditr)
    {
      // Skip all weekends
      if ((*ditr).day_of_week() == Saturday || (*ditr).day_of_week() == Sunday)
	continue;

      // kptodo skip all holidays

      std::string url;
      url.reserve(128);
      url = endpoint + "/time_series?apikey=" + key + "&interval="
        + interval + "&symbol=" + symbol + "&start_date="
        + to_iso_extended_string(*ditr) + "%2009:30:00"
        + "&end_date=" + to_iso_extended_string(*ditr)
        + "%2015:30:00" + "&format=JSON";

      urlList.emplace_back(url);
    }
  }

  // kptodo rm
  std::cout << "Created " << urlList.size() << " urls" << std::endl;
  for (const auto& i : urlList)
    std::cout << i << std::endl;  
}

//=============================================================================
//=============================================================================
void Calendar::addAllMarketHolidays()
{
  //
  // kptodo
  // good friday..unsure how to calculate that yet (stub below)
  //
  // kptodo
  // Market half days:
  // 3 July (1pm est)
  // 29 November (1pm est)
  // 24 December (0930 est)
  //

  //
  // Fixed holidays
  //
  
  // Western NY
  mHolidays.push_back(new partial_date(1, Jan));

  // Juneteenth
  mHolidays.push_back(new partial_date(19, Jun));

  // US Independence Day
  mHolidays.push_back(new partial_date(4, Jul));

  // Christmas Day
  mHolidays.push_back(new partial_date(25, Dec));

  //
  // Rotating holidays
  //
  
  // nth_day_of_week_in_month is way too long
  typedef nth_day_of_the_week_in_month nth_dow;
    
  // MLK Day
  mHolidays.push_back(new nth_dow(nth_dow::third, Monday, Jan));
  
  // President's Day
  mHolidays.push_back(new nth_dow(nth_dow::third, Monday, Feb));

  // kptodo good friday

  // Memorial Day
  mHolidays.push_back(new nth_dow(nth_dow::fourth, Monday, May));

  // US Labor Day
  mHolidays.push_back(new nth_dow(nth_dow::first, Monday, Sep));
  
  // Thanksgiving
  mHolidays.push_back(new nth_dow(nth_dow::fourth, Thursday, Nov)); 
}

//=============================================================================
//=============================================================================
void Calendar::generateAllHolidaysForYear()
{
  if (mYear <= 0)
    return;
      
  for (std::vector<year_based_generator*>::iterator it = mHolidays.begin();
      it != mHolidays.end(); ++it)
  {
    mAllHolidays.insert((*it)->get_date(mYear));
  }

  // kptodo rm
  for (std::set<date>::iterator it = mAllHolidays.begin();
       it != mAllHolidays.end(); ++it)
    std::cout << to_iso_extended_string(*it) << std::endl;
    
  std::cout << "Number o Holidays: " << mAllHolidays.size() << std::endl;
}

