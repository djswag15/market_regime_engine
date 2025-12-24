#pragma once
#include "core/TimeSeries.hpp"
#include <string>

class CSVReader {
public:
    static TimeSeries read_price_series(const std::string& path, 
                                       const std::string& price_col = "Close");
};