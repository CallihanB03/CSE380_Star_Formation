#include "../include/particles.hpp"
#include "../include/integrator.hpp"
#include "../include/gravity.hpp"
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char** argv) {

    // ======================
    // 1. Initialize system
    // ======================
    size_t N = 1000;      // <-- change for your system
    float dt = 0.01f;

    Particles P(N);       // <-- FIXED (must pass N)

    // NON-CACHED acceleration arrays
    std::vector<float> ax(N, 0.0f);
    std::vector<float> ay(N, 0.0f);
    std::vector<float> az(N, 0.0f);

    // ======================
    // 2. Parse flag
    // ======================
    bool use_cached = false;
    if (argc > 1 && std::string(argv[1]) == "--cached") {
        use_cached = true;
    }

    // ======================
    // 3. Run integrator
    // ======================
    if (use_cached) {
        std::cout << "Running cached integrator\n";
        velocity_verlet_cached(P, dt);
    } else {
        std::cout << "Running normal integrator\n";
        velocity_verlet(P, ax, ay, az, dt);
    }

    return 0;
}

