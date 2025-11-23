#pragma once
#include "particles.hpp"

void update_physics(Particles& P, float dt);
void compute_density_kNN(Particles& P, int k);
