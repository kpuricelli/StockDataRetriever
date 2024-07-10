#include "WebDataRetriever.h"
#include "StockTimeSeriesData.h"
#include "Json.hpp"
#include <map>
#include <iostream>
#include <fstream>

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
bool WebDataRetriever::isResponseOk()
{
  bool noError = true;
  
  if (mHttpCode != 200)
  {
    std::cout << "HTTP Status Code: " << mHttpCode << std::endl;
    noError = false;
  }

  if (mCurlCode != CURLE_OK)
  {
    std::cout << "cURL Status Code: " << mCurlCode << std::endl;
    noError = false;
  }

  if (!mResponsePtr)
  {
    std::cout << "!mResponsePtr" << std::endl;
    noError = false;
  }

  // kptodo is this a necessary check?
  // Size of the response is zero, but with no errors
  if (mResponsePtr && mResponsePtr->size() == 0)
  {
    std::cout << "mResponsePtr->size() is zero" << std::endl;
    noError = false;
  }

  return noError;
}

//=============================================================================
//=============================================================================
void WebDataRetriever::sendRequest()
{
  if (!mResponsePtr || !mCurlHandle)
  {
    if (!mResponsePtr)
      std::cout << "WebDataRetriever::sendRequest(): !mResponsePtr"
		<< std::endl;
    if (!mCurlHandle)
      std::cout << "WebDataRetriever::sendRequest(): !mCurlHandle"
		<< std::endl;
    std::cout << "Returning...\n" << std::endl;
    return;
  }

  // kptodo
  // assert all url params exist?
  // is there a way for curl to assert a url is valid?
  
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
void WebDataRetriever::parseJsonFile(SymbolContainer& /*container*/)
{
  // kptodo
  // Open the file
#if 0
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

  // kptodo
#endif
}

//=============================================================================
//=============================================================================
void WebDataRetriever::writeResponse2File(const std::string& /*filename*/)
{
  if (!isResponseOk())
  {
    // kptodo how to continue to the next request?
    return;
  }
  
  // Cast to std::string
  mResponse.reserve(mResponsePtr->size());
  mResponse = *mResponsePtr;

  std::ofstream inputFile("../../json/test.json");
  if (inputFile.is_open())
  {
    inputFile << mResponse;
    inputFile.close();
  }
  else
    std::cout << "Unable to open input file!" << std::endl;
}

//=============================================================================
//=============================================================================
void WebDataRetriever::getFileName(std::string& filename)
{
  std::string startDate = mStartDate;
  
  filename = mSymbol + "_" + mStartDate + ".json";
  std::cout << filename << std::endl;
}
