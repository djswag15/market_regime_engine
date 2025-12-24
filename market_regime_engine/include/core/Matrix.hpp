#pragma once
#include <vector>
#include <stdexcept>

class Matrix {
public:
    size_t rows, cols;
    std::vector<double> data;

    Matrix(size_t r, size_t c) : rows(r), cols(c), data(r * c, 0.0) {}

    double& operator()(size_t i, size_t j) {
        if (i >= rows || j >= cols) throw std::out_of_range("Matrix index out of bounds");
        return data[i * cols + j];
    }

    double operator()(size_t i, size_t j) const {
        if (i >= rows || j >= cols) throw std::out_of_range("Matrix index out of bounds");
        return data[i * cols + j];
    }

    std::vector<double> get_row(size_t i) const {
        if (i >= rows) throw std::out_of_range("Row index out of bounds");
        std::vector<double> row(cols);
        for (size_t j = 0; j < cols; ++j) {
            row[j] = data[i * cols + j];
        }
        return row;
    }
};