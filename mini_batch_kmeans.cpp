#include "mini_batch_kmeans.hpp"

MiniBatchKMeans::MiniBatchKMeans(){}
void MiniBatchKMeans::cluster(const MatrixXdRowMajor& data, int k, int b, int t)
{
    tiempo_promedio = 0.0;
    MatrixXdRowMajor data_points = data.block(0,0,data.rows(),data.cols());
    init_centroids(data_points,k,b);
    MatrixXdRowMajor batch = MatrixXdRowMajor::Zero(b,data_points.cols());
    for (int i=0; i<t; i++)
    {
        utils.tic();
        for (int j=0; j<b; j++)
        {
            int random_pos = utils.get_random() * data_points.rows();
            batch.row(j) = data_points.row(random_pos);
            remove_row(data_points,random_pos);
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
double MiniBatchKMeans::euclid_distance(const VectorXd& A, const VectorXd& B)
{
    return (A-B).norm();
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
void MiniBatchKMeans::remove_row(MatrixXdRowMajor& matrix,unsigned int row_to_remove)
{
    unsigned int num_rows = matrix.rows()-1;
    unsigned int num_cols = matrix.cols();
    if( row_to_remove < num_rows )
        matrix.block(row_to_remove,0,num_rows-row_to_remove,num_cols) = matrix.block(row_to_remove+1,0,num_rows-row_to_remove,num_cols);
    matrix.conservativeResize(num_rows,num_cols);
}
