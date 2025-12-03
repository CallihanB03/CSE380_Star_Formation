#include "../include/particles.hpp"
#include <vector>
#include <cstddef>
#include <cmath>

void compute_gravity(const Particles& P,
                     std::vector<float>& ax,
                     std::vector<float>& ay,
                     std::vector<float>& az,
                     float G,
                     float softening)
{
    size_t N = P.N;
    std::fill(ax.begin(), ax.end(), 0.0f);
    std::fill(ay.begin(), ay.end(), 0.0f);
    std::fill(az.begin(), az.end(), 0.0f);

    for (size_t i = 0; i < N; ++i) {
        if (!P.alive[i]) continue;
        for (size_t j = i+1; j < N; ++j) {
            if (!P.alive[j]) continue;
            float dx = P.x[j] - P.x[i];
            float dy = P.y[j] - P.y[i];
            float dz = P.z[j] - P.z[i];
            float r2 = dx*dx + dy*dy + dz*dz;
            float denom = std::sqrt(r2 + softening*softening);
            float inv_r3 = 1.0f / (denom * denom * denom + 1e-20f);
            float f = G * P.mass[i] * P.mass[j] * inv_r3;
            // acceleration contributions
            float aix = f * dx / P.mass[i]; // f/m_i = G m_j / r^3 * dx
            float ajx = -f * dx / P.mass[j];
            ax[i] += aix; ax[j] += ajx;
            float aiy = f * dy / P.mass[i];
            float ajy = -f * dy / P.mass[j];
            ay[i] += aiy; ay[j] += ajy;
            float aiz = f * dz / P.mass[i];
            float ajz = -f * dz / P.mass[j];
            az[i] += aiz; az[j] += ajz;
        }
    }
}