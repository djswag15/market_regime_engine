#pragma once
#include "core/TimeSeries.hpp"
#include "strategies/Strategy.hpp"

class Backtester {
public:
    struct BacktestResult {
        TimeSeries equity_curve;
        TimeSeries returns;
        TimeSeries signals;
    };

    static BacktestResult run(const TimeSeries& prices, Strategy& strategy) {
        auto signals = strategy.generate_signals(prices);
        
        BacktestResult result;
        result.signals = signals;
        result.returns = TimeSeries(prices.size() - 1);
        result.equity_curve = TimeSeries(prices.size());
        
        result.equity_curve[0] = 100.0;

        for (size_t i = 1; i < prices.size(); ++i) {
            double price_return = (prices[i] - prices[i-1]) / prices[i-1];
            double strategy_return = signals[i-1] * price_return;
            
            result.returns[i-1] = strategy_return;
            result.equity_curve[i] = result.equity_curve[i-1] * (1.0 + strategy_return);
            
            if (i < prices.dates.size()) {
                result.returns.dates[i-1] = prices.dates[i];
                result.equity_curve.dates[i] = prices.dates[i];
            }
        }

        return result;
    }
};