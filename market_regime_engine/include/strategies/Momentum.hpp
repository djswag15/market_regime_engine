#pragma once
#include "strategies/Strategy.hpp"

class Momentum : public Strategy {
public:
    explicit Momentum(size_t lookback) : lookback_(lookback) {}

    TimeSeries generate_signals(const TimeSeries& prices) override {
        if (prices.size() < lookback_) {
            throw std::invalid_argument("Price series too short for lookback");
        }

        TimeSeries signals(prices.size());
        
        for (size_t i = 0; i < lookback_; ++i) {
            signals[i] = 0.0;
            if (i < prices.dates.size()) {
                signals.dates[i] = prices.dates[i];
            }
        }

        for (size_t i = lookback_; i < prices.size(); ++i) {
            double price_change = prices[i] - prices[i - lookback_];
            signals[i] = (price_change > 0) ? 1.0 : -1.0;
            if (i < prices.dates.size()) {
                signals.dates[i] = prices.dates[i];
            }
        }

        return signals;
    }

    std::string name() const override { 
        return "Momentum(" + std::to_string(lookback_) + ")"; 
    }

private:
    size_t lookback_;
};