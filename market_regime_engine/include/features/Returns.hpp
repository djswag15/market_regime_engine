#pragma once
#include "core/TimeSeries.hpp"
#include <cmath>

class Returns {
public:
    static TimeSeries log_returns(const TimeSeries& prices) {
        if (prices.size() < 2) throw std::invalid_argument("Need at least 2 prices");
        
        TimeSeries returns(prices.size() - 1);
        for (size_t i = 1; i < prices.size(); ++i) {
            if (prices[i-1] <= 0 || prices[i] <= 0) {
                throw std::invalid_argument("Prices must be positive");
            }
            returns[i-1] = std::log(prices[i] / prices[i-1]);
            if (i < prices.dates.size()) {
                returns.dates[i-1] = prices.dates[i];
            }
        }
        return returns;
    }
};