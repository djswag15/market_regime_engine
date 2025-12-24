#pragma once
#include "strategies/Strategy.hpp"

class BuyHold : public Strategy {
public:
    TimeSeries generate_signals(const TimeSeries& prices) override {
        TimeSeries signals(prices.size());
        for (size_t i = 0; i < signals.size(); ++i) {
            signals[i] = 1.0;
            if (i < prices.dates.size()) {
                signals.dates[i] = prices.dates[i];
            }
        }
        return signals;
    }

    std::string name() const override { return "Buy & Hold"; }
};