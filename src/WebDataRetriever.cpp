#include "WebDataRetriever.h"
#include "StockTimeSeriesData.h"
#include "Json.hpp"
#include <map>
#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>

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

  // Curl callback when data is received
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
  // kptodo put this in an environment variable
  mApiKey = "cfa77bb3562b4677aed66bcc63659505";
  
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
// Setup the required curl options and callbacks
//=============================================================================
void WebDataRetriever::initInternal()
{
  mCurlHandle = curl_easy_init();
  if (!mCurlHandle)
  {
    std::cout << "Failed to create curl handle" << std::endl;
    return;
  }

  mResponsePtr = new std::string();
  if (!mResponsePtr)
  {
    std::cout << "Failed to create mResponsePtr" << std::endl;
    return;
  }

  // One trading day's worth of data is around 50000 bytes
  mResponsePtr->reserve(50000);

  // Timeout at 10s
  curl_easy_setopt(mCurlHandle, CURLOPT_TIMEOUT, 10);
    
  // Follow redirects
  curl_easy_setopt(mCurlHandle, CURLOPT_FOLLOWLOCATION, 1L);

  // Set curl callback write function (handleResponse)
  curl_easy_setopt(mCurlHandle, CURLOPT_WRITEFUNCTION, handleResponse);

  // Tell curl to write into mResponsePtr (curl technically writes as void*)
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
void WebDataRetriever::sendRangeOfRequests()
{
  // kptodo sanity check the time span (?)

  // Get the list of endpoint URLs to send as requests from the calendar
  getUrls();

  if (mUrlList.empty())
  {
    std::cout << "WebDataRetriever::sendRangeOfRequests(): "
	      <<  "no URLs generated for given time span" << std::endl;
    return;
  }

  // kptodo rm (?)
  std::cout << "WebDataRetriever::sendRangeOfRequests(): "
            << mUrlList.size() << " requests to process" << std::endl;

  // The free API plan i'm on has a limit of 8 requests per minute, so gotta
  // slow ourselves down
  int requestNumber = 0;
  std::chrono::duration<double, std::milli> elapsedTime;
  std::chrono::duration<double, std::milli> oneRequestTime;
  std::chrono::duration oneMin = std::chrono::milliseconds(60000);

  // For each url
  for (const auto& url : mUrlList)
  {
    // kptodo rm (?)
    std::cout << url.first << std::endl;
    
    // Time now
    const auto start = std::chrono::high_resolution_clock::now();

    // Send the request
    sendRequest(url.first);

    // Check the curl and http response codes
    if (!isResponseOk())
    {
      // Update the time
      // Time now
      const auto end = std::chrono::high_resolution_clock::now();
      oneRequestTime = end - start;
      elapsedTime += oneRequestTime;
      ++requestNumber;
      
      std::cout << "Warning: !isResponseOk() for url: "
                << url.first << " skipping to next url..." << std::endl;
      
      continue;
    }

    // Create a filename in the form of symbol_interval_year_month_day.json
    std::string filename;
    filename.reserve(64);
    getFileName(filename, url.second);

    // Write the response
    writeResponse2File(filename);

    // Time now
    const auto end = std::chrono::high_resolution_clock::now();

    // Update the time
    oneRequestTime = end - start;
    elapsedTime += oneRequestTime;
    ++requestNumber;

    // kptodo rm (?)
    std::cout << "Request number " << requestNumber << " took "
              << oneRequestTime.count() << "ms" << std::endl
              << "Elapsed time " << elapsedTime.count() << "ms" << std::endl;

    // If we sent 8 requests in under a minute, sleep until the next minute
    if (requestNumber % 8 == 0)
    {
      std::cout << "8 requests took " << elapsedTime.count() << std::endl;
      std::cout << mUrlList.size() - requestNumber
                << " requests still in queue!" << std::endl;

      auto time2sleep = oneMin - elapsedTime;
      std::cout << "Sleeping for: " << time2sleep.count() << "ms" << std::endl;
      std::this_thread::sleep_for(time2sleep);
    }
  }
}

//=============================================================================
//=============================================================================
void WebDataRetriever::sendRequest(const std::string& url)
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

  // Release the old data
  mResponsePtr->clear();

  // One trading day's worth of data is around 50000 bytes
  mResponsePtr->reserve(50000);

  // Set the URL
  curl_easy_setopt(mCurlHandle, CURLOPT_URL, url.c_str());

  // Send request + grab curl response code
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
void WebDataRetriever::writeResponse2File(const std::string& filename)
{
  // Cast to std::string
  mResponse.reserve(mResponsePtr->size());
  mResponse = *mResponsePtr;

  const static std::string path = "../json/";
  
  std::ofstream inputFile(path + filename);
  if (inputFile.is_open())
  {
    inputFile << mResponse;
    inputFile.close();
  }

  // kptodo do something about this error
  else
    std::cout << "Unable to open input file!" << std::endl;
}

//=============================================================================
//=============================================================================
void WebDataRetriever::getFileName(std::string& filename,
                                   const boost::gregorian::date& date)
{
  filename = mSymbol + "-" + mInterval + "-" + to_iso_extended_string(date)
    + ".json";
}
