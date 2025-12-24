#pragma once
#include <vector>
#include <string>
#include <stdexcept>

class TimeSeries {
public:
    std::vector<double> values;
    std::vector<std::string> dates;

    TimeSeries() = default;
    TimeSeries(size_t n) : values(n), dates(n) {}

    size_t size() const { return values.size(); }
    
    double operator[](size_t i) const { 
        if (i >= values.size()) throw std::out_of_range("Index out of bounds");
        return values[i]; 
    }
    
    double& operator[](size_t i) { 
        if (i >= values.size()) throw std::out_of_range("Index out of bounds");
        return values[i]; 
    }
};