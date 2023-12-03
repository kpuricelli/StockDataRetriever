#pragma once

//
//
//

#include "SymbolContainer.h"
#include <string>
#include <curl/curl.h>

class WebDataRetriever
{
public:

  WebDataRetriever();
  ~WebDataRetriever();

  void setEndpoint(const std::string& str) { mEndpoint = str; }
  void setSymbol(const std::string& str) { mSymbol = str; }
  void setInterval(const std::string& str) { mInterval = str; }
  void setStartDate(const std::string& str) { mStartDate = str; }
  void setEndDate(const std::string& str) { mEndDate = str; }
  void setPrecision(int value) { mPrecision = value; }

  void sendRequest();
  void parseResponse(SymbolContainer& container);
  bool responseOk() const { return mHttpCode != 200 || mCurlCode != 200; }
  int getHttpCode() const { return mHttpCode; }
  int getCurlCode() const { return mCurlCode; }
  
private:
  
  void initInternal();
  
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

  int mHttpCode;
  CURLcode mCurlCode;
};
