# Market Regime Attribution Engine

A sophisticated C++ system for detecting market regimes and analyzing strategy performance across different market conditions.

## Overview

This engine uses unsupervised machine learning (K-Means clustering) to identify distinct market regimes based on volatility and drawdown characteristics. It then evaluates strategy performance within each regime to provide regime-conditioned attribution analysis.

## Features

- **Regime Detection**: K-Means clustering on volatility and drawdown metrics
- **Strategy Backtesting**: Test multiple strategies (Buy & Hold, Momentum, Mean Reversion)
- **Regime-Conditioned Analysis**: Performance metrics broken down by market regime
- **Comprehensive Reporting**: Beautiful ASCII reports with regime characteristics and transition matrices

## Architecture

### Core Components

```
include/
├── core/              # Matrix and TimeSeries data structures
├── data/              # CSV reading utilities
├── features/          # Technical indicators (volatility, returns, drawdown)
├── models/            # Machine learning models (K-Means)
├── strategies/        # Trading strategies
└── backtest/          # Backtesting engine and metrics
```

### Key Classes

- **Matrix**: 2D matrix operations for feature engineering
- **TimeSeries**: Time series data container
- **KMeans**: Unsupervised clustering for regime detection
- **Backtester**: Strategy evaluation engine
- **Metrics**: Performance metrics (Sharpe ratio, max drawdown, etc.)

## Building

### Prerequisites
- CMake 3.10+
- MSVC 2022 (or compatible C++17 compiler)
- Windows (Visual Studio)

### Build Steps

```bash
cd market_regime_engine
mkdir build
cd build
cmake ..
cmake --build . --config Debug
```

## Running

```bash
./build/Debug/regime_engine.exe data/sp500.csv
```

The program will:
1. Load price data from CSV
2. Compute volatility and drawdown features
3. Detect 3 market regimes using K-Means
4. Generate a comprehensive regime analysis report
5. Backtest three strategies (Buy & Hold, Momentum, Mean Reversion)
6. Show performance metrics overall and by regime

## Sample Output

```
========================================================================
           REGIME-CONDITIONED STRATEGY ATTRIBUTION REPORT              
========================================================================

📊 REGIME TIMELINE SUMMARY
------------------------------------------------------------------------
Total trading days analyzed: 2516
Number of regimes detected: 3

Regime Characteristics:

  Regime 0 [LOW VOLATILITY]:
    Duration: 1200 days (47.7% of sample)
    Avg Volatility: 12.34%
    Avg Drawdown: 5.23%
    ...
```

## Why Regime Analysis Matters

- **Markets have structure**: Financial markets exhibit distinct behavioral regimes (calm, normal, crisis)
- **Strategy performance varies**: The same strategy performs differently in different regimes
- **Risk misattribution**: Treating returns as stationary leads to incorrect risk assessment
- **Regime persistence**: Markets tend to persist in a regime before switching
- **Improved risk management**: Understanding regime dynamics enhances portfolio construction

## Data Format

Input CSV should have OHLCV data with at least a 'Close' column:
```
Date,Open,High,Low,Close,Volume
2020-01-02,100.50,101.20,100.30,101.00,50000000
2020-01-03,101.00,102.50,100.80,102.30,55000000
...
```

## Performance Metrics

- **Total Return**: Cumulative return over the period
- **Annual Return**: Annualized return estimate
- **Sharpe Ratio**: Risk-adjusted return metric
- **Max Drawdown**: Largest peak-to-trough decline

## Regime Interpretation

The algorithm classifies regimes by volatility levels:
- **Regime 0**: LOW VOLATILITY (calm markets)
- **Regime 1**: NORMAL (typical trading conditions)
- **Regime 2**: HIGH VOLATILITY/CRISIS (stressed markets)

The transition matrix shows how frequently the market moves between regimes.

## Future Enhancements

- [ ] Hidden Markov Model for regime prediction
- [ ] Multi-regime portfolio optimization
- [ ] Risk parity across regimes
- [ ] Real-time regime monitoring
- [ ] More sophisticated feature engineering
- [ ] Support for multiple assets

## License

Proprietary - Market Regime Analysis System

## Author

djswag15 (GitHub)
LinkedIn/email : http://www.linkedin.com/in/darnell-nziga-967695265 / nzigad@gmail.com
