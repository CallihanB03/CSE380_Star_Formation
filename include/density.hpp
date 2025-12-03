#pragma once
#include "particles.hpp"

// Compute density using SPH cubic-spline kernel with fixed smoothing length h
// This is O(N^2). kNeighbors unused here (kept for compatibility).
void compute_density_sph(Particles& P, float h);

// Compute densities for all particles using k-nearest neighbors
// Default k=32 neighbors
// void compute_density_kNN(Particles& P, int k = 32);
