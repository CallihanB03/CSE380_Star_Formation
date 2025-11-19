#ifndef GRAVITY_HPP
#define GRAVITY_HPP

#include <vector>
#include "particles.hpp"
#include "vec3.hpp"

// Function declaration for cached N-body forces
void compute_gravity_cached(const Particles& P, std::vector<Vec3>& force_cache);

#endif // GRAVITY_HPP


#pragma once

// #include "particles.hpp"
// #include <vector>

// Compute gravitational accelerations for N particles
void compute_gravity(const Particles& P,
                     std::vector<float>& ax,
                     std::vector<float>& ay,
                     std::vector<float>& az);
