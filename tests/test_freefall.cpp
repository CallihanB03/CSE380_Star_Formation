#include <iostream>
#include <cmath>
#include "../include/vec3.hpp"
#include "../include/particle.hpp"
#include "../include/gravity.hpp"
#include "../include/integrator.hpp"

int main() {

    float G = 1.0f;      // normalized gravitational constant
    float dt = 0.001f;   // timestep
    int steps = 10000;

    // --- Freefall test: two masses at rest, separated by 2 units ---
    Particle p1, p2;

    p1.mass = 1.0f;
    p2.mass = 1.0f;

    p1.pos = Vec3(-1.0f, 0.0f, 0.0f);
    p2.pos = Vec3( 1.0f, 0.0f, 0.0f);

    p1.vel = Vec3(0.0f, 0.0f, 0.0f);
    p2.vel = Vec3(0.0f, 0.0f, 0.0f);

    for (int step = 0; step < steps; step++) {

        // compute forces
        Vec3 F12 = gravitational_force(p1, p2, G);
        Vec3 F21 = gravitational_force(p2, p1, G);

        // update using velocity Verlet (or whatever integrator you use)
        velocity_verlet_step(p1, F12, dt);
        velocity_verlet_step(p2, F21, dt);

        // Print positions and distance occasionally
        if (step % 100 == 0) {
            float dist = (p1.pos - p2.pos).norm();
            std::cout << step*dt << " "
                      << p1.pos.x << " "
                      << p2.pos.x << " "
                      << dist << "\n";
        }
    }

    return 0;
}
