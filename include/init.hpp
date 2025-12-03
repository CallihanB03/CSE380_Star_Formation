// init.hpp
#pragma once
#include "particles.hpp"

void init_uniform(Particles& P);
void init_clustered(Particles& P);
void init_spherical(Particles& P);
void identity_for_debugging(Particles& P);

void init_particles(Particles& P, int version_type);
