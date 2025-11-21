#pragma once

#include <vector>
#include <cstddef> // for size_t
#include "vec3.hpp"  // so we can store Vec3 in the cache

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

    // =============================
    //       CACHING STRUCTURES
    // =============================

    // Cached pairwise displacement vectors (x_j - x_i)
    // Only stores *one* Vec3 per particle for now (acc caching)
    std::vector<Vec3> acc_cache;     // cached accelerations

    // Cached squared distance for each particle pair (flattened matrix)
    // r2_cache[i * N + j] = |r_j - r_i|^2
    std::vector<float> r2_cache;

    // Flag that tells the integrator/gravity solver
    // whether the cached values are valid for the current positions.
    bool cache_valid = false;

    // Constructor
    Particles(size_t n)
        : N(n),
          mass(n, 1.0f),
          x(n, 0.0f), y(n, 0.0f), z(n, 0.0f),
          vx(n, 0.0f), vy(n, 0.0f), vz(n, 0.0f),
          temperature(n, 1.0f),
          density(n, 1.0f),
          pressure(n, 0.0f),
          acc_cache(n, Vec3{0.0f, 0.0f, 0.0f}),
          r2_cache(n * n, 0.0f),
          cache_valid(false)
    {}
};

