#pragma once

#include "particles.hpp"
#include <vector>

// Compute gravitational accelerations for N particles
void compute_gravity(const Particles& P,
                     std::vector<float>& ax,
                     std::vector<float>& ay,
                     std::vector<float>& az);
