#pragma once
#include "core/TimeSeries.hpp"
#include <cmath>

class Volatility {
public:
    static TimeSeries rolling_vol(const TimeSeries& returns, size_t window) {
        if (returns.size() < window) {
            throw std::invalid_argument("Window size larger than series");
        }

        TimeSeries vol(returns.size() - window + 1);
        
        for (size_t i = 0; i <= returns.size() - window; ++i) {
            double mean = 0.0;
            for (size_t j = i; j < i + window; ++j) {
                mean += returns[j];
            }
            mean /= window;

            double variance = 0.0;
            for (size_t j = i; j < i + window; ++j) {
                double diff = returns[j] - mean;
                variance += diff * diff;
            }
            variance /= (window - 1);

            vol[i] = std::sqrt(variance * 252);
            if (i + window - 1 < returns.dates.size()) {
                vol.dates[i] = returns.dates[i + window - 1];
            }
        }
        return vol;
    }
};