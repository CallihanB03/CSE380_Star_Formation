#include "particles.hpp"
#include <random>

void init_spherical_cloud(Particles& P, float radius) {
    std::mt19937 rng(42);
    std::uniform_real_distribution<float> dist(-radius, radius);

    for (size_t i = 0; i < P.N; i++) {
        float x, y, z;
        // reject points outside sphere
        do {
            x = dist(rng);
            y = dist(rng);
            z = dist(rng);
        } while (x*x + y*y + z*z > radius*radius);

        P.x[i] = x;
        P.y[i] = y;
        P.z[i] = z;

        P.vx[i] = P.vy[i] = P.vz[i] = 0.0f;
        P.temperature[i] = 1.0f;
        P.mass[i] = 1.0f;
    }
}
