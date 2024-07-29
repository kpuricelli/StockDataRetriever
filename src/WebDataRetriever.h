#pragma once

//
//
//

#include "SymbolContainer.h"
#include "Calendar.h"
#include <string>
#include <vector>
#include <curl/curl.h>

class WebDataRetriever
{
public:

  WebDataRetriever();
  ~WebDataRetriever();

  // API endpoint parameters we own
  void setEndpoint(const std::string& str) { mEndpoint = str; }
  void setSymbol(const std::string& str) { mSymbol = str; }
  void setInterval(const std::string& str) { mInterval = str; }
  void setPrecision(int value) { mPrecision = value; }

  // API endpoint parameters mCalendar owns
  void setStartYear(unsigned short year) { mCalendar.setStartYear(year); }
  void setEndYear(unsigned short year) { mCalendar.setEndYear(year); }  
  void setStartMonth(unsigned short month) { mCalendar.setStartMonth(month); }
  void setEndMonth(unsigned short month) { mCalendar.setEndMonth(month); }
  void setStartDay(unsigned short day) { mCalendar.setStartDay(day); }
  void setEndDay(unsigned short day) { mCalendar.setEndDay(day); }
  
  // Public wrapper which calls sendRequest() over the set time span
  void sendRangeOfRequests();
  
private:

  // Get the CURL URLs from mCalendar
  void getUrls()
  { mCalendar.generateUrls(mUrlList, mEndpoint, mApiKey, mSymbol, mInterval); }

  // Prepare internal members / setup curl options
  void initInternal();

  // Assert status of the response from the API
  bool isResponseOk();

  // Send a single http request
  void sendRequest(const std::string& url);

  // kptodo
  // Read local json files
  void parseJsonFile(SymbolContainer& container);

  // Write the returned data to a local file
  void writeResponse2File(const std::string& filename);

  // Create filename of the form: symbol-interval-year-month-day.json
  void getFileName(std::string& filename, const boost::gregorian::date& date);

  // URL container
  std::vector<std::pair<std::string, boost::gregorian::date>> mUrlList;

  // Calendar object to help pull data from over a time span
  Calendar mCalendar;

  // Handle for curl configuration
  CURL* mCurlHandle;

  // Curl writes to mResponsePtr as void*
  std::string* mResponsePtr;
  std::string mResponse;

  // 12data api parameters
  std::string mEndpoint;
  std::string mApiKey;
  std::string mSymbol;
  std::string mInterval;
  int mPrecision;

  // Response codes
  int mHttpCode;
  CURLcode mCurlCode;
};
