#include "../include/particles.hpp"
#include "../include/gravity.hpp"
#include "../include/vec3.hpp"
#include <vector>


void velocity_verlet(Particles& P,
                     const std::vector<float>& ax,
                     const std::vector<float>& ay,
                     const std::vector<float>& az,
                     float dt)
{
    // std::cout << "N=" << P.N 
    //       << " | x.size()=" << P.x.size()
    //       << " | y.size()=" << P.y.size()
    //       << " | z.size()=" << P.z.size()
    //       << " | vx.size()=" << P.vx.size()
    //       << " | vy.size()=" << P.vy.size()
    //       << " | vz.size()=" << P.vz.size()
    //       << std::endl;

    for (size_t i = 0; i < P.N; i++) {
        P.vx[i] += 0.5f * ax[i] * dt;
        P.vy[i] += 0.5f * ay[i] * dt;
        P.vz[i] += 0.5f * az[i] * dt;

        P.x[i] += P.vx[i] * dt;
        P.y[i] += P.vy[i] * dt;
        P.z[i] += P.vz[i] * dt;
    }

    P.update_thermodynamics();
}


void velocity_verlet_cached(Particles& P, float dt)
{
    // std::cout << "N=" << P.N 
    //       << " | x.size()=" << P.x.size()
    //       << " | y.size()=" << P.y.size()
    //       << " | z.size()=" << P.z.size()
    //       << " | vx.size()=" << P.vx.size()
    //       << " | vy.size()=" << P.vy.size()
    //       << " | vz.size()=" << P.vz.size()
    //       << std::endl;

    size_t N = P.N;
    std::vector<Vec3> force_cache(N);

    // Step 1: compute initial accelerations
    compute_gravity_cached(P, force_cache);

    // Step 2: half-step velocity update
    for (size_t i = 0; i < N; i++) {
        P.vx[i] += 0.5f * force_cache[i].x * dt;
        P.vy[i] += 0.5f * force_cache[i].y * dt;
        P.vz[i] += 0.5f * force_cache[i].z * dt;
    }

    // Step 3: full-step position update
    for (size_t i = 0; i < N; i++) {
        P.x[i] += P.vx[i] * dt;
        P.y[i] += P.vy[i] * dt;
        P.z[i] += P.vz[i] * dt;
    }

    // Step 3.5: update thermodynamics
    P.update_thermodynamics();

    // Step 4: compute new accelerations
    compute_gravity_cached(P, force_cache);

    // Step 5: second half-step velocity update
    for (size_t i = 0; i < N; i++) {
        P.vx[i] += 0.5f * force_cache[i].x * dt;
        P.vy[i] += 0.5f * force_cache[i].y * dt;
        P.vz[i] += 0.5f * force_cache[i].z * dt;
    }
}

