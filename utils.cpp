#include "utils.hpp"

Utils::Utils()
{
    this->mt = std::mt19937(rd());
    this->rng = std::uniform_real_distribution<double>(0.0, 0.9999999999);
}

double Utils::get_random(){
    return this->rng(this->mt);
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
