#pragma once

//
//
//

#include <string>

class StockTimeSeriesData
{
public:

  StockTimeSeriesData();
  StockTimeSeriesData(double open, double high, double low, double close,
                      double volume);


  // Get values from this timepoint
  double getOpenValue() const { return mOpenValue; }
  double getHighValue() const { return mHighValue; }
  double getLowValue() const { return mLowValue; }
  double getCloseValue() const { return mCloseValue; }
  double getVolume() const { return mVolume; }

  // kptodo needed (?)
  void debugPrint() const;
  
private:

  double mOpenValue;
  double mHighValue;
  double mLowValue;
  double mCloseValue;
  double mVolume;
};
