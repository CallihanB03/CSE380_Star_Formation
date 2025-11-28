// #include "particles.hpp"
// #include "physics.hpp"
// #include "integrator.hpp"
// #include "gravity.hpp"
// #include "physics.hpp"
// #include <iostream>
// #include <chrono>
// #include <string>
// #include <filesystem>
// #include <algorithm> 


// int main(int argc, char** argv) {
//     size_t N = 100;    // safe particle count for now
//     size_t num_steps = 100;
//     float dt = 0.01f;

//     bool use_cached = (argc > 1 && std::string(argv[1]) == "--cached");

//     Particles P(N);

//     // Small example initialization
//     for (size_t i = 0; i < N; i++) {
//         P.x[i] = float(i) * 0.001f;
//         P.y[i] = float(i) * 0.002f;
//         P.z[i] = float(i) * 0.003f;
//     }

    

//     std::ofstream star_log("stars_per_timestep.txt");
//     if (!star_log.is_open()) {
//         std::cerr << "Error opening star log file!\n";
//     }

//     auto start = std::chrono::high_resolution_clock::now();

//     for (size_t t = 0; t < num_steps; ++t) {
//         if (use_cached)
//             velocity_verlet_cached(P, dt);
//         else {
//             // Precompute accelerations
//             std::vector<float> ax(P.N, 0.0f);
//             std::vector<float> ay(P.N, 0.0f);
//             std::vector<float> az(P.N, 0.0f);

//             compute_gravity(P, ax, ay, az);

//             // compute densities with SPH kernel
//             float h = 0.05f; // choose based on mean interparticle spacing; tune later
//             compute_density_sph(P, h);

            

//             // compute pressure (hydro) forces and add to ax/ay/az
//             compute_pressure_forces(P, h, ax, ay, az);

//             velocity_verlet(P, ax, ay, az, dt);
//         }
        
//         // Update densities as part of physics
//         compute_density_kNN(P, 32);

//         // Update thermodynamics / physics
//         update_physics(P, dt);

//         // Check for Star Formation
//         P.check_star_formation(10.0f);
//         size_t n_stars = std::count(P.is_star.begin(), P.is_star.end(), true);
//         star_log << t << " " << n_stars << "\n";



//         // Output frame
//         std::string fname = "frames/frame_" + std::to_string(t) + ".csv";
//         P.write_csv(fname);
//         }

//     auto end = std::chrono::high_resolution_clock::now();
//     star_log.close();

//     std::cout << (use_cached ? "Cached" : "Normal") << " runtime: "
//               << std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count()
//               << " ms\n";

//     // Output final particle state
//     P.write_csv(use_cached ? "particles_cached.csv" : "particles_normal.csv");

//     return 0;
// }


#include "../include/particles.hpp"
#include "../include/physics.hpp"
#include "../include/integrator.hpp"
#include "../include/gravity.hpp"
#include "../include/density.hpp"
#include "../include/hydro.hpp"
#include "../include/init.hpp"


#include <iostream>
#include <chrono>
#include <string>
#include <filesystem>
#include <algorithm>
#include <fstream>

int main(int argc, char** argv) {
    size_t N = 1000;           // number of particles
    size_t num_steps = 100;   // simulation steps
    float dt = 0.01f;         // timestep

    bool use_cached = (argc > 1 && std::string(argv[1]) == "--cached");

    Particles P(N);

    // ----------------------------------------------------
    // Initialize particle positions
    // ----------------------------------------------------
    int version_type = 2; // or passed via CLI
    init_particles(P, version_type);

    // ----------------------------------------------------
    // Open star formation log
    // ----------------------------------------------------
    std::ofstream star_log("stars_per_timestep.txt");
    if (!star_log.is_open()) {
        std::cerr << "Error opening star log file!\n";
        return 1;
    }

    auto start = std::chrono::high_resolution_clock::now();

    // ----------------------------------------------------
    // Simulation loop
    // ----------------------------------------------------
    float h = 0.03f; // smoothing length (based on your spacing)
    for (size_t t = 0; t < num_steps; ++t) {

        if (use_cached) {
            // Cached gravity-only integrator
            velocity_verlet_cached(P, dt, h);

        } else {
            // ------------------------------------------------
            // 1. Compute gravitational acceleration
            // ------------------------------------------------
            std::vector<float> ax(P.N, 0.0f);
            std::vector<float> ay(P.N, 0.0f);
            std::vector<float> az(P.N, 0.0f);

            compute_gravity(P, ax, ay, az);

            // ------------------------------------------------
            // 2. Compute densities (SPH)
            // ------------------------------------------------
            
            compute_density_sph(P, h);

            // ------------------------------------------------
            // 3. Compute pressure forces
            // ------------------------------------------------
            compute_pressure_forces(P, h, ax, ay, az);

            // ------------------------------------------------
            // 4. Integrate motion
            // ------------------------------------------------
            velocity_verlet(P, ax, ay, az, dt);
        }

        // ----------------------------------------------------
        // 5. Update thermodynamic physics
        //    (temperature, pressure eqn of state, etc.)
        // ----------------------------------------------------
        update_physics(P, dt);

        // ----------------------------------------------------
        // 6. Check star formation
        // ----------------------------------------------------
        P.check_star_formation(10.0f);

        size_t n_stars = std::count(P.is_star.begin(), P.is_star.end(), true);
        star_log << t << " " << n_stars << "\n";

        // if (t % 10 == 0) {
        //     std::cout << "Step " << t << ": rho[0] = " << P.density[0] << "\n";
        // }


        // ----------------------------------------------------
        // 7. Output snapshot
        // ----------------------------------------------------
        std::string fname = "frames/frame_" + std::to_string(t) + ".csv";
        P.write_csv(fname);
    }

    auto end = std::chrono::high_resolution_clock::now();
    star_log.close();

    std::cout << (use_cached ? "Cached" : "Normal") << " runtime: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
              << " ms\n";

    // Final snapshot
    P.write_csv(use_cached ? "particles_cached.csv" : "particles_normal.csv");

    return 0;
}
