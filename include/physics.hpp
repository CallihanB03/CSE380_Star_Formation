// physics.hpp
#pragma once
#include "particles.hpp"

inline void update_physics(Particles& P, float dt) {
    for (size_t i = 0; i < P.N; i++) {
        // Example: simple adiabatic heating/cooling
        P.temperature[i] += 0.01f * dt; // dummy physics
        P.pressure[i] = P.density[i] * P.temperature[i]; // ideal gas approximation
    }
}
