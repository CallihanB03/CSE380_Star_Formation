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


// void velocity_verlet_cached(Particles& P, float dt, float h) {
//     size_t N = P.N;
//     std::vector<float> ax(N, 0.0f), ay(N, 0.0f), az(N, 0.0f);

//     // 1. Compute gravity
//     compute_gravity(P, ax, ay, az);

//     // 2. Compute densities
//     compute_density_sph(P, h);

//     // 3. Compute pressure forces
//     compute_pressure_forces(P, h, ax, ay, az);

//     // 4. Integrate motion
//     for (size_t i = 0; i < N; ++i) {
//         P.vx[i] += 0.5f * ax[i] * dt;
//         P.vy[i] += 0.5f * ay[i] * dt;
//         P.vz[i] += 0.5f * az[i] * dt;

//         P.x[i] += P.vx[i] * dt;
//         P.y[i] += P.vy[i] * dt;
//         P.z[i] += P.vz[i] * dt;
//     }

//     // 5. Update gravity/forces again for second half-step
//     compute_density_sph(P, h); 
//     compute_gravity(P, ax, ay, az);
//     compute_pressure_forces(P, h, ax, ay, az);

//     for (size_t i = 0; i < N; ++i) {
//         P.vx[i] += 0.5f * ax[i] * dt;
//         P.vy[i] += 0.5f * ay[i] * dt;
//         P.vz[i] += 0.5f * az[i] * dt;
//     }
// }


// void velocity_verlet_cached(Particles& P, float dt, float h)
// {
//     size_t N = P.N;

//     std::vector<float> ax(N, 0.0f);
//     std::vector<float> ay(N, 0.0f);
//     std::vector<float> az(N, 0.0f);

//     // ---- (1) First force evaluation ------------------------------------
//     compute_gravity(P, ax, ay, az);      // Gravity first
//     compute_density_sph(P, h);           // Density
//     compute_pressure_forces(P, h, ax, ay, az);  // Pressure forces add to ax/ay/az

//     // ---- (2) First half velocity update + position update --------------
//     for (size_t i = 0; i < N; i++) {
//         if (!P.alive[i]) continue;

//         // v(t + dt/2)
//         P.vx[i] += 0.5f * ax[i] * dt;
//         P.vy[i] += 0.5f * ay[i] * dt;
//         P.vz[i] += 0.5f * az[i] * dt;

//         // x(t + dt)
//         P.x[i] += P.vx[i] * dt;
//         P.y[i] += P.vy[i] * dt;
//         P.z[i] += P.vz[i] * dt;
//     }

//     // ---- (3) Recompute forces at new positions -------------------------
//     // Reset force buffers
//     std::fill(ax.begin(), ax.end(), 0.0f);
//     std::fill(ay.begin(), ay.end(), 0.0f);
//     std::fill(az.begin(), az.end(), 0.0f);

//     compute_gravity(P, ax, ay, az);
//     compute_density_sph(P, h);
//     compute_pressure_forces(P, h, ax, ay, az);

//     // ---- (4) Second half velocity update --------------------------------
//     for (size_t i = 0; i < N; i++) {
//         if (!P.alive[i]) continue;

//         P.vx[i] += 0.5f * ax[i] * dt;
//         P.vy[i] += 0.5f * ay[i] * dt;
//         P.vz[i] += 0.5f * az[i] * dt;
//     }
// }


void velocity_verlet_cached(Particles& P, float dt, float h) {
    size_t N = P.N;
    std::vector<float> ax(N, 0.0f), ay(N, 0.0f), az(N, 0.0f);

    // 1. Compute forces at t
    compute_gravity(P, ax, ay, az);
    compute_density_sph(P, h);
    compute_pressure_forces(P, h, ax, ay, az);

    // 2. Half-step velocity & full-step position
    for (size_t i = 0; i < N; ++i) {
        if (!P.alive[i]) continue;
        P.vx[i] += 0.5f * ax[i] * dt;
        P.vy[i] += 0.5f * ay[i] * dt;
        P.vz[i] += 0.5f * az[i] * dt;

        P.x[i] += P.vx[i] * dt;
        P.y[i] += P.vy[i] * dt;
        P.z[i] += P.vz[i] * dt;
    }

    // 3. Recompute densities and forces at t+dt
    compute_density_sph(P, h);          // <-- MUST recompute
    compute_gravity(P, ax, ay, az);
    compute_pressure_forces(P, h, ax, ay, az);

    // 4. Complete velocity half-step
    for (size_t i = 0; i < N; ++i) {
        if (!P.alive[i]) continue;
        P.vx[i] += 0.5f * ax[i] * dt;
        P.vy[i] += 0.5f * ay[i] * dt;
        P.vz[i] += 0.5f * az[i] * dt;
    }
}
