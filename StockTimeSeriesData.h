#pragma once

//
//
//

#include <string>

class StockTimeSeriesData
{
public:

  StockTimeSeriesData(const std::string& symbol, const std::string& timeStamp,
                      double open, double high, double low, double close,
                      double volume);

  const std::string& getTimestamp() const { return mDateTime; }

  void debugPrint() const;
  
private:
  
  std::string mSymbol;
  std::string mDateTime;
  double mOpenValue;
  double mHighValue;
  double mLowValue;
  double mCloseValue;
  double mVolume;
};
