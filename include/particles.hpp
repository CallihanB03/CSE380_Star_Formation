#pragma once

#include <vector>
#include <cstddef>
#include <string>
#include <fstream>
#include <iomanip>
#include <iostream>

#include "vec3.hpp"
#include "stars.hpp"

struct Particles {
    size_t N = 0;

    // particle properties
    std::vector<float> mass;
    std::vector<float> x, y, z;
    std::vector<float> vx, vy, vz;
    std::vector<float> ax, ay, az; // caching for reuse and O(N log N) gravity computation
    std::vector<float> temperature;

    // thermodynamic
    std::vector<float> density;
    std::vector<float> pressure;

    // Optimization 1: 
    
    // sink/star bookkeeping
    // std::vector<bool> is_star;   // true if particle is a star / sink
    // std::vector<bool> alive;

    std::vector<uint8_t> is_star;   // true if particle is a star / sink
    std::vector<uint8_t> alive;


    Particles(size_t n)
        : N(n),
          mass(n, 1.0f),
          x(n, 0.0f), y(n, 0.0f), z(n, 0.0f),
          vx(n, 0.0f), vy(n, 0.0f), vz(n, 0.0f),
          ax(n, 0.0f), ay(n, 0.0f), az(n, 0.0f),
          temperature(n, 1.0f),
          density(n, 0.0f),
          pressure(n, 0.0f),
          is_star(n, false),
          alive(n, true)
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

    size_t count_particles_in_stars() const {
        size_t count = 0;
        for (size_t i = 0; i < N; i++) {
            if (is_star[i]) count++;
        }
        return count;
    }

};