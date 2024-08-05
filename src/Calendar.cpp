#include "Calendar.h"


//=============================================================================
//=============================================================================
namespace
{
  using namespace boost::gregorian;
}

//=============================================================================
//=============================================================================
Calendar::Calendar(unsigned short startYear, unsigned short endYear,
                   unsigned short startMonth, unsigned short endMonth,
                   unsigned short startDay, unsigned short endDay)
  : mStartYear(startYear), mEndYear(endYear),
    mStartMonth(startMonth), mEndMonth(endMonth),
    mStartDay(startDay), mEndDay(endDay)
{
}

//=============================================================================
//=============================================================================
Calendar::~Calendar()
{
  mHolidays.clear();
}

//=============================================================================
//=============================================================================
void Calendar::generateUrls(std::vector<std::pair<std::string, date>>& urlList,
                            const std::string& endpoint,
                            const std::string& key,
                            const std::string& symbol,
                            const std::string& interval)
{
  // For each year
  for (unsigned short year = mStartYear; year <= mEndYear; ++year)
  {
    // Update the holidays with the current year's dates
    if (mHolidays.size() > 0)
      resetHolidays();
    
    addAllMarketHolidays(year);
    
    // For each month
    for (unsigned short month = mStartMonth; month <= mEndMonth; ++month)
    {
      // If mEndDay == 0, create a url for each day in the month,
      // otherwise, use the given end day
      auto eom_day = gregorian_calendar::end_of_month_day(year, month);

      date endOfMonth = (mEndDay == 0) ?
        boost::gregorian::date(year, month, eom_day) :
        boost::gregorian::date(year, month, mEndDay);
    
      // For each day
      day_iterator ditr(date(year, month, mStartDay));
      for (; ditr <= endOfMonth; ++ditr)
      {
        // Skip all weekends
        if ((*ditr).day_of_week() == Saturday ||
            (*ditr).day_of_week() == Sunday)
        {
#if DEBUG
          std::cout << "Skipping weekend on date: "
                    << to_iso_extended_string(*ditr) << std::endl;
#endif
          continue;
        }

        // Skip all holidays
        if (mMarketHolidaysForYear.contains(*ditr))
        {
#if DEBUG
          std::cout << "Skipping market holiday on date: "
                    << to_iso_extended_string(*ditr) << std::endl;
#endif
          continue;
        }

        // kptodo some market trading days are half days
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
}

//=============================================================================
// Resets mHolidays and mMarketHolidaysForYear
//=============================================================================
void Calendar::resetHolidays()
{
  // Since mHolidays is full of std::unique_ptr-s, just using .clear() on
  // the container itself will delete the new-d ptr-s since they're losing
  // the only reference count they have in mHolidays
  mHolidays.clear();
  mMarketHolidaysForYear.clear();
}

//=============================================================================
// Calculate the date of Easter for this year
// Credit:
// web.archive.org/web/20120223154950/aa.usno.navy.mil/faq/docs/easter.php
//
// Then take Easter Sunday, and go back in time two days = Good Friday
//=============================================================================
date Calendar::calculateGoodFriday(unsigned short year)
{
  // kptodo clean this up a bit maybe...? v difficult to look at
  int c = year / 100;
  int n = year - 19 * ( year / 19 );
  int k = ( c - 17 ) / 25;
  int i = c - c / 4 - ( c - k ) / 3 + 19 * n + 15;
  i = i - 30 * ( i / 30 );
  i = i - ( i / 28 ) * ( 1 - ( i / 28 ) * ( 29 / ( i + 1 ) )
                         * ( ( 21 - n ) / 11 ) );
  int j = year + year / 4 + i + 2 - c + c / 4;
  j = j - 7 * ( j / 7 );
  int l = i - j;
  int m = 3 + ( l + 40 ) / 44;
  int d = l + 28 - 31 * ( m / 4 );

  // Construct the date of Easter
  const date easter = from_string(std::to_string(year) + "-" +
                                  std::to_string(m) + "-" + std::to_string(d));

  // Could do a little extra math and construct Good Friday without having to
  // create easter, but this way we can let boost worry about whether two days
  // before Easter Sunday is the same month or not, etc.
  const date_duration dd(2);

  // Two days before Easter Sunday is Good Friday
  const date gFriday = easter - dd;
  return gFriday;
}

//=============================================================================
//=============================================================================
void Calendar::addAllMarketHolidays(unsigned short year)
{
  //
  // kptodo
  // Market half days:
  // 3 July (1pm est)
  // 29 November (1pm est)
  // 24 December (0930 est)
  //

  //
  // Holidays with the same literal date each year (not market observed date)
  //

  // For Western NY, if the first falls on a Sunday, market is closed the
  // following Monday, if it falls on Saturday, no closure
  {
    date tmpDate = from_string(std::to_string(year) + "-01-01");
    if (tmpDate.day_of_week() == Sunday)
      mHolidays.push_back(std::make_unique<partial_date>(2, Jan));
    else
      mHolidays.push_back(std::make_unique<partial_date>(1, Jan));
  }

  // Juneteenth: officially became a federal holiday in 2022
  // If it falls on a Saturday, observed the day prior. If it's on a Sunday,
  // the next weekday
  if (year >= 2022)
  {
    date tmpDate = from_string(std::to_string(year) + "-06-19");
    if (tmpDate.day_of_week() == Saturday)
      mHolidays.push_back(std::make_unique<partial_date>(18, Jun));
    else if (tmpDate.day_of_week() == Sunday)
      mHolidays.push_back(std::make_unique<partial_date>(20, Jun));
    else
      mHolidays.push_back(std::make_unique<partial_date>(19, Jun));      
  }

  // US Independence Day
  // If it falls on a Saturday, observed the day prior. If it's on a Sunday,
  // the next weekday
  {
    date tmpDate = from_string(std::to_string(year) + "-07-04");
    if (tmpDate.day_of_week() == Saturday)
      mHolidays.push_back(std::make_unique<partial_date>(3, Jul));
    else if (tmpDate.day_of_week() == Sunday)
      mHolidays.push_back(std::make_unique<partial_date>(5, Jul));
    else
      mHolidays.push_back(std::make_unique<partial_date>(4, Jul));      
   }

  // Christmas Day
  // If it falls on a Saturday, observed the day prior. If it's on a Sunday,
  // the next weekday
  {
    date tmpDate = from_string(std::to_string(year) + "-12-25");
    if (tmpDate.day_of_week() == Saturday)
      mHolidays.push_back(std::make_unique<partial_date>(24, Dec));
    else if (tmpDate.day_of_week() == Sunday)
      mHolidays.push_back(std::make_unique<partial_date>(26, Dec));
    else
      mHolidays.push_back(std::make_unique<partial_date>(25, Dec));
   }

  //
  // Holidays with the same position in the week / month each year
  //
  
  // nth_day_of_week_in_month is way too long
  typedef nth_day_of_the_week_in_month nth_dow;
    
  // MLK Day
  mHolidays.push_back(std::make_unique<nth_dow>(nth_dow::third, Monday, Jan));
  
  // President's Day
  mHolidays.push_back(std::make_unique<nth_dow>(nth_dow::third, Monday, Feb));

  // Memorial Day: using 'fifth' defaults to 'last monday in month' :)
  mHolidays.push_back(std::make_unique<nth_dow>(nth_dow::fifth, Monday, May));

  // US Labor Day
  mHolidays.push_back(std::make_unique<nth_dow>(nth_dow::first, Monday, Sep));
  
  // Thanksgiving
  mHolidays.push_back(std::make_unique<nth_dow>(
                        nth_dow::fourth, Thursday, Nov));

  // Insert into mMarketHolidaysForYear
  generateAllHolidaysForYear(year);
}

//=============================================================================
//=============================================================================
void Calendar::generateAllHolidaysForYear(unsigned short year)
{
  if (year <= 0)
  {
    std::cout << "Calendar::generateAllHolidaysForYear(): "
	      << "invalid year: " << year << std::endl;
    return;
  }

  // Since we have a formula to calculate Easter Sunday by year, the date
  // two days in the past from Easter is Good Friday
  const date& goodFriday = calculateGoodFriday(year);
  mMarketHolidaysForYear.insert(goodFriday);

  // Using an rvalue reference whenever possible
  for (auto&& holiday : mHolidays)
    mMarketHolidaysForYear.insert(holiday->get_date(year));
}
