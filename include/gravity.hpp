// include/gravity.hpp
#pragma once
#include "particles.hpp"
#include <vector>

void compute_gravity(const Particles& P,
                     std::vector<float>& ax,
                     std::vector<float>& ay,
                     std::vector<float>& az,
                     float G = 1.0f,
                     float softening = 0.01f);

