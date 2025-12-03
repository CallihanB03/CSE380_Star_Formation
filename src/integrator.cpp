#include "../include/particles.hpp"
#include "../include/gravity.hpp"
#include "../include/vec3.hpp"
#include "../include/density.hpp"
#include "../include/hydro.hpp"
#include <vector>


void velocity_verlet(Particles& P,
                     const std::vector<float>& ax,
                     const std::vector<float>& ay,
                     const std::vector<float>& az,
                     float dt)
{

    for (size_t i = 0; i < P.N; i++) {
        if (!P.alive[i]) continue;
        P.vx[i] += 0.5f * ax[i] * dt;
        P.vy[i] += 0.5f * ay[i] * dt;
        P.vz[i] += 0.5f * az[i] * dt;

        P.x[i] += P.vx[i] * dt;
        P.y[i] += P.vy[i] * dt;
        P.z[i] += P.vz[i] * dt;
    }
}

void velocity_verlet_cached(Particles& P, float dt)
{

    for (size_t i = 0; i < P.N; i++) {
        if (!P.alive[i]) continue;

        P.vx[i] += 0.5f * P.ax[i] * dt;
        P.vy[i] += 0.5f * P.ay[i] * dt;
        P.vz[i] += 0.5f * P.az[i] * dt;

        P.x[i] += P.vx[i] * dt;
        P.y[i] += P.vy[i] * dt;
        P.z[i] += P.vz[i] * dt;
    }
}

