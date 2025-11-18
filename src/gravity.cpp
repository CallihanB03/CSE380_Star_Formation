#include "../include/particles.hpp"
#include <vector>
#include <cstddef>
#include <cmath>

void compute_gravity(const Particles& P,
                     std::vector<float>& ax,
                     std::vector<float>& ay,
                     std::vector<float>& az)
{
    const float G = 1.0f;
    const float eps2 = 0.01f;

    size_t N = P.N;
    for (size_t i = 0; i < N; i++) {
        ax[i] = ay[i] = az[i] = 0.0f;
    }

    for (size_t i = 0; i < N; i++) {
        for (size_t j = i+1; j < N; j++) {
            float dx = P.x[j] - P.x[i];
            float dy = P.y[j] - P.y[i];
            float dz = P.z[j] - P.z[i];

            float r2 = dx*dx + dy*dy + dz*dz + eps2;
            float inv_r3 = 1.0f / (sqrt(r2) * r2);

            float f = G * P.mass[i] * P.mass[j] * inv_r3;

            ax[i] += f * dx / P.mass[i];
            ay[i] += f * dy / P.mass[i];
            az[i] += f * dz / P.mass[i];

            ax[j] -= f * dx / P.mass[j];
            ay[j] -= f * dy / P.mass[j];
            az[j] -= f * dz / P.mass[j];
        }
    }
}
