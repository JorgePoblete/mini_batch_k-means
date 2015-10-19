#include <iostream>
#include <cstdlib>
#include <Eigen/Dense>
#include "mini_batch_kmeans.hpp"

using namespace Eigen;

int main(int argc, char** argv)
{
    std::cout.precision(10);
    if (argc < 6){
        std::cout << "/* message */" << std::endl;
        exit (0);
    }
    int k = atoi(argv[1]);
    int n = atoi(argv[2]);
    int d = atoi(argv[3]);
    int b = atoi(argv[4]);
    int t = atoi(argv[5]);
    int buffer_size = atoi(argv[6]);

    std::cout << "K: " << k << std::endl;
    std::cout << "N: " << n << std::endl;
    std::cout << "D: " << d << std::endl;
    std::cout << "b: " << b << std::endl;
    std::cout << "t: " << t << std::endl;
    std::cout << "Read Buffer Size: " << buffer_size << std::endl;


    MiniBatchKMeans kmeans;
    MatrixXdRowMajor buffer = MatrixXdRowMajor::Zero(k, d);

    //read the first k data points to initialize the centroids
    for (int i=0; i<k;i++)
    {
        for (int j=0; j<d; j++)
            std::cin >> buffer(i,j);
    }
    kmeans.init_centroids(buffer,k,b);
    n -= k;
    buffer = MatrixXdRowMajor::Zero(buffer_size, d);
    //read and cluster the next buffer_size data points until the remaining points are less than buffer_size
    while (n > buffer_size)
    {
        for (int i=0; i<buffer_size;i++)
        {
            for (int j=0; j<d; j++)
                std::cin >> buffer(i,j);
        }
        kmeans.cluster(buffer,b,t);
        n -= buffer_size;
    }
    //read and cluster the las data points
    buffer = MatrixXdRowMajor::Zero(n, d);
    for (int i=0; i<n;i++)
    {
        for (int j=0; j<d; j++)
            std::cin >> buffer(i,j);
    }
    kmeans.cluster(buffer,b,t);

    if (k<100)
        std::cout << kmeans.get_centroids() << std::endl;
    return 0;
}
