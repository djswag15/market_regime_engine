#pragma once
#include "core/Matrix.hpp"
#include <vector>

class KMeans {
public:
    KMeans(size_t k, size_t max_iters = 100, double tolerance = 1e-4)
        : k_(k), max_iters_(max_iters), tolerance_(tolerance) {}

    std::vector<int> fit_predict(const Matrix& X);
    
    const Matrix& get_centroids() const { return centroids_; }
    double get_inertia() const { return inertia_; }

private:
    size_t k_;
    size_t max_iters_;
    double tolerance_;
    Matrix centroids_{0, 0};
    double inertia_ = 0.0;

    void initialize_centroids(const Matrix& X);
    std::vector<int> assign_clusters(const Matrix& X);
    bool update_centroids(const Matrix& X, const std::vector<int>& labels);
    double euclidean_distance(const std::vector<double>& a, 
                             const std::vector<double>& b) const;
};