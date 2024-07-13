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

#if 1
  // API endpoint parameters mCalendar owns
  void setYear(unsigned short year) { mCalendar.setYear(year); }
  void setStartMonth(unsigned short month) { mCalendar.setStartMonth(month); }
  void setEndMonth(unsigned short month) { mCalendar.setEndMonth(month); }
  void setStartDay(unsigned short day) { mCalendar.setStartDay(day); }
  void setEndDay(unsigned short day) { mCalendar.setEndDay(day); }
#endif
  
  // Status codes
  int getHttpCode() const { return mHttpCode; }
  int getCurlCode() const { return mCurlCode; }

  // kptodo
  // Loop from start month / start day to end month / end day
  // void sendRangeOfRequests();

  // kptodo 4 tst
  void getUrls()
  { mCalendar.generateUrls(mUrlList, mEndpoint, mApiKey, mSymbol, mInterval); }
  
  // Tells curl to send the request
  void sendRequest();

  // Different ways to handle the response
  void parseJsonFile(SymbolContainer& container);
  void writeResponse2File(const std::string& filename);
  void getFileName(std::string& filename);
  
private:
  
  void initInternal();
  bool isResponseOk();

  // URL container
  std::vector<std::string> mUrlList;

  // Calendar object to help pull data from over a time span
  Calendar mCalendar;
  
  CURL* mCurlHandle;

  std::string* mResponsePtr;
  std::string mResponse;

  // 12data api parameters
  std::string mEndpoint;
  std::string mApiKey;
  std::string mSymbol;
  std::string mInterval;
  std::string mStartDate;
  std::string mEndDate;
  int mPrecision;

  // Response codes
  int mHttpCode;
  CURLcode mCurlCode;
};
