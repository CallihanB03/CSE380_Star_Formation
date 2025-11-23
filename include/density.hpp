#pragma once
#include "particles.hpp"

// Compute densities for all particles using k-nearest neighbors
// Default k=32 neighbors
void compute_density_kNN(Particles& P, int k = 32);
