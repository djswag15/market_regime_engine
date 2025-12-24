#include "data/CSVReader.hpp"
#include "features/Returns.hpp"
#include "features/Volatility.hpp"
#include "features/Drawdown.hpp"
#include "models/KMeans.hpp"
#include "strategies/BuyHold.hpp"
#include "strategies/Momentum.hpp"
#include "strategies/MeanReversion.hpp"
#include "backtest/Backtester.hpp"
#include "backtest/Metrics.hpp"

#include <iostream>
#include <iomanip>

void print_regime_stats(const std::vector<int>& regimes, size_t num_regimes) {
    std::vector<int> counts(num_regimes, 0);
    for (int r : regimes) {
        counts[r]++;
    }

    std::cout << "\n=== Regime Statistics ===" << std::endl;
    for (size_t i = 0; i < num_regimes; ++i) {
        double pct = 100.0 * counts[i] / regimes.size();
        std::cout << "Regime " << i << ": " << counts[i] 
                  << " periods (" << std::fixed << std::setprecision(1) 
                  << pct << "%)" << std::endl;
    }
}

void print_strategy_performance(const std::string& name, 
                               const Backtester::BacktestResult& result) {
    double sharpe = Metrics::sharpe(result.returns);
    double max_dd = Metrics::max_drawdown(result.equity_curve);
    double total_ret = Metrics::total_return(result.equity_curve);
    double annual_ret = Metrics::annual_return(result.returns);

    std::cout << "\n" << name << ":" << std::endl;
    std::cout << "  Total Return: " << std::fixed << std::setprecision(2) 
              << total_ret * 100 << "%" << std::endl;
    std::cout << "  Annual Return: " << annual_ret * 100 << "%" << std::endl;
    std::cout << "  Sharpe Ratio: " << std::setprecision(3) << sharpe << std::endl;
    std::cout << "  Max Drawdown: " << std::setprecision(2) 
              << max_dd * 100 << "%" << std::endl;
}

void print_regime_performance(Strategy& strat, const TimeSeries& prices,
                             const std::vector<int>& regimes, size_t num_regimes) {
    auto full_result = Backtester::run(prices, strat);
    
    std::cout << "\n=== " << strat.name() << " by Regime ===" << std::endl;
    
    for (size_t regime = 0; regime < num_regimes; ++regime) {
        TimeSeries regime_returns;
        
        for (size_t i = 0; i < regimes.size() && i < full_result.returns.size(); ++i) {
            if (regimes[i] == static_cast<int>(regime)) {
                regime_returns.values.push_back(full_result.returns[i]);
            }
        }

        if (regime_returns.size() > 0) {
            double regime_sharpe = Metrics::sharpe(regime_returns);
            double regime_annual = Metrics::annual_return(regime_returns);
            
            std::cout << "  Regime " << regime << ": " 
                      << "Ann. Return = " << std::fixed << std::setprecision(2)
                      << regime_annual * 100 << "%, Sharpe = " 
                      << std::setprecision(3) << regime_sharpe << std::endl;
        }
    }
}

int main(int argc, char* argv[]) {
    try {
        std::cout << "=== Market Regime & Strategy Attribution Engine ===" << std::endl;
        
        std::string data_path = (argc > 1) ? argv[1] : "data/sp500.csv";
        std::cout << "\nLoading data from: " << data_path << std::endl;
        
        auto prices = CSVReader::read_price_series(data_path);
        std::cout << "Loaded " << prices.size() << " price observations" << std::endl;

        std::cout << "\nComputing features..." << std::endl;
        auto returns = Returns::log_returns(prices);
        auto vol = Volatility::rolling_vol(returns, 20);
        auto dd = Drawdown::rolling_drawdown(prices, 20);

        size_t min_size = std::min({vol.size(), dd.size()});
        Matrix X(min_size, 2);
        
        for (size_t i = 0; i < min_size; ++i) {
            X(i, 0) = vol[i];
            X(i, 1) = dd[i];
        }

        std::cout << "\nDetecting market regimes..." << std::endl;
        size_t num_regimes = 3;
        KMeans km(num_regimes, 100, 1e-4);
        auto regimes = km.fit_predict(X);
        
        std::cout << "Regimes detected with inertia: " << km.get_inertia() << std::endl;
        print_regime_stats(regimes, num_regimes);

        std::cout << "\n=== Overall Strategy Performance ===" << std::endl;
        
        BuyHold bh_strat;
        auto bh_result = Backtester::run(prices, bh_strat);
        print_strategy_performance(bh_strat.name(), bh_result);

        Momentum mom_strat(20);
        auto mom_result = Backtester::run(prices, mom_strat);
        print_strategy_performance(mom_strat.name(), mom_result);

        MeanReversion mr_strat(20, 1.5);
        auto mr_result = Backtester::run(prices, mr_strat);
        print_strategy_performance(mr_strat.name(), mr_result);

        std::cout << "\n=== Regime-Conditioned Performance ===" << std::endl;
        print_regime_performance(bh_strat, prices, regimes, num_regimes);
        print_regime_performance(mom_strat, prices, regimes, num_regimes);
        print_regime_performance(mr_strat, prices, regimes, num_regimes);

        std::cout << "\n=== Analysis Complete ===" << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}