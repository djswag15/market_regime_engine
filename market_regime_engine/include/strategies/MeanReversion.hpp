#pragma once
#include "strategies/Strategy.hpp"
#include <cmath>

class MeanReversion : public Strategy {
public:
    explicit MeanReversion(size_t window, double threshold = 1.0) 
        : window_(window), threshold_(threshold) {}

    TimeSeries generate_signals(const TimeSeries& prices) override {
        if (prices.size() < window_) {
            throw std::invalid_argument("Price series too short for window");
        }

        TimeSeries signals(prices.size());
        
        for (size_t i = 0; i < window_; ++i) {
            signals[i] = 0.0;
            if (i < prices.dates.size()) {
                signals.dates[i] = prices.dates[i];
            }
        }

        for (size_t i = window_; i < prices.size(); ++i) {
            double mean = 0.0;
            for (size_t j = i - window_; j < i; ++j) {
                mean += prices[j];
            }
            mean /= window_;

            double variance = 0.0;
            for (size_t j = i - window_; j < i; ++j) {
                double diff = prices[j] - mean;
                variance += diff * diff;
            }
            variance /= (window_ - 1);
            double std_dev = std::sqrt(variance);

            double z_score = (std_dev > 1e-8) ? (prices[i] - mean) / std_dev : 0.0;

            if (z_score > threshold_) {
                signals[i] = -1.0;
            } else if (z_score < -threshold_) {
                signals[i] = 1.0;
            } else {
                signals[i] = 0.0;
            }

            if (i < prices.dates.size()) {
                signals.dates[i] = prices.dates[i];
            }
        }

        return signals;
    }

    std::string name() const override { 
        return "MeanReversion(" + std::to_string(window_) + ")"; 
    }

private:
    size_t window_;
    double threshold_;
};