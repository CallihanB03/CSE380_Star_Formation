// #pragma once

// #include <vector>
// #include <cstddef>  // for size_t
// #include <string>   // for std::string
// #include <fstream>  // for std::ofstream
// #include <iomanip>  // for std::fixed, std::setprecision
// #include <iostream>

// #include "vec3.hpp"


// struct Particles {
//     size_t N = 0;                    // number of particles

//     // Particle properties
//     std::vector<float> mass;         // mass
//     std::vector<float> x, y, z;      // positions
//     std::vector<float> vx, vy, vz;   // velocities
//     std::vector<float> temperature;  // simple thermodynamics
//     std::vector<bool> is_star;  // true if particle forms a star


//     // Optional thermodynamic properties
//     std::vector<float> density;      // mass density
//     std::vector<float> pressure;     // pressure

//     // =============================
//     //       CACHING STRUCTURES
//     // =============================

//     // Cached pairwise displacement vectors (x_j - x_i)
//     // Only stores *one* Vec3 per particle for now (acc caching)
//     std::vector<Vec3> acc_cache;     // cached accelerations

//     // Cached squared distance for each particle pair (flattened matrix)
//     // r2_cache[i * N + j] = |r_j - r_i|^2
//     std::vector<float> r2_cache;

//     // Flag that tells the integrator/gravity solver
//     // whether the cached values are valid for the current positions.
//     bool cache_valid = false;

//     // Constructor
//     Particles(size_t n)
//         : N(n),
//           mass(n, 1.0f),
//           x(n, 0.0f), y(n, 0.0f), z(n, 0.0f),
//           vx(n, 0.0f), vy(n, 0.0f), vz(n, 0.0f),
//           temperature(n, 1.0f),
//           density(n, 1.0f),
//           pressure(n, 0.0f),
//           acc_cache(n, Vec3{0.0f, 0.0f, 0.0f}),
//           r2_cache(n * n, 0.0f),
//           cache_valid(false), is_star(n, false)
//     {}


//     // Output particle data to CSV
//     void write_csv(const std::string& filename) const {
//         std::ofstream file(filename);
//         file << "x,y,z,vx,vy,vz,temperature,density,pressure\n";
//         file << std::fixed << std::setprecision(5);

//         for (size_t i = 0; i < N; i++) {
//             file << x[i] << "," << y[i] << "," << z[i] << ","
//                  << vx[i] << "," << vy[i] << "," << vz[i] << ","
//                  << temperature[i] << "," << density[i] << "," << pressure[i] << "\n";
//         }
//         file.close();
//     }

//     // Simple thermodynamic update for all particles
//     void update_thermodynamics() {
//         // Step 1: compute density
//         for (size_t i = 0; i < N; i++) {
//             // Very basic: assume uniform density proportional to mass
//             // Later you could do SPH summation over neighbors
//             density[i] = mass[i];  // placeholder
//         }

//         // Step 2: compute pressure using ideal gas law
//         for (size_t i = 0; i < N; i++) {
//             pressure[i] = density[i] * temperature[i];
//         }

//         // Step 3: update temperature (optional)
//         // Here we keep it constant, or apply a simple adiabatic-like decay
//         for (size_t i = 0; i < N; i++) {
//             // Example: temperature slowly decreases over time
//             temperature[i] *= 0.999f; 
//         }
//     }
// // Simple thermodynamic update for all particles
// void update_thermodynamics() {
//     // Step 1: compute density
//     for (size_t i = 0; i < N; i++) {
//         // Very basic: assume uniform density proportional to mass
//         // Later you could do SPH summation over neighbors
//         density[i] = mass[i];  // placeholder
//     }

//     // Step 2: compute pressure using ideal gas law
//     for (size_t i = 0; i < N; i++) {
//         pressure[i] = density[i] * temperature[i];
//     }

//     // Step 3: update temperature (optional)
//     // Here we keep it constant, or apply a simple adiabatic-like decay
//     for (size_t i = 0; i < N; i++) {
//         // Example: temperature slowly decreases over time
//         temperature[i] *= 0.999f; 
//     }
// }

// inline void check_star_formation(float rho_threshold) {
//     for (size_t i = 0; i < N; i++) {
//         if (density[i] > rho_threshold) {
//             is_star[i] = true;
//         }
//     }
// }
// };

// include/particles.hpp
#pragma once

#include <vector>
#include <cstddef>
#include <string>
#include <fstream>
#include <iomanip>
#include <iostream>

#include "vec3.hpp"

struct Particles {
    size_t N = 0;

    // particle properties
    std::vector<float> mass;
    std::vector<float> x, y, z;
    std::vector<float> vx, vy, vz;
    std::vector<float> temperature;

    // thermodynamic
    std::vector<float> density;
    std::vector<float> pressure;

    // sink/star bookkeeping
    std::vector<bool> is_star;   // true if particle is a star / sink
    std::vector<bool> alive;     // false -> particle logically removed (merged into a sink)

    // caching
    std::vector<Vec3> acc_cache;
    std::vector<float> r2_cache;
    bool cache_valid = false;

    Particles(size_t n)
        : N(n),
          mass(n, 1.0f),
          x(n, 0.0f), y(n, 0.0f), z(n, 0.0f),
          vx(n, 0.0f), vy(n, 0.0f), vz(n, 0.0f),
          temperature(n, 1.0f),
          density(n, 0.0f),
          pressure(n, 0.0f),
          is_star(n, false),
          alive(n, true),
          acc_cache(n, Vec3{0.0f,0.0f,0.0f}),
          r2_cache(n*n, 0.0f),
          cache_valid(false)
    {}

    // write CSV of *alive* particles only
    void write_csv(const std::string& filename) const {
        std::ofstream file(filename);
        if (!file.is_open()) return;
        file << "x,y,z,vx,vy,vz,temperature,density,pressure,is_star\n";
        file << std::fixed << std::setprecision(5);
        for (size_t i = 0; i < N; ++i) {
            if (!alive[i]) continue;
            file << x[i] << "," << y[i] << "," << z[i] << ","
                 << vx[i] << "," << vy[i] << "," << vz[i] << ","
                 << temperature[i] << "," << density[i] << "," << pressure[i] << ","
                 << (is_star[i] ? 1 : 0) << "\n";
        }
        file.close();
    }

    // mark all dead particles (optional convenience)
    void prune_dead() {
        // Not reshaping internal arrays here (keeps indices stable).
        // If you want to entirely compact arrays, do that in a separate utility.
    }

    // helper: count alive non-star gas particles
    size_t count_alive() const {
        size_t c = 0;
        for (size_t i=0;i<N;++i) if (alive[i]) ++c;
        return c;
    }


    // Simple thermodynamic update for all particles
    void update_thermodynamics() {
        // Step 1: compute density
        for (size_t i = 0; i < N; i++) {
            // Very basic: assume uniform density proportional to mass
            // Later you could do SPH summation over neighbors
            density[i] = mass[i];  // placeholder
        }

        // Step 2: compute pressure using ideal gas law
        for (size_t i = 0; i < N; i++) {
            pressure[i] = density[i] * temperature[i];
        }

        // Step 3: update temperature (optional)
        // Here we keep it constant, or apply a simple adiabatic-like decay
        for (size_t i = 0; i < N; i++) {
            // Example: temperature slowly decreases over time
            temperature[i] *= 0.999f; 
        }
    }

    inline void check_star_formation(float rho_threshold) {
        for (size_t i = 0; i < N; i++) {
            if (density[i] > rho_threshold) {
                is_star[i] = true;
            }
        }
    }
};