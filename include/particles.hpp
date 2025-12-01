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
    std::vector<float> temperature;

    // thermodynamic
    std::vector<float> density;
    std::vector<float> pressure;

    // sink/star bookkeeping
    // std::vector<bool> is_star;   // true if particle is a star / sink
    // std::vector<bool> alive;

    std::vector<uint8_t> is_star;   // true if particle is a star / sink
    std::vector<uint8_t> alive;

    // caching
    std::vector<Vec3> acc_cache;
    std::vector<float> r2_cache;
    bool cache_valid = false;

    // ⭐ NEW: store stars separately
    std::vector<Star> stars;

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

    void write_stars_csv(const std::string& fname) const {
        std::ofstream f(fname);
        f << "mass,x,y,z,vx,vy,vz,formation_time\n";
        for (const auto& s : stars) {
            f << s.mass << ","
            << s.position.x << "," << s.position.y << "," << s.position.z << ","
            << s.velocity.x << "," << s.velocity.y << "," << s.velocity.z << ","
            << s.formation_time << "\n";
        }
    }


    // helper: count alive non-star gas particles
    // size_t count_alive() const {
    //     size_t c = 0;
    //     for (size_t i=0;i<N;++i) if (alive[i]) ++c;
    //     return c;
    // }

     // --- NEW METHODS ---
    size_t count_particles_in_stars() const {
        size_t count = 0;
        for (size_t i = 0; i < N; i++) {
            if (is_star[i]) count++;
        }
        return count;
    }

    void print_star_particles() const {
        std::cout << "Particles that became stars: ";
        bool any = false;
        for (size_t i = 0; i < N; i++) {
            if (is_star[i]) {
                std::cout << i << " ";
                any = true;
            }
        }
        if (!any) std::cout << "None";
        std::cout << "\n";
    }

    size_t count_star_particles() const {
        size_t count = 0;
        for (size_t i = 0; i < N; i++) {
            if (is_star[i]) count++;
        }
        return count;
    }

};