#pragma once

#include "particles.hpp"
#include <vector>
#include "vec3.hpp"


// Velocity-Verlet integrator for N-body particles
void velocity_verlet(Particles& P,
                     const std::vector<float>& ax,
                     const std::vector<float>& ay,
                     const std::vector<float>& az,
                     float dt);

// void velocity_verlet_cached(Particles& P, float dt, float h);
