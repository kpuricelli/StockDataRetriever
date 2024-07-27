#include "Calendar.h"


//=============================================================================
//=============================================================================
namespace
{
  using namespace boost::gregorian;
}

//=============================================================================
//=============================================================================
Calendar::Calendar(unsigned short year,
                   unsigned short startMonth, unsigned short endMonth,
                   unsigned short startDay, unsigned short endDay)
  : mYear(year), mStartMonth(startMonth), mEndMonth(endMonth),
    mStartDay(startDay), mEndDay(endDay)
{
  // kptodo better range checking on year?
  // If year was set, generate the market holidays for this year
  if (mYear > 0)
  {
    // On first thought these holidays happen every year, but that's not true:
    // ex: Juneteenth wasn't an official holiday until 2021
    addAllMarketHolidays();    
    generateAllHolidaysForYear();
  }
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
  // If setting the same year, don't do anything
  if (mYear == year)
    return;
  
  mYear = year;
  generateAllHolidaysForYear();
}

//=============================================================================
//=============================================================================
void Calendar::generateUrls(std::vector<std::pair<std::string, date>>& urlList,
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
  
  // For each month
  for (unsigned short month = mStartMonth; month <= mEndMonth; ++month)
  {
    // If mEndDay == 0, create a url for each day in the month,
    // otherwise, use the given end day
    auto eom_day = gregorian_calendar::end_of_month_day(mYear, month);

    date endOfMonth = (mEndDay == 0) ?
      boost::gregorian::date(mYear, month, eom_day) :
      boost::gregorian::date(mYear, month, mEndDay);
    
    // For each day
    day_iterator ditr(date(mYear, month, mStartDay));
    for (; ditr <= endOfMonth; ++ditr)
    {
      // Skip all weekends
      if ((*ditr).day_of_week() == Saturday || (*ditr).day_of_week() == Sunday)
      {
	std::cout << "Skipping weekend on date: "
		  << to_iso_extended_string(*ditr) << std::endl;
	continue;
      }

      // Skip all holidays
      if (mMarketHolidaysForYear.contains(*ditr))
      {
	// kptodo rm cout
	std::cout << "Skipping market holiday on date: "
		  << to_iso_extended_string(*ditr) << std::endl;
	continue;
      }

      std::string url;
      url.reserve(128);
      url = endpoint + "/time_series?apikey=" + key + "&interval="
        + interval + "&symbol=" + symbol + "&start_date="
        + to_iso_extended_string(*ditr) + "%2009:30:00"
        + "&end_date=" + to_iso_extended_string(*ditr)
        + "%2015:30:00" + "&format=JSON";

      urlList.emplace_back(std::make_pair(url, (*ditr)));
    }
  }
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

  // For Western NY, if the first falls on a Sunday, market is closed the
  // following Monday, if it falls on Saturday, no closure
  {
    date tmpDate = from_string(std::to_string(mYear) + "-01-01");
    if (tmpDate.day_of_week() == Sunday)
      mHolidays.push_back(new partial_date(2, Jan));
    else
      mHolidays.push_back(new partial_date(1, Jan));
  }
  
  // Juneteenth: officially became a federal holiday in 2022
  // If it falls on a Saturday, observed the day prior. If it's on a Sunday,
  // the next weekday
  if (mYear >= 2022)
  {
    date tmpDate = from_string(std::to_string(mYear) + "-06-19");
    if (tmpDate.day_of_week() == Saturday)
      mHolidays.push_back(new partial_date(18, Jun));
    else if (tmpDate.day_of_week() == Sunday)
      mHolidays.push_back(new partial_date(20, Jun));
    else
      mHolidays.push_back(new partial_date(19, Jun));      
  }

  // US Independence Day
  // If it falls on a Saturday, observed the day prior. If it's on a Sunday,
  // the next weekday
  {
    date tmpDate = from_string(std::to_string(mYear) + "-07-04");
    if (tmpDate.day_of_week() == Saturday)
      mHolidays.push_back(new partial_date(3, Jul));
    else if (tmpDate.day_of_week() == Sunday)
      mHolidays.push_back(new partial_date(5, Jul));
    else
      mHolidays.push_back(new partial_date(4, Jul));      
   }
  
  // Christmas Day
  // If it falls on a Saturday, observed the day prior. If it's on a Sunday,
  // the next weekday
  {
    date tmpDate = from_string(std::to_string(mYear) + "-12-25");
    if (tmpDate.day_of_week() == Saturday)
      mHolidays.push_back(new partial_date(24, Dec));
    else if (tmpDate.day_of_week() == Sunday)
      mHolidays.push_back(new partial_date(26, Dec));
    else
      mHolidays.push_back(new partial_date(25, Dec));
   }

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

  // Memorial Day: using 'fifth' defaults to 'last monday in month' :)
  mHolidays.push_back(new nth_dow(nth_dow::fifth, Monday, May));

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
  {
    std::cout << "Calendar::generateAllHolidaysForYear(): "
	      << "invalid year: " << mYear << std::endl;
    return;
  }
      
  for (std::vector<year_based_generator*>::iterator it = mHolidays.begin();
      it != mHolidays.end(); ++it)
  {
    mMarketHolidaysForYear.insert((*it)->get_date(mYear));
  }
}
