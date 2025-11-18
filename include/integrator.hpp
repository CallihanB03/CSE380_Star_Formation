#pragma once

#include "particles.hpp"
#include <vector>

// Velocity-Verlet integrator for N-body particles
void velocity_verlet(Particles& P,
                     const std::vector<float>& ax,
                     const std::vector<float>& ay,
                     const std::vector<float>& az,
                     float dt);
