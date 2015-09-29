#include "mini_batch_kmeans.hpp"

MiniBatchKMeans::MiniBatchKMeans(){}
void MiniBatchKMeans::cluster(const MatrixXdRowMajor& data_points, int k, int b, int t)
{
    tiempo_promedio = 0.0;
    init_centroids(data_points,k,b);
    MatrixXdRowMajor batch = MatrixXdRowMajor::Zero(b,data_points.cols());
    for (int i=0; i<t; i++)
    {
        utils.tic();
        for (int j=0; j<b; j++)
        {
            int random_pos = utils.get_random() * data_points.rows();
            batch.row(j) = data_points.row(random_pos);
        }
        e_step(batch);
        m_step(batch);
        double tiempo_parcial = utils.toc();
        tiempo_promedio += tiempo_parcial;
        std::cout<<"Iteration: "<<i+1<<"\ttime: "<< tiempo_parcial;
        std::cout<<"("<< (tiempo_promedio/(i+1)) << ")" << std::endl;
    }
}
void MiniBatchKMeans::init_centroids(const MatrixXdRowMajor& data_points, int k, int b)
{
    centroids = data_points.block(0,0,k,data_points.cols());
    membership = VectorXd::Zero(b);
    points_per_centroid = VectorXd::Zero(k);
}
void MiniBatchKMeans::e_step(const MatrixXdRowMajor& data_points)
{
    #pragma omp parallel for
    for (int i=0; i<data_points.rows(); i++)
    {
        membership(i) = find_nearest_centroid(data_points.row(i));
    }
}
int MiniBatchKMeans::find_nearest_centroid(const RowVectorXd& data_point)
{
    MatrixXf::Index index;
    (centroids.rowwise() - data_point).rowwise().norm().minCoeff(&index);
    return index;
}
void MiniBatchKMeans::m_step(const MatrixXdRowMajor& data_points)
{
    double learning_rate;
    int centroid;
    for (int i=0; i<data_points.rows(); i++)
    {
        centroid = membership(i);
        points_per_centroid(centroid)++;
        learning_rate = 1 / points_per_centroid(centroid);
        centroids.row(centroid) = ((1 - learning_rate)*centroids.row(centroid)) + (learning_rate * data_points.row(i));
    }
}
MatrixXdRowMajor MiniBatchKMeans::get_centroids()
{
    return centroids;
}
