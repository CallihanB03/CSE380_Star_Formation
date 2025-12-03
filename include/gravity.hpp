// include/gravity.hpp
#pragma once
#include "particles.hpp"
#include "vec3.hpp"
#include <vector>

void compute_gravity(const Particles& P,
                     std::vector<float>& ax,
                     std::vector<float>& ay,
                     std::vector<float>& az,
                     float G = 1.0f,
                     float softening = 0.01f);

void compute_gravity_cached(Particles& P,
                           std::vector<Vec3>& acc_cache,
                           float G = 1.0f,
                           float softening = 0.01f);
