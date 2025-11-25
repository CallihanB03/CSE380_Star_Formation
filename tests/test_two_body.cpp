// #include "../include/particles.hpp"
// #include "../include/gravity.hpp"
// #include "../include/integrator.hpp"
// #include <iostream>
// #include <cmath>

// float compute_distance(const Particles& P) {
//     float dx = P.x[1] - P.x[0];
//     float dy = P.y[1] - P.y[0];
//     float dz = P.z[1] - P.z[0];
//     return std::sqrt(dx*dx + dy*dy + dz*dz);
// }

// float compute_kinetic_energy(const Particles& P) {
//     float KE = 0.0f;
//     for (size_t i = 0; i < P.N; i++) {
//         KE += 0.5f * P.mass[i] * (P.vx[i]*P.vx[i] + P.vy[i]*P.vy[i] + P.vz[i]*P.vz[i]);
//     }
//     return KE;
// }

// float compute_potential_energy(const Particles& P, float G) {
//     float PE = 0.0f;
//     for (size_t i = 0; i < P.N; i++) {
//         for (size_t j = i+1; j < P.N; j++) {
//             float dx = P.x[j] - P.x[i];
//             float dy = P.y[j] - P.y[i];
//             float dz = P.z[j] - P.z[i];
//             float r = std::sqrt(dx*dx + dy*dy + dz*dz);
//             PE -= G * P.mass[i] * P.mass[j] / r;
//         }
//     }
//     return PE;
// }

// int main() {
//     const float G = 1.0f;     // Gravitational constant
//     const float dt = 0.01f;   // Time step
//     const int steps = 100;

//     Particles P(2);  // Two particles
//     P.mass[0] = P.mass[1] = 1.0f;
//     P.x[0] = -1.0f; P.y[0] = 0.0f; P.z[0] = 0.0f;
//     P.x[1] =  1.0f; P.y[1] = 0.0f; P.z[1] = 0.0f;

//     std::vector<float> ax(P.N), ay(P.N), az(P.N);

//     // Initial acceleration
//     compute_gravity(P, ax, ay, az);

//     float initial_energy = compute_kinetic_energy(P) + compute_potential_energy(P, G);

//     for (int step = 0; step <= steps; step++) {
//         // Print positions
//         std::cout << "Step " << step << ": ";
//         std::cout << "P1=(" << P.x[0] << "," << P.y[0] << "," << P.z[0] << ") ";
//         std::cout << "P2=(" << P.x[1] << "," << P.y[1] << "," << P.z[1] << ") ";

//         float KE = compute_kinetic_energy(P);
//         float PE = compute_potential_energy(P, G);
//         std::cout << "KE=" << KE << " PE=" << PE << " Total=" << (KE+PE) << "\n";

//          compute_gravity(P, ax, ay, az);

         
//         // Velocity Verlet step
//         velocity_verlet(P, ax, ay, az, dt);
//     }

//     float final_energy = compute_kinetic_energy(P) + compute_potential_energy(P, G);
//     std::cout << "\nInitial Total Energy: " << initial_energy 
//               << " | Final Total Energy: " << final_energy 
//               << " | ΔE: " << (final_energy - initial_energy) << "\n";

//     return 0;
// }

#include "../include/particles.hpp"
#include "../include/gravity.hpp"
#include "../include/integrator.hpp"
#include <iostream>
#include <cmath>
#include <vector>

float compute_kinetic_energy(const Particles& P) {
    float KE = 0.0f;
    for (size_t i = 0; i < P.N; i++) {
        KE += 0.5f * P.mass[i] * (P.vx[i]*P.vx[i] + P.vy[i]*P.vy[i] + P.vz[i]*P.vz[i]);
    }
    return KE;
}

float compute_potential_energy(const Particles& P, float G=1.0f) {
    float PE = 0.0f;
    for (size_t i = 0; i < P.N; i++) {
        for (size_t j = i+1; j < P.N; j++) {
            float dx = P.x[j] - P.x[i];
            float dy = P.y[j] - P.y[i];
            float dz = P.z[j] - P.z[i];
            float r = std::sqrt(dx*dx + dy*dy + dz*dz);
            if (r > 0) PE -= G * P.mass[i] * P.mass[j] / r;
        }
    }
    return PE;
}

int main() {
    const float dt = 0.01f;
    const int steps = 100;
    const float G = 1.0f;

    // Initialize 2 particles
    Particles P(2);
    P.mass[0] = P.mass[1] = 1.0f;
    P.x[0] = -1.0f; P.y[0] = 0.0f; P.z[0] = 0.0f;
    P.x[1] =  1.0f; P.y[1] = 0.0f; P.z[1] = 0.0f;

    std::cout << "N=" << P.N << " | acc_cache size=" << P.acc_cache.size() 
          << " | r2_cache size=" << P.r2_cache.size() << "\n";


    // Initialize acceleration vectors
    std::vector<float> ax(P.N), ay(P.N), az(P.N);

    float initial_energy = compute_kinetic_energy(P) + compute_potential_energy(P, G);

    for (int step = 0; step <= steps; step++) {
        // Recompute accelerations each step
        compute_gravity(P, ax, ay, az);

        std::cout << "Step " << step << " | P1=(" << P.x[0] << "," << P.y[0] << "," << P.z[0] << ") "
          << "P2=(" << P.x[1] << "," << P.y[1] << "," << P.z[1] << ")\n";


        // Update positions and velocities
        velocity_verlet(P, ax, ay, az, dt);

        // Print step info
        float KE = compute_kinetic_energy(P);
        float PE = compute_potential_energy(P, G);
        std::cout << "Step " << step 
                  << " | KE=" << KE 
                  << " PE=" << PE 
                  << " Total=" << (KE + PE) << "\n";
    }

    float final_energy = compute_kinetic_energy(P) + compute_potential_energy(P, G);
    std::cout << "\nInitial Energy: " << initial_energy 
              << " | Final Energy: " << final_energy
              << " | ΔE: " << (final_energy - initial_energy) << "\n";

    return 0;
}
