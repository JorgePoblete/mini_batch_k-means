#ifndef MINI_BATCH_KMEANS_H_
#define MINI_BATCH_KMEANS_H_

#include <iostream>
#include <climits>
#include <Eigen/Dense>
#include "utils.hpp"

using namespace Eigen;

typedef Matrix<double, Dynamic, Dynamic, RowMajor> MatrixXdRowMajor;

#ifdef MIC
    #include <offload.h>
    #define ALLOC alloc_if(1) free_if(0)
    #define FREE alloc_if(0) free_if(1)
    #define REUSE alloc_if(0) free_if(0)
#endif

class MiniBatchKMeans
{
public:
    MiniBatchKMeans();

    MatrixXdRowMajor get_centroids();
    void init_centroids(const MatrixXdRowMajor& data_points, int k, int b);
    void cluster(const MatrixXdRowMajor& data, int k, int b, int t);
private:
    double tiempo_promedio;
    int changed_centroids;
    double error;
    MatrixXdRowMajor centroids;
    VectorXd membership;
    VectorXd points_per_centroid;
    Utils utils;

    #ifdef MIC
        int mic_data_points_count;
        int host_initial_data_point;
        int mic_number_devices;
        double * mic_data_points;
        double * mic_centroids;
        void mic_e_step(const MatrixXdRowMajor& data_points);
        __attribute__((target(mic))) double mic_euclid_distance(double *a, double *b, int d);
        __attribute__((target(mic))) int mic_find_nearest_centroid(double *mic_data_point, double *mic_centroids, int k, int d);
    #else
        void e_step(const MatrixXdRowMajor& data_points);
    #endif
    int find_nearest_centroid(const RowVectorXd& data_point);
    void m_step(const MatrixXdRowMajor& data_points);
};

#endif
