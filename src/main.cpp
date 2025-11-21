// #include "../include/particles.hpp"
// #include "../include/integrator.hpp"
// #include "../include/gravity.hpp"
// #include <iostream>
// #include <string>
// #include <vector>

// int main(int argc, char** argv) {

//     // ======================
//     // 1. Initialize system
//     // ======================
//     size_t N = 1000;      // <-- change for your system
//     float dt = 0.01f;

//     Particles P(N);       // <-- FIXED (must pass N)

//     // NON-CACHED acceleration arrays
//     std::vector<float> ax(N, 0.0f);
//     std::vector<float> ay(N, 0.0f);
//     std::vector<float> az(N, 0.0f);

//     // ======================
//     // 2. Parse flag
//     // ======================
//     bool use_cached = false;
//     if (argc > 1 && std::string(argv[1]) == "--cached") {
//         use_cached = true;
//     }

//     // ======================
//     // 3. Run integrator
//     // ======================
//     if (use_cached) {
//         std::cout << "Running cached integrator\n";
//         velocity_verlet_cached(P, dt);
//     } else {
//         std::cout << "Running normal integrator\n";
//         velocity_verlet(P, ax, ay, az, dt);
//     }

//     return 0;
// }


#include "particles.hpp"
#include "integrator.hpp"
#include "gravity.hpp"
#include "physics.hpp"
#include <iostream>
#include <chrono>
#include <string>

int main(int argc, char** argv) {
    size_t N = 100;    // safe particle count for now
    size_t num_steps = 100;
    float dt = 0.01f;

    bool use_cached = (argc > 1 && std::string(argv[1]) == "--cached");

    Particles P(N);

    // Small example initialization
    for (size_t i = 0; i < N; i++) {
        P.x[i] = float(i) * 0.001f;
        P.y[i] = float(i) * 0.002f;
        P.z[i] = float(i) * 0.003f;
    }

    auto start = std::chrono::high_resolution_clock::now();

    for (size_t t = 0; t < num_steps; ++t) {
        if (use_cached)
            velocity_verlet_cached(P, dt);
        else {
            // Precompute accelerations
            std::vector<float> ax(P.N, 0.0f);
            std::vector<float> ay(P.N, 0.0f);
            std::vector<float> az(P.N, 0.0f);
            compute_gravity(P, ax, ay, az);

            velocity_verlet(P, ax, ay, az, dt);
        }

        // Update thermodynamics / physics
        update_physics(P, dt);
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::cout << (use_cached ? "Cached" : "Normal") << " runtime: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count()
              << " ms\n";

    // Output final particle state
    P.write_csv(use_cached ? "particles_cached.csv" : "particles_normal.csv");

    return 0;
}
