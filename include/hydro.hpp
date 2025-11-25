#pragma once
#include "particles.hpp"
#include <vector>

// Compute pressure from density and temperature or polytropic EOS
void compute_pressure(Particles& P, float K = 1.0f, float gamma = 5.0f/3.0f);

// Compute hydrodynamic acceleration from pressure gradients (SPH symmetric form).
// Writes into ax,ay,az which should be pre-filled with gravity contributions.
void compute_pressure_forces(Particles& P, float h,
                             std::vector<float>& ax,
                             std::vector<float>& ay,
                             std::vector<float>& az);
