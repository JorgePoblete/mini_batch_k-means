#include "utils.hpp"

Utils::Utils(){}
void Utils::seed_random(){
     std::srand(std::time(0));
}
double Utils::get_random(){
    return ((double)rand() / (double)(RAND_MAX));
}
void Utils::tic(std::string timer)
{
    this->start[timer] = std::chrono::system_clock::now();
}
double Utils::toc(std::string timer)
{
    if (start.find(timer) != start.end())
    {
        std::chrono::duration<double> elapsed_seconds = std::chrono::system_clock::now() - this->start[timer];
        return elapsed_seconds.count();
    }
    return 0.0;
}
