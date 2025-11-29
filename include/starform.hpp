// #pragma once
// #include "particles.hpp"
// #include <vector>

// // find clusters among candidate particles (indices). linking_length in same units as positions
// std::vector<std::vector<size_t>> find_star_clusters(const Particles& P,
//                                                     const std::vector<size_t>& candidates,
//                                                     float linking_length);

// // form sinks or mark cluster membership
// void form_sinks_from_clusters(Particles& P, const std::vector<std::vector<size_t>>& clusters);

// include/starformation.hpp
// #pragma once
// #include "particles.hpp"

// #include <vector>

// void check_and_create_sinks(Particles& P, float rho_threshold, float r_merge);

#pragma once
#include "particles.hpp"
#include <vector>

struct FormedStar {
    float x, y, z;
    float mass;
};

// Finds clusters of gas particles
std::vector<std::vector<int>>
find_clusters(const Particles& P, float cluster_radius);

// Forms stars (sinks) out of clusters exceeding mass threshold
// std::vector<FormedStar>
std::vector<size_t>
form_stars_from_clusters(Particles& P,
                         float cluster_radius,
                         float mass_threshold,
                         float time);
