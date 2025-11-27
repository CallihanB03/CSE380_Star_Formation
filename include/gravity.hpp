

// #ifndef GRAVITY_HPP
// #define GRAVITY_HPP

// #include <vector>
// #include "particles.hpp"
// #include "vec3.hpp"

// // Original acceleration function
// void compute_gravity(const Particles& P,
//                      std::vector<float>& ax,
//                      std::vector<float>& ay,
//                      std::vector<float>& az);

// // Cached version using Vec3
// void compute_gravity_cached(const Particles& P,
//                             std::vector<Vec3>& force_cache);

// #endif // GRAVITY_HPP


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
