#pragma once
#include <vector>

struct Particles {
    std::vector<float> x, y, z;
    std::vector<float> vx, vy, vz;
    std::vector<float> mass;

    std::vector<float> density;
    std::vector<float> pressure;
    std::vector<float> temperature;

    size_t N = 0;

    Particles(size_t n) : 
        x(n), y(n), z(n), 
        vx(n), vy(n), vz(n), 
        mass(n, 1.0f),
        density(n, 0.0f),
        pressure(n, 0.0f),
        temperature(n, 1.0f),
        N(n) 
    {}
};
