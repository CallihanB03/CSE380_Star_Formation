#pragma once
#include "particles.hpp"

inline void update_physics(Particles& P, float dt) {
    // simple cooling (damp velocities slightly) and update pressure from ideal gas
    const float cooling_alpha = 0.001f; // tune: 1e-3 .. 1e-2
    for (size_t i = 0; i < P.N; ++i) {
        if (!P.alive[i]) continue;
        // damping velocities (very simple cooling)
        P.vx[i] *= (1.0f - cooling_alpha);
        P.vy[i] *= (1.0f - cooling_alpha);
        P.vz[i] *= (1.0f - cooling_alpha);

        // update pressure using ideal gas law P = rho * T
        P.pressure[i] = P.density[i] * P.temperature[i];

        // optional temperature change (small radiative cooling)
        P.temperature[i] *= 0.9995f; // very gentle cooling
    }
}
