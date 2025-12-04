#include "../include/particles.hpp"
#include "../include/physics.hpp"
#include "../include/integrator.hpp"
#include "../include/gravity.hpp"
#include "../include/density.hpp"
#include "../include/hydro.hpp"
#include "../include/init.hpp"
#include "../include/thermo.hpp"
#include "../include/starform.hpp"
#include "../include/distance.hpp"
#include "../include/bh.hpp"



#include <iostream>
#include <chrono>
#include <string>
#include <filesystem>
#include <algorithm>
#include <fstream>
#include <array>
#include <iostream>


int main(int argc, char** argv) {
    size_t N = 1000;                // number of particles
    size_t num_steps = 100;         // simulation steps
    float dt = 0.001f;               // timestep
    float neighbor_radius = 0.05f;  
    int min_neighbors = 5;          
    float density_threshold =  100.0; 
    int version_type = 2;  

    // int k = 50; // for density KNN    

    bool use_cached = (argc > 1 && ((std::string(argv[1]) == "--use_cached")));

    Particles P(N);
    // ----------------------------------------------------
    // Initialize particle positions
    // ----------------------------------------------------
    init_particles(P, version_type);
    StarFormation SF(neighbor_radius, min_neighbors, density_threshold);
    auto start = std::chrono::high_resolution_clock::now();


    float h = 0.03f; // smoothing length for Density

    if (use_cached){
        // BarnesHutSolver bh(P, 0., 1e-3);
        // bh.build_with_particles(P);
        start = std::chrono::high_resolution_clock::now();

        // ----------------------------------------------------
        // Simulation loop
        // ----------------------------------------------------
        for (size_t t = 0; t < num_steps; ++t) {
            // ------------------------------------------------
            // 1. Compute gravitational acceleration
            // ------------------------------------------------
            // bh.compute_accelerations(P, 1.0); // writes accelerations into P.ax,P.ay,P.az
            compute_gravity_cached_optimized(P);


            // ------------------------------------------------
            // 2. Compute densities (SPH or KNN)
            // ------------------------------------------------
            // compute_density_kNN(P, k);
            compute_density_sph(P, h);


            // ------------------------------------------------
            // 3. Compute pressure forces
            // (This is where the thermodynamics and kinetics kiss)
            // ------------------------------------------------
            compute_pressure_forces_cached(P, h);

            // ------------------------------------------------
            // 4. Integrate motion
            // ------------------------------------------------
            velocity_verlet_cached(P, dt);

            // ------------------------------------------------
            // 5. Update thermodynamics
            // ------------------------------------------------
            update_thermodynamics(P, dt);

            // ----------------------------------------------------
            // 6. Update thermodynamic physics
            //    (temperature, pressure eqn of state, etc.)
            // ----------------------------------------------------
            update_physics(P, dt);

            // ----------------------------------------------------
            // 7. Check star formation
            // ----------------------------------------------------
            auto candidates = SF.detect_star_candidates(P);
            SF.form_stars(P, candidates, t * dt);

            // ----------------------------------------------------
            // 8. Output snapshot
            // ----------------------------------------------------
            std::string fname = "frames/frame_" + std::to_string(t) + ".csv";
            P.write_csv(fname);
        }


    }

    else {
        start = std::chrono::high_resolution_clock::now();

        // ----------------------------------------------------
        // Simulation loop
        // ----------------------------------------------------
        for (size_t t = 0; t < num_steps; ++t) {
            // ------------------------------------------------
            // 1. Compute gravitational acceleration
            // ------------------------------------------------
            std::vector<float> ax(P.N, 0.0f);
            std::vector<float> ay(P.N, 0.0f);
            std::vector<float> az(P.N, 0.0f);

            compute_gravity(P, ax, ay, az);

            // ------------------------------------------------
            // 2. Compute densities (SPH or KNN)
            // ------------------------------------------------
            // compute_density_kNN(P, k);
            compute_density_sph(P, h);


            // ------------------------------------------------
            // 3. Compute pressure forces
            // (This is where the thermodynamics and kinetics kiss)
            // ------------------------------------------------
            compute_pressure_forces(P, h, ax, ay, az);

            // ------------------------------------------------
            // 4. Integrate motion
            // ------------------------------------------------
            velocity_verlet(P, ax, ay, az, dt);

            // ------------------------------------------------
            // 5. Update thermodynamics
            // ------------------------------------------------
            update_thermodynamics(P, dt);

            // ----------------------------------------------------
            // 6. Update thermodynamic physics
            //    (temperature, pressure eqn of state, etc.)
            // ----------------------------------------------------
            update_physics(P, dt);

            // ----------------------------------------------------
            // 7. Check star formation
            // ----------------------------------------------------
            auto candidates = SF.detect_star_candidates(P);
            SF.form_stars(P, candidates, t * dt);

            // ----------------------------------------------------
            // 8. Output snapshot
            // ----------------------------------------------------
            std::string fname = "frames/frame_" + std::to_string(t) + ".csv";
            P.write_csv(fname);
        }

    }

    auto end = std::chrono::high_resolution_clock::now();

    std::cout << (use_cached ? "cached" : "normal")
              << " runtime: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
              << " ms\n";


    int particless_to_star_count = P.count_particles_in_stars();
    std::cout << "Particles that became stars: " <<  particless_to_star_count << std::endl;

    return 0;
}
