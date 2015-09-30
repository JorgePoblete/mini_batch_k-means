#include <cstdlib>
#include <iostream>
#include <ctime>
#include <chrono>
#include <random>

#ifndef UTILS_H_
#define UTILS_H_

class Utils
{
public:
    Utils();
    void seed_random();
    double get_random();
    void tic();
    double toc();

private:
    std::chrono::time_point<std::chrono::system_clock> start;
};

#endif
