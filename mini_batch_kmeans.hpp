#ifndef MINI_BATCH_KMEANS_H_
#define MINI_BATCH_KMEANS_H_

#include <iostream>
#include <climits>
#include <Eigen/Dense>
#include "utils.hpp"

using namespace Eigen;

typedef Matrix<double, Dynamic, Dynamic, RowMajor> MatrixXdRowMajor;

class MiniBatchKMeans
{
public:
    MiniBatchKMeans();

    MatrixXdRowMajor get_centroids();
    void cluster(const MatrixXdRowMajor& data, int k, int b, int t);
private:
    double tiempo_promedio;
    int changed_centroids;
    double error;
    MatrixXdRowMajor centroids;
    VectorXd membership;
    VectorXd points_per_centroid;
    Utils utils;

    void e_step(const MatrixXdRowMajor& data_points);
    int find_nearest_centroid(const RowVectorXd& data_point);
    void init_centroids(const MatrixXdRowMajor& data_points, int k, int b);
    void m_step(const MatrixXdRowMajor& data_points);
    double euclid_distance(const VectorXd& A, const VectorXd& B);
};

#endif
