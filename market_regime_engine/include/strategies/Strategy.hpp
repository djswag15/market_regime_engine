#pragma once
#include "core/TimeSeries.hpp"
#include <string>

class Strategy {
public:
    virtual ~Strategy() = default;
    virtual TimeSeries generate_signals(const TimeSeries& prices) = 0;
    virtual std::string name() const = 0;
};