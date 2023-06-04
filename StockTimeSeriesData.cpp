#include "StockTimeSeriesData.h"
#include <iostream>

//=============================================================================
//=============================================================================
StockTimeSeriesData::
StockTimeSeriesData(const std::string& symbol, const std::string& timeStamp,
                    double open, double high,  double low, double close,
                    double volume) :
  mSymbol(symbol),
  mDateTime(timeStamp),
  mOpenValue(open),
  mHighValue(high),
  mLowValue(low),
  mCloseValue(close),
  mVolume(volume)
{
}

//=============================================================================
//=============================================================================
void StockTimeSeriesData::debugPrint() const
{
  std::cout << mDateTime << std::endl << mSymbol  << std::endl
            << mOpenValue << std::endl << mHighValue << std::endl
            << mLowValue << std::endl << mCloseValue
            << std::endl << mVolume << std::endl;
}
