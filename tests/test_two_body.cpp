#include "../include/particles.hpp"
#include "../include/gravity.hpp"
#include "../include/integrator.hpp"
#include <vector>
#include <iostream>

int main() {
    // Constants
    const float G = 1.0f;
    const float dt = 0.01f;
    const int steps = 100;

    // Create 2 particles
    Particles P(2);
    P.mass[0] = 1.0f;
    P.mass[1] = 1.0f;

    P.x[0] = -1.0f; P.y[0] = 0.0f; P.z[0] = 0.0f;
    P.x[1] = 1.0f;  P.y[1] = 0.0f; P.z[1] = 0.0f;

    P.vx[0] = 0.0f; P.vy[0] = 0.5f; P.vz[0] = 0.0f;
    P.vx[1] = 0.0f; P.vy[1] = -0.5f; P.vz[1] = 0.0f;

    std::vector<float> ax(2, 0.0f);
    std::vector<float> ay(2, 0.0f);
    std::vector<float> az(2, 0.0f);

    // Time loop
    for (int step = 0; step < steps; ++step) {
        // Compute gravitational accelerations
        compute_gravity(P, ax, ay, az);

        // Update positions and velocities
        velocity_verlet(P, ax, ay, az, dt);

        // Print positions for debugging
        std::cout << "Step " << step 
                  << ": P1=(" << P.x[0] << "," << P.y[0] << "," << P.z[0] << ") "
                  << "P2=(" << P.x[1] << "," << P.y[1] << "," << P.z[1] << ")\n";
    }

    return 0;
}

