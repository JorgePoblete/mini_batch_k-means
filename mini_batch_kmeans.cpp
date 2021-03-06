#include "mini_batch_kmeans.hpp"

MiniBatchKMeans::MiniBatchKMeans(){}
void MiniBatchKMeans::cluster(const MatrixXdRowMajor& data_points, int b, int t)
{
    tiempo_promedio = 0.0;
    MatrixXdRowMajor old_centroids;
    MatrixXdRowMajor batch = MatrixXdRowMajor::Zero(b,data_points.cols());
    double error;
    utils.seed_random();
    for (int i=0; i<t; i++)
    {
        utils.tic("I");
        for (int j=0; j<b; j++)
        {
            double r = utils.get_random();
            batch.row(j) = data_points.row(r*data_points.rows());
        }
        old_centroids = centroids;
        #ifdef MIC
            mic_e_step(batch);
        #else
            e_step(batch);
        #endif
        m_step(batch);
        error = (centroids - old_centroids).rowwise().norm().maxCoeff();
        double tiempo_parcial = utils.toc("I");
        tiempo_promedio += tiempo_parcial;
        std::cout<<"Iteration: "<<i+1<<"\ttime: "<< tiempo_parcial;
        std::cout<<"("<< (tiempo_promedio/(i+1)) << ")";
        std::cout<<"\tError: " << error << std::endl;
    }
    std::cout<<"Total Time: " << tiempo_promedio << std::endl;
}
void MiniBatchKMeans::init_centroids(const MatrixXdRowMajor& data_points, int k, int b)
{
    centroids = data_points.block(0,0,k,data_points.cols());
    membership = VectorXd::Zero(b);
    points_per_centroid = VectorXd::Zero(k);

    #ifdef MIC
    mic_number_devices = _Offload_number_of_devices();
    std::cout << "Number of MICs on the system: " << mic_number_devices << std::endl;

    double mic_workload = 0.8;
	mic_data_points_count = (b * (mic_workload/(double)mic_number_devices));
    host_initial_data_point = mic_number_devices * mic_data_points_count;
	int data_points_length = mic_data_points_count * data_points.cols();
	int centroids_length = k * data_points.cols();
    for (int i=0; i<mic_number_devices; i++)
    {
	   	std::cout << "MIC" << i << ": " << " data count " << mic_data_points_count << std::endl;
	    utils.tic("ALLOC");
	    #pragma offload target(mic:i)\
        nocopy(mic_data_points[0:data_points_length]: ALLOC)\
        nocopy(mic_centroids[0:centroids_length]: ALLOC)
	    {}
	    std::cout << "MIC" << i << ": " << " memmory allocated in " << utils.toc("ALLOC") << "secs" << std::endl;
    }
    #endif
}
#ifdef MIC
void MiniBatchKMeans::mic_e_step(const MatrixXdRowMajor& data_points)
{
    int k = centroids.rows();
    int n = data_points.rows();
    int d = data_points.cols();

    double *host_data_points = (double *)data_points.data();
    double *host_centroids = centroids.data();
    double *host_membership = membership.data();
	int data_points_count = mic_data_points_count;
	double *mic_signal;
    for (int i=0; i<mic_number_devices; i++)
    {
	    #pragma offload target(mic:i) \
	    in(host_data_points[0:data_points_count*d]: REUSE into(mic_data_points[0:data_points_count*d]))\
        in(host_centroids[0:k*d]: REUSE into(mic_centroids[0:k*d])) \
	    out(host_membership[0:data_points_count]) signal(mic_signal)
	    {
	        #pragma omp parallel
	        {
	            #pragma omp for nowait
	            for (int j=0; j<data_points_count; j++)
	            {
	                host_membership[j] = mic_find_nearest_centroid(j * d + mic_data_points,mic_centroids, k, d);
	            }
	        }
	    }
        host_data_points += mic_data_points_count*d;
		host_membership += mic_data_points_count;
	}
    #pragma omp parallel
    {
        #pragma omp for nowait
        for (int i=host_initial_data_point; i<data_points.rows(); i++)
        {
            membership(i) = find_nearest_centroid(data_points.row(i));
        }
    }
    for (int i=0; i<mic_number_devices; i++)
    {
    	#pragma offload_wait target(mic:i) wait(mic_signal)
    }
}
__attribute__((target(mic)))
int MiniBatchKMeans::mic_find_nearest_centroid(double *mic_data_point, double *mic_centroids, int k, int d)
{
    int nearest_centroid_pos = 0;
    double dist;
    double min_dist = mic_euclid_distance(mic_data_point, mic_centroids, d);
    for (int i=1; i<k; i++)
    {
        dist = mic_euclid_distance(i * d + mic_centroids, mic_data_point, d);
        if (dist < min_dist)
        {
            min_dist = dist;
            nearest_centroid_pos = i;
        }
    }
    return nearest_centroid_pos;
}
__attribute__((target(mic)))
double MiniBatchKMeans::mic_euclid_distance(double *a, double *b, int d)
{
    double dist = 0.0;
    #pragma omp simd
    for (int i=0; i<d; i++)
        dist += (a[i] - b[i]) * (a[i] - b[i]);
    return sqrt(dist);
}
#else
void MiniBatchKMeans::e_step(const MatrixXdRowMajor& data_points)
{
    #pragma omp parallel
    {
        #pragma omp for nowait
        for (int i=0; i<data_points.rows(); i++)
        {
            membership(i) = find_nearest_centroid(data_points.row(i));
        }
    }
}
#endif
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
