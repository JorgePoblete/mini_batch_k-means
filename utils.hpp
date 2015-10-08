#include <chrono>
#include <random>
#include <map>
#include <string>

#ifndef UTILS_H_
#define UTILS_H_

class Utils
{
public:
    Utils();
    void seed_random();
    double get_random();
    void tic(std::string timer);
    double toc(std::string timer);

private:
    std::map<std::string, std::chrono::time_point<std::chrono::system_clock>> start;
};

#endif
