#include "../include/particles.hpp"
#include "../include/physics.hpp"
#include "../include/integrator.hpp"
#include "../include/gravity.hpp"
#include "../include/density.hpp"
#include "../include/hydro.hpp"
#include "../include/init.hpp"
#include "../include/thermo.hpp"
#include "../include/starform.hpp"


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

    float cluster_radius = 0.5f;
    float mass_threshold = 2.0f;

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

        // 5. Update thermodynamics
        update_thermodynamics(P, dt);

        // ----------------------------------------------------
        // 6. Update thermodynamic physics
        //    (temperature, pressure eqn of state, etc.)
        // ----------------------------------------------------
        update_physics(P, dt);

        // ----------------------------------------------------
        // 7. Check star formation
        // ----------------------------------------------------
        // P.check_star_formation(10.0f);
        // auto stars = form_stars_from_clusters(P, cluster_radius, mass_threshold);
        auto new_stars = form_stars_from_clusters(P, cluster_radius, mass_threshold, t * dt);


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
    P.write_stars_csv("stars.csv");

    return 0;
}
