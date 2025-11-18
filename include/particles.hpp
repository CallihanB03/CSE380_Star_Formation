#pragma once

#include <vector>
#include <cstddef> // for size_t

struct Particles {
    size_t N = 0;                    // number of particles

    // Particle properties
    std::vector<float> mass;         // mass
    std::vector<float> x, y, z;      // positions
    std::vector<float> vx, vy, vz;   // velocities
    std::vector<float> temperature;  // simple thermodynamics

    // Optional thermodynamic properties
    std::vector<float> density;      // mass density
    std::vector<float> pressure;     // pressure

    // Constructor
    Particles(size_t n)
        : N(n),
          mass(n, 1.0f),
          x(n, 0.0f), y(n, 0.0f), z(n, 0.0f),
          vx(n, 0.0f), vy(n, 0.0f), vz(n, 0.0f),
          temperature(n, 1.0f),
          density(n, 1.0f),           // default density
          pressure(n, 0.0f)           // default pressure
    {}
};

