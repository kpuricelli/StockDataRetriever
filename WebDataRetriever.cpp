#include "WebDataRetriever.h"
#include "StockTimeSeriesData.h"
#include "Json.hpp"
#include <map>
#include <iostream>

//=============================================================================
//=============================================================================
namespace
{
  // Scope the resolution of the parser
  using json = nlohmann::json; 

  // curl callback
  std::size_t handleResponse(const char* in, std::size_t size,
                             std::size_t num, std::string* out)
  {
    const std::size_t totalBytes(size * num);
    out->append(in, totalBytes);
    return totalBytes;
  }
}

//=============================================================================
//=============================================================================
WebDataRetriever::WebDataRetriever()
{
  initInternal();
}

//=============================================================================
//=============================================================================
WebDataRetriever::~WebDataRetriever()
{
  delete mResponsePtr;
}

//=============================================================================
//=============================================================================
void WebDataRetriever::initInternal()
{
  mApiKey = "cfa77bb3562b4677aed66bcc63659505";

  // curl lib
  mCurlHandle = curl_easy_init();
  if (!mCurlHandle)
  {
    std::cout << "Failed to create curl handle" << std::endl;
    return;
  }

  // Curl callbacks
  
  // Timeout: 10s
  curl_easy_setopt(mCurlHandle, CURLOPT_TIMEOUT, 10);
    
  // Follow redirects
  curl_easy_setopt(mCurlHandle, CURLOPT_FOLLOWLOCATION, 1L);

  // Write data callback
  curl_easy_setopt(mCurlHandle, CURLOPT_WRITEFUNCTION, handleResponse);

  // Hook up data container (curl write data uses void*)
  mResponsePtr = new std::string();
  curl_easy_setopt(mCurlHandle, CURLOPT_WRITEDATA, mResponsePtr);
}

//=============================================================================
//=============================================================================
void WebDataRetriever::sendRequest()
{
  if (!mResponsePtr || !mCurlHandle)
  {
    std::cout << "WebDataRetriever::sendRequest(): "
      "!mResponsePtr or !mCurlHandle" << std::endl;
    return;
  }
  
  std::string url;
  url.reserve(64);
  
  url += mEndpoint + "/time_series?apikey=" + mApiKey + "&interval="
    + mInterval + "&symbol=" + mSymbol + "&start_date=" + mStartDate
    + "&end_date=" + mEndDate + "&format=JSON";

  // Set the URL
  curl_easy_setopt(mCurlHandle, CURLOPT_URL, url.c_str());

  // Send
  const CURLcode curlCode = curl_easy_perform(mCurlHandle);
  
  // HTTP response code
  long httpCode = 0;
  curl_easy_getinfo(mCurlHandle, CURLINFO_RESPONSE_CODE, &httpCode);

  // Check responses
  if (httpCode != 200 || curlCode != 0)
  {
    std::cout << "WebDataRetriever::sendRequest(): "
      "httpCode, curlCode: " << httpCode << curlCode << std::endl;
  }
  
  curl_easy_cleanup(mCurlHandle);
}

//=============================================================================
//=============================================================================
void WebDataRetriever::parseResponse(SymbolContainer& container)
{
  if (!mResponsePtr)
  {
    std::cout << "WebDataRetriever::parseResponse: mResponsePtr nullptr"
              << std::endl;
    return;
  }
  
  // Cast to std::string
  mResponse = *mResponsePtr;

  const json responseAsJson = json::parse(mResponse);

  const json meta = responseAsJson["meta"];
  const std::string symbol = meta["symbol"];
  
  const json values = responseAsJson["values"];
  std::map<std::string, StockTimeSeriesData> valuesContainer;
  for (const auto& value : values)
  {
    const std::string timestamp = value["datetime"];
    const std::string openStr = value["open"];
    const std::string highStr = value["high"];
    const std::string lowStr = value["low"];
    const std::string closeStr = value["close"];
    const std::string volumeStr = value["volume"];

    // kptodo check precision / set
    // Typecast to numbers
    const double open = std::atof(openStr.c_str());
    const double high = std::atof(highStr.c_str());
    const double low = std::atof(lowStr.c_str());
    const double close = std::atof(closeStr.c_str());
    const double volume = std::atof(volumeStr.c_str());

    // Create the time series object
    StockTimeSeriesData record(symbol, timestamp, open, high, low, close,
                               volume);

    // Save local
    valuesContainer.emplace(timestamp, record);
  }
  
  // Save to container
  container.insertSymbol(symbol, valuesContainer);
}
