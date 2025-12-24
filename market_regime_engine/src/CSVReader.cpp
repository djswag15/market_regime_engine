#include "data/CSVReader.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

// Helper function to parse CSV line properly handling quoted fields
std::vector<std::string> parse_csv_line(const std::string& line) {
    std::vector<std::string> fields;
    std::string field;
    bool in_quotes = false;
    
    for (size_t i = 0; i < line.length(); ++i) {
        char c = line[i];
        
        if (c == '"') {
            in_quotes = !in_quotes;
        } else if (c == ',' && !in_quotes) {
            fields.push_back(field);
            field.clear();
        } else {
            field += c;
        }
    }
    fields.push_back(field);
    
    return fields;
}

// Helper function to clean a CSV value (remove quotes and commas from numbers)
std::string clean_value(std::string value) {
    // Remove leading/trailing whitespace
    while (!value.empty() && (value.front() == ' ' || value.front() == '"')) {
        value = value.substr(1);
    }
    while (!value.empty() && (value.back() == ' ' || value.back() == '"')) {
        value = value.substr(0, value.length() - 1);
    }
    // Remove all commas (for thousand separators in numbers)
    value.erase(std::remove(value.begin(), value.end(), ','), value.end());
    return value;
}

TimeSeries CSVReader::read_price_series(const std::string& path, 
                                       const std::string& price_col) {
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + path);
    }

    TimeSeries series;
    std::string line;
    
    if (!std::getline(file, line)) {
        throw std::runtime_error("Empty file");
    }

    auto headers = parse_csv_line(line);
    for (auto& h : headers) {
        h = clean_value(h);
    }

    int price_idx = -1;
    int date_idx = -1;
    for (size_t i = 0; i < headers.size(); ++i) {
        if (headers[i] == price_col) {
            price_idx = static_cast<int>(i);
        }
        if (headers[i] == "Date" || headers[i] == "date") {
            date_idx = static_cast<int>(i);
        }
    }

    if (price_idx == -1) {
        throw std::runtime_error("Price column not found: " + price_col);
    }

    while (std::getline(file, line)) {
        auto fields = parse_csv_line(line);
        
        std::string date_str;
        double price = 0.0;

        if (static_cast<int>(fields.size()) > price_idx && price_idx >= 0) {
            price = std::stod(clean_value(fields[price_idx]));
        }
        if (date_idx >= 0 && static_cast<int>(fields.size()) > date_idx) {
            date_str = clean_value(fields[date_idx]);
        }

        series.values.push_back(price);
        series.dates.push_back(date_str);
    }

    std::cout << "Read " << series.size() << " rows from " << path << std::endl;
    return series;
}