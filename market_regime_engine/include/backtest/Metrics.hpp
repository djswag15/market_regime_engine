#pragma once
#include "core/TimeSeries.hpp"
#include "features/Drawdown.hpp"
#include <cmath>

class Metrics {
public:
    static double sharpe(const TimeSeries& returns, double risk_free_rate = 0.0) {
        if (returns.size() == 0) return 0.0;

        double mean = 0.0;
        for (double r : returns.values) {
            mean += r;
        }
        mean /= returns.size();

        double variance = 0.0;
        for (double r : returns.values) {
            double diff = r - mean;
            variance += diff * diff;
        }
        variance /= (returns.size() - 1);
        double std_dev = std::sqrt(variance);

        if (std_dev < 1e-8) return 0.0;

        double annualized_mean = mean * 252 - risk_free_rate;
        double annualized_std = std_dev * std::sqrt(252);
        
        return annualized_mean / annualized_std;
    }

    static double max_drawdown(const TimeSeries& equity_curve) {
        return Drawdown::max_drawdown(equity_curve);
    }

    static double total_return(const TimeSeries& equity_curve) {
        if (equity_curve.size() < 2) return 0.0;
        return (equity_curve[equity_curve.size() - 1] - equity_curve[0]) / equity_curve[0];
    }

    static double annual_return(const TimeSeries& returns) {
        if (returns.size() == 0) return 0.0;
        
        double mean_daily = 0.0;
        for (double r : returns.values) {
            mean_daily += r;
        }
        mean_daily /= returns.size();
        
        return mean_daily * 252;
    }
};