#include "StockTimeSeriesData.h"
#include <iostream>

//=============================================================================
//=============================================================================
StockTimeSeriesData::
StockTimeSeriesData(double open, double high,  double low, double close,
                    double volume) :
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
  std::cout << mOpenValue << std::endl << mHighValue << std::endl
            << mLowValue << std::endl << mCloseValue
            << std::endl << mVolume << std::endl;
}
