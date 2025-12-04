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
    const size_t N = P.N;

    // Cache pointers for speed
    float* x  = P.x.data();
    float* y  = P.y.data();
    float* z  = P.z.data();

    float* vx = P.vx.data();
    float* vy = P.vy.data();
    float* vz = P.vz.data();

    float* ax = P.ax.data();
    float* ay = P.ay.data();
    float* az = P.az.data();

    const uint8_t* alive = P.alive.data();

    for (size_t i = 0; i < N; ++i) {
        if (!alive[i]) continue;

        // Half-step velocity update
        vx[i] += 0.5f * ax[i] * dt;
        vy[i] += 0.5f * ay[i] * dt;
        vz[i] += 0.5f * az[i] * dt;

        // Full-step position update
        x[i] += vx[i] * dt;
        y[i] += vy[i] * dt;
        z[i] += vz[i] * dt;
    }
}

