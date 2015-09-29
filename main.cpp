#include <iostream>
#include <cstdlib>
#include <Eigen/Dense>
#include "mini_batch_kmeans.hpp"

using namespace Eigen;

int main(int argc, char** argv)
{
    std::cout.precision(10);
    if (argc < 4){
        exit (0);
    }
    int k = atoi(argv[1]);
    int n = atoi(argv[2]);
    int d = atoi(argv[3]);
    int b = atoi(argv[4]);
    int t = atoi(argv[5]);
    MatrixXdRowMajor data_points = MatrixXdRowMajor::Zero(n,d);
    for (int i=0; i<n;i++)
    {
        for (int j=0; j<d; j++)
            std::cin >> data_points(i,j);

    }
    std::cout << "rows: " << data_points.rows() << std::endl;
    std::cout << "cols: " << data_points.cols() << std::endl;
    MiniBatchKMeans kmeans;
    kmeans.cluster(data_points,k,b,t);
    std::cout << kmeans.get_centroids() << std::endl;
    return 0;
}
