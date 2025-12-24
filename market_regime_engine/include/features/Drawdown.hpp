#pragma once
#include "core/TimeSeries.hpp"
#include <algorithm>

class Drawdown {
public:
    static TimeSeries rolling_drawdown(const TimeSeries& prices, size_t window) {
        if (prices.size() < window) {
            throw std::invalid_argument("Window size larger than series");
        }

        TimeSeries dd(prices.size() - window + 1);
        
        for (size_t i = 0; i <= prices.size() - window; ++i) {
            double max_price = prices[i];
            for (size_t j = i; j < i + window; ++j) {
                max_price = std::max(max_price, prices[j]);
            }
            double current_price = prices[i + window - 1];
            dd[i] = (current_price - max_price) / max_price;
            
            if (i + window - 1 < prices.dates.size()) {
                dd.dates[i] = prices.dates[i + window - 1];
            }
        }
        return dd;
    }

    static double max_drawdown(const TimeSeries& equity_curve) {
        if (equity_curve.size() == 0) return 0.0;
        
        double max_dd = 0.0;
        double peak = equity_curve[0];
        
        for (size_t i = 1; i < equity_curve.size(); ++i) {
            if (equity_curve[i] > peak) {
                peak = equity_curve[i];
            }
            double dd = (equity_curve[i] - peak) / peak;
            max_dd = std::min(max_dd, dd);
        }
        return max_dd;
    }
};