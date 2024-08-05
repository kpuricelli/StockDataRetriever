#include "WebDataRetriever.h"
#include "boost/date_time/posix_time/posix_time.hpp"
#include <map>
#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>

//=============================================================================
//=============================================================================
namespace
{
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
  // Get the api key from the env var
#if UseAPIKey1
  mApiKey = std::getenv("tdkey1");
#else
  mApiKey = std::getenv("tdkey2");
#endif
  
  initInternal();
}

//=============================================================================
//=============================================================================
WebDataRetriever::~WebDataRetriever()
{
  if (mResponsePtr)
    delete mResponsePtr;
  mResponsePtr = nullptr;

  // Confirmed w/ valgrind mCurlHandle is being free-d
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

  // One trading day's worth of data @1min intervals is ~50000 bytes
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

  if (mResponsePtr && mResponsePtr->size() == 0)
  {
    std::cout << "mResponsePtr->size() is zero" << std::endl;
    noError = false;
  }

  return noError;
}

//=============================================================================
// kpnote
// There's some odd looking stuff happening in this function, for
// the following reasons: the API key i'm using is the free one. So we can
// send a maximum of 8 requests to the API endpoint per minute ("per minute"
// here means wall clock time, not time since first request)
// Ex: request 1: 10:44:58 (7 credits remaining)
// Ex: request 2: 10:45:00 (7 credits remaining)
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

  // General info about total number of requests we have in our list to send
  std::cout << "WebDataRetriever::sendRangeOfRequests(): "
            << mUrlList.size() << " requests to process" << std::endl;

  // Variables to track state
  int requestNumber = 0;
  int requestsThisMinute = 0;
  boost::posix_time::time_duration time4OneRequest;
  boost::posix_time::time_duration time4EightRequests;
  boost::posix_time::time_duration totalTime;

  // Start time
  auto startTime = boost::posix_time::second_clock::local_time();

  // Convert to struct tm to grab just the seconds part
  auto startTimeAsTm = boost::posix_time::to_tm(startTime);

  // Create a time limit for the next 8 requests, which is 1 minute - the
  // number of seconds at the start time
  auto timeLimit = boost::posix_time::minutes(1) -
    boost::posix_time::seconds(startTimeAsTm.tm_sec);

  // Create a buffer of 5 seconds to compare the time limit against the
  // total time with
  const auto timeBuffer = boost::posix_time::seconds(3);
  const auto timeBufferAsChrono = std::chrono::seconds(3);

  // For each request
  for (const auto& url : mUrlList)
  {
    // kptodo rm (?)
    // std::cout << url.first << std::endl;
    
    // Time as of sending the request
    startTime = boost::posix_time::second_clock::local_time();

    // Send the request
    sendRequest(url.first);

    // Check the curl and http response codes
    if (!isResponseOk())
    {
      std::cout << "Warning: !isResponseOk()" << std::endl;
      std::cout << "Overall request number: " << requestNumber << std::endl;
      std::cout << "Request this minute: " << requestsThisMinute << std::endl;
      std::cout << "URL: " << url.first << std::endl;
      continue;
    }
    
    // Create a filename in the form of symbol_interval_year_month_day.json
    std::string filename;
    filename.reserve(64);
    getFileName(filename, url.second);

    // Write the response
    writeResponse2File(filename);

    // Update internal state
    auto endTime = boost::posix_time::second_clock::local_time();
    time4OneRequest = endTime - startTime;
    time4EightRequests += time4OneRequest;
    totalTime += time4OneRequest;
    ++requestNumber;
    ++requestsThisMinute;

    //
    // Two cases we need to consider:
    //
    // 1.) We've hit the time limit before sending 8 requests for the
    //     current minute => reset the time limit and requestsThisMinute
    if (timeLimit.total_seconds() <=
        (time4EightRequests.total_seconds() + timeBuffer.total_seconds()))
    {
#if DEBUG
      std::cout << "Time limit reached!" << std::endl;
      std::cout << "Sent " << requestsThisMinute << " requests this minute"
                << std::endl;
#endif
      
      // Reset how many requests we've sent this minute + time taken
      requestsThisMinute = 0;
      time4EightRequests = boost::posix_time::seconds(0);

      // Sleep for bufferTime to ensure we cross a new minute boundary
#if DEBUG
      std::cout << "Sleeping for: " << timeBuffer.total_seconds() << "s"
                << std::endl;
#endif
      
      std::this_thread::sleep_for(timeBufferAsChrono);

      // Reset the start time
      startTime = boost::posix_time::second_clock::local_time();

      // Convert to struct tm to grab just the seconds part
      startTimeAsTm = boost::posix_time::to_tm(startTime);

      // Create a time limit for the next 8 requests, which is 1 minute - the
      // number of seconds at the start time
      timeLimit = boost::posix_time::minutes(1) -
        boost::posix_time::seconds(startTimeAsTm.tm_sec);
    }

    //
    // 2.) We've sent 8 requests before the time limit has been reached,
    //     in which case we need to wait until the next minute before sending
    //     the next request
    else if ((requestsThisMinute % 8) == 0)
    {
#if DEBUG
      std::cout << "Request limit reached!" << std::endl;
      std::cout << "Sent " << requestsThisMinute << " requests this minute"
                << std::endl;
#endif
      
      // Reset how many requests we've sent this minute + time taken
      requestsThisMinute = 0;
      time4EightRequests = boost::posix_time::seconds(0);

      // Figure out how long we need to sleep for
      // Convert to struct tm to grab just the seconds part
      auto endTimeAsTm = boost::posix_time::to_tm(endTime);

      // Need to wait until the next wall clock minute => 1 minute - seconds it
      // took to fill 8 requests
      const auto time2wait =
        std::chrono::minutes(1) - std::chrono::seconds(endTimeAsTm.tm_sec);

#if DEBUG
      std::cout << time2wait.count() << "s until next minute. Calling "
        "std::this_thread_sleep_for(" << time2wait.count() << ")" << std::endl;
#endif
      
      std::this_thread::sleep_for(time2wait);

      // Now that we've awoken, recalculate the time limit
      endTime = boost::posix_time::second_clock::local_time();

      // Figure out how long we need to sleep for
      // Convert to struct tm to grab just the seconds part
      endTimeAsTm = boost::posix_time::to_tm(endTime);

      // Create a time limit for the next 8 requests, which is 1 minute - the
      // number of seconds at the start time
      timeLimit = boost::posix_time::minutes(1) -
        boost::posix_time::seconds(endTimeAsTm.tm_sec);
    }

    // Print out some stats / progress
    std::cout << "Request number " << requestNumber << " took "
              << time4OneRequest.total_seconds() << "s; total time: "
              << totalTime.total_seconds() << "s; requests still in list: "
              << mUrlList.size() - requestNumber << std::endl;
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
// Create a filename in the form: of symbol_interval_year_month_day.json
//=============================================================================
void WebDataRetriever::writeResponse2File(const std::string& filename)
{
  // Cast to std::string
  mResponse.reserve(mResponsePtr->size());
  mResponse = *mResponsePtr;

  // Moved the json files into time-specific directories
  const std::string path = "../json/" + mInterval + "/";
  
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
