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
#pragma once
#include "particles.hpp"

#include <vector>

void check_and_create_sinks(Particles& P, float rho_threshold, float r_merge);
