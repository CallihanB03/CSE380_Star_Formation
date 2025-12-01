#pragma once
#include <vector>
#include <cmath>
#include "particles.hpp"
#include "vec3.hpp"
#include "stars.hpp"

class StarFormation {
public:
    float neighbor_radius;
    size_t min_neighbors;
    float min_density;

    StarFormation(float R = 0.5f, size_t k = 8, float rho = 5.0f)
        : neighbor_radius(R), min_neighbors(k), min_density(rho) {}

    std::vector<int> detect_star_candidates(const Particles& P) const;

private:
    int count_neighbors(const Particles& P, int idx) const;
    float compute_local_density(const Particles& P, int idx) const;

public:
    // convert gas particles to stars (keeps them in the array)
    void form_stars(
        Particles& P,
        const std::vector<int>& to_convert,
        float current_time
    );
};
