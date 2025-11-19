#include "../include/particles.hpp"
#include "../include/integrator.hpp"
#include "../include/gravity.hpp"
#include "../include/vec3.hpp"
#include <iostream>
#include <cmath>
#include <vector>

// Compute total momentum for Particles
Vec3 total_momentum(const Particles& P) {
    Vec3 Ptot{0, 0, 0};
    for (size_t i = 0; i < P.N; ++i) {
        Ptot += Vec3{P.vx[i], P.vy[i], P.vz[i]} * P.mass[i];
    }
    return Ptot;
}

int main() {
    const int N = 2;
    Particles P(N);

    // Initialize positions
    P.x[0] = -1.0f; P.y[0] = 0.0f; P.z[0] = 0.0f;
    P.x[1] =  1.0f; P.y[1] = 0.0f; P.z[1] = 0.0f;

    // Initialize velocities
    P.vx[0] =  1.0f; P.vy[0] = 0.0f; P.vz[0] = 0.0f;
    P.vx[1] = -1.0f; P.vy[1] = 0.0f; P.vz[1] = 0.0f;

    // Initialize masses
    P.mass[0] = 1.0f;
    P.mass[1] = 1.0f;

    // Compute initial total momentum
    Vec3 initial_P = total_momentum(P);

    // Time step
    float dt = 0.0001f;

    // Allocate force cache using Vec3
    std::vector<Vec3> force_cache(N);

    // Integrate 10000 steps
    for (int i = 0; i < 10000; ++i) {
        compute_gravity_cached(P, force_cache);  // fills force_cache
        // Half-step velocity + full-step position
        for (size_t j = 0; j < N; ++j) {
            P.vx[j] += 0.5f * force_cache[j].x * dt;
            P.vy[j] += 0.5f * force_cache[j].y * dt;
            P.vz[j] += 0.5f * force_cache[j].z * dt;

            P.x[j] += P.vx[j] * dt;
            P.y[j] += P.vy[j] * dt;
            P.z[j] += P.vz[j] * dt;
        }
        // Recompute forces for second half-step velocity update
        compute_gravity_cached(P, force_cache);
        for (size_t j = 0; j < N; ++j) {
            P.vx[j] += 0.5f * force_cache[j].x * dt;
            P.vy[j] += 0.5f * force_cache[j].y * dt;
            P.vz[j] += 0.5f * force_cache[j].z * dt;
        }
    }

    // Compute final total momentum
    Vec3 final_P = total_momentum(P);

    std::cout << "Initial P: " << initial_P.x << ", " << initial_P.y << ", " << initial_P.z << "\n";
    std::cout << "Final   P: " << final_P.x << ", " << final_P.y << ", " << final_P.z << "\n";

    if (std::abs(final_P.x - initial_P.x) < 1e-4 &&
        std::abs(final_P.y - initial_P.y) < 1e-4 &&
        std::abs(final_P.z - initial_P.z) < 1e-4) {
        std::cout << "PASS\n";
        return 0;
    } else {
        std::cout << "FAIL\n";
        return 1;
    }
}
