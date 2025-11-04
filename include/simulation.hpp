#pragma once
#include "config.hpp"
#include <vector>

class Simulation {
public:
    Simulation(const Config& config);
    void run();

private:
    Config cfg;
    std::vector<double> density;
    std::vector<double> velocity;
    std::vector<double> potential;

    void initialize();
    void step();
    void output(int step);
};
