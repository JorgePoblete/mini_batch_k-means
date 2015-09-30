#include "utils.hpp"

Utils::Utils()
{

}
void Utils::seed_random(){
     std::srand(std::time(0));
}
double Utils::get_random(){
    return ((double)rand() / (double)(RAND_MAX));
}
void Utils::tic()
{
    this->start = std::chrono::system_clock::now();
}
double Utils::toc()
{
    std::chrono::duration<double> elapsed_seconds = std::chrono::system_clock::now() - this->start;
    return elapsed_seconds.count();
}
