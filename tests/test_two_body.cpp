#include <iostream>
#include <cmath>
#include "../include/vec3.hpp"
#include "../include/particle.hpp"
#include "../include/gravity.hpp"
#include "../include/integrator.hpp"

int main() {

    float G = 1.0f;      // normalized gravity
    float dt = 0.001f;   // small timestep
    int steps = 20000;

    // --- Two-body circular orbit test ---

    Particle p1, p2;

    p1.mass = 1.0f;
    p2.mass = 1.0f;

    p1.pos = Vec3(-1.0f, 0.0f, 0.0f);
    p2.pos = Vec3( 1.0f, 0.0f, 0.0f);

    p1.vel = Vec3(0.0f, 0.5f, 0.0f);
    p2.vel = Vec3(0.0f,-0.5f, 0.0f);

    for (int step = 0; step < steps; step++) {

        // compute forces
        Vec3 F12 = gravitational_force(p1, p2, G);
        Vec3 F21 = gravitational_force(p2, p1, G);

        // velocity-verlet update
        velocity_verlet_step(p1, F12, dt);
        velocity_verlet_step(p2, F21, dt);

        // --- print the distance occasionally ---
        if (step % 100 == 0) {
            float dist = (p1.pos - p2.pos).norm();
            std::cout << step*dt << " " << dist << "\n";
        }
    }

    return 0;
}
