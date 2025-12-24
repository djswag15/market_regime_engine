#include "models/KMeans.hpp"
#include <iostream>
#include <random>
#include <limits>
#include <cmath>

void KMeans::initialize_centroids(const Matrix& X) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(0, static_cast<int>(X.rows) - 1);

    centroids_ = Matrix(k_, X.cols);

    size_t first_idx = dis(gen);
    for (size_t j = 0; j < X.cols; ++j) {
        centroids_(0, j) = X(first_idx, j);
    }

    for (size_t c = 1; c < k_; ++c) {
        std::vector<double> distances(X.rows, std::numeric_limits<double>::max());
        
        for (size_t i = 0; i < X.rows; ++i) {
            auto row = X.get_row(i);
            for (size_t existing_c = 0; existing_c < c; ++existing_c) {
                auto centroid = centroids_.get_row(existing_c);
                double dist = euclidean_distance(row, centroid);
                distances[i] = std::min(distances[i], dist);
            }
        }

        double sum = 0.0;
        for (double d : distances) sum += d * d;
        
        std::uniform_real_distribution<> prob_dis(0.0, sum);
        double target = prob_dis(gen);
        
        double cumsum = 0.0;
        size_t chosen_idx = 0;
        for (size_t i = 0; i < distances.size(); ++i) {
            cumsum += distances[i] * distances[i];
            if (cumsum >= target) {
                chosen_idx = i;
                break;
            }
        }

        for (size_t j = 0; j < X.cols; ++j) {
            centroids_(c, j) = X(chosen_idx, j);
        }
    }
}

std::vector<int> KMeans::assign_clusters(const Matrix& X) {
    std::vector<int> labels(X.rows);
    inertia_ = 0.0;

    for (size_t i = 0; i < X.rows; ++i) {
        auto row = X.get_row(i);
        double min_dist = std::numeric_limits<double>::max();
        int best_cluster = 0;

        for (size_t c = 0; c < k_; ++c) {
            auto centroid = centroids_.get_row(c);
            double dist = euclidean_distance(row, centroid);
            
            if (dist < min_dist) {
                min_dist = dist;
                best_cluster = static_cast<int>(c);
            }
        }

        labels[i] = best_cluster;
        inertia_ += min_dist * min_dist;
    }

    return labels;
}

bool KMeans::update_centroids(const Matrix& X, const std::vector<int>& labels) {
    Matrix new_centroids(k_, X.cols);
    std::vector<int> counts(k_, 0);

    for (size_t i = 0; i < X.rows; ++i) {
        int cluster = labels[i];
        counts[cluster]++;
        for (size_t j = 0; j < X.cols; ++j) {
            new_centroids(cluster, j) += X(i, j);
        }
    }

    for (size_t c = 0; c < k_; ++c) {
        if (counts[c] > 0) {
            for (size_t j = 0; j < X.cols; ++j) {
                new_centroids(c, j) /= counts[c];
            }
        }
    }

    double movement = 0.0;
    for (size_t c = 0; c < k_; ++c) {
        auto old_centroid = centroids_.get_row(c);
        auto new_centroid = new_centroids.get_row(c);
        movement += euclidean_distance(old_centroid, new_centroid);
    }

    centroids_ = new_centroids;
    return movement < tolerance_;
}

double KMeans::euclidean_distance(const std::vector<double>& a, 
                                 const std::vector<double>& b) const {
    double sum = 0.0;
    for (size_t i = 0; i < a.size(); ++i) {
        double diff = a[i] - b[i];
        sum += diff * diff;
    }
    return std::sqrt(sum);
}

std::vector<int> KMeans::fit_predict(const Matrix& X) {
    if (X.rows < k_) {
        throw std::invalid_argument("Number of samples must be >= k");
    }

    initialize_centroids(X);

    for (size_t iter = 0; iter < max_iters_; ++iter) {
        auto labels = assign_clusters(X);
        bool converged = update_centroids(X, labels);

        if (converged) {
            std::cout << "K-Means converged at iteration " << iter << std::endl;
            return labels;
        }
    }

    std::cout << "K-Means reached max iterations" << std::endl;
    return assign_clusters(X);
}