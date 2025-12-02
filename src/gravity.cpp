#include "../include/particles.hpp"
#include "../include/vec3.hpp"
#include "../include/gravity.hpp"
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

// cached version: compute per-particle acceleration Vec3 and fill acc_cache
void compute_gravity_cached(Particles& P,
                           std::vector<Vec3>& acc_cache,
                           float G,
                           float softening)
{
    size_t N = P.N;
    if (acc_cache.size() != N) acc_cache.assign(N, Vec3{0.0f,0.0f,0.0f});
    std::fill(acc_cache.begin(), acc_cache.end(), Vec3{0.0f,0.0f,0.0f});

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
            float a_scalar = G * inv_r3;
            // acceleration on i due to j: a_i += a_scalar * m_j * (r_j - r_i)
            Vec3 aij{ a_scalar * P.mass[j] * dx,
                      a_scalar * P.mass[j] * dy,
                      a_scalar * P.mass[j] * dz };
            acc_cache[i] += aij;
            // acceleration on j due to i: equal and opposite
            Vec3 aji{ -a_scalar * P.mass[i] * dx,
                      -a_scalar * P.mass[i] * dy,
                      -a_scalar * P.mass[i] * dz };
            acc_cache[j] += aji;
        }
    }
    // Note: acc_cache stores acceleration vectors (not divided by the particle mass).
    P.cache_valid = true;
}