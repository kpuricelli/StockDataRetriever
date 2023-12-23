#include "WebDataRetriever.h"
#include "StockTimeSeriesData.h"
#include "Json.hpp"
#include <map>
#include <iostream>

//
// kptodo / thoughts
// Using a larger dataset, examine the performance tradeoffs of doing a
// new/delete on mResponsePtr between each api call (current implementation),
// and once per creation / destruction of this object. see other 'kptodo-s'
//

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
  : mCurlHandle(nullptr), mResponsePtr(nullptr)
{
  initInternal();
}

//=============================================================================
//=============================================================================
WebDataRetriever::~WebDataRetriever()
{
  if (mResponsePtr)
    delete mResponsePtr;
  mResponsePtr = nullptr;

  // Unsure what curl is doing internally with this ptr
  curl_easy_cleanup(mCurlHandle);
  mCurlHandle = nullptr;
}

//=============================================================================
//=============================================================================
void WebDataRetriever::initInternal()
{
  // kptodo hide this somewhere if this repo ever becomes public
  mApiKey = "cfa77bb3562b4677aed66bcc63659505";

  // curl lib
  mCurlHandle = curl_easy_init();
  if (!mCurlHandle)
  {
    std::cout << "Failed to create curl handle" << std::endl;
    return;
  }

  // kptodo reserve size (?)
  mResponsePtr = new std::string();
  if (!mResponsePtr)
  {
    std::cout << "Failed to create mResponsePtr" << std::endl;
    return;
  }
  mResponsePtr->reserve(4096);

  // Curl callbacks
  
  // Timeout: 10s
  curl_easy_setopt(mCurlHandle, CURLOPT_TIMEOUT, 10);
    
  // Follow redirects
  curl_easy_setopt(mCurlHandle, CURLOPT_FOLLOWLOCATION, 1L);

  // Write data callback
  curl_easy_setopt(mCurlHandle, CURLOPT_WRITEFUNCTION, handleResponse);

  // Hook up data container (curl write data uses void*)
  curl_easy_setopt(mCurlHandle, CURLOPT_WRITEDATA, mResponsePtr);
}

//=============================================================================
// Print out some more info if we get a non-ok status from either curl
// or http
//=============================================================================
void WebDataRetriever::checkResponseStatus()
{
  std::cout << "\nErrors in http or curl response with codes: "
            << "httpcode: " << mHttpCode << std::endl;
  std::cout << "\n curlcode: " << mCurlCode << std::endl;
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

  // kptodo
  // assert all url params exist?
  
  std::string url;
  url.reserve(64);
  
  url += mEndpoint + "/time_series?apikey=" + mApiKey + "&interval="
    + mInterval + "&symbol=" + mSymbol + "&start_date=" + mStartDate
    + "&end_date=" + mEndDate + "&format=JSON";

  // Set the URL
  curl_easy_setopt(mCurlHandle, CURLOPT_URL, url.c_str());

  // Send + curl response code
  mCurlCode = curl_easy_perform(mCurlHandle);
  
  // HTTP response code
  curl_easy_getinfo(mCurlHandle, CURLINFO_RESPONSE_CODE, &mHttpCode);
}

//=============================================================================
//=============================================================================
void WebDataRetriever::parseResponse(SymbolContainer& container)
{
  // Sanity check
  if (!mResponsePtr)
  {
    std::cout << "WebDataRetriever::parseResponse: mResponsePtr is null!"
              << std::endl;
    return;
  }

  // If we got an non-good status code frin either curl or http, investigate
  if (!responseOk())
  {
    checkResponseStatus();
    std::cout << "WebDataRetriever::parseResponse: !responseOk()" << std::endl;
    return;
  }
  
  // Cast to std::string
  mResponse.reserve(mResponsePtr->size());
  mResponse = *mResponsePtr;

  // kptodo
  // should this be in a try block?
  const json responseAsJson = json::parse(mResponse);
  
  // Something bad happened if status != ok
  const std::string status = responseAsJson["status"];
  if (status != "ok")
  {
    const std::string reason = responseAsJson["message"];
    std::cout << "\nError: API status returned: " << status
              << "\nWith message: " << reason << std::endl;
    return;
  }

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

    // kptodo check precision / set (?)
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

  // Clear for next query
  mResponse.clear();
}
