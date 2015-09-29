#include <chrono>
#include <random>

#ifndef UTILS_H_
#define UTILS_H_

class Utils
{
public:
    Utils();
    double get_random();
    void tic();
    double toc();

private:
    std::random_device rd;
    std::mt19937 mt;
    std::uniform_real_distribution<double> rng;
    std::chrono::time_point<std::chrono::system_clock> start;
};

#endif
