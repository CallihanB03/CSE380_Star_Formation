#pragma once
#include "particles.hpp"

void update_thermodynamics(Particles& P, float dt) {
    size_t N = P.N;
    float gamma = 5.0f / 3.0f;

    // Need velocity divergence (cheap finite difference)
    std::vector<float> divv(N, 0.0f);

    for (size_t i = 0; i < N; ++i) {
        divv[i] = P.vx[i] + P.vy[i] + P.vz[i]; // simple stand-in placeholder
    }

    // Update temperature
    for (size_t i = 0; i < N; ++i) {
        float T = P.temperature[i];
        P.temperature[i] = T - (gamma - 1.0f) * T * divv[i] * dt;
        if (P.temperature[i] < 0.0f) P.temperature[i] = 0.0f;
    }

    // Update pressure using ideal gas
    for (size_t i = 0; i < N; ++i) {
        P.pressure[i] = P.density[i] * P.temperature[i];
    }
}