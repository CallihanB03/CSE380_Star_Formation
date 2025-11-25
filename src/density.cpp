// #include "../include/density.hpp"
// #include <algorithm>
// #include <vector>
// #include <cmath>

// static inline float cubic_spline_W(float r, float h) {
//     const float q = r / h;
//     const float sigma = 1.0f / (M_PI * h*h*h); // normalization in 3D
//     if (q >= 0.0f && q < 1.0f) {
//         return sigma * (1.0f - 1.5f*q*q + 0.75f*q*q*q);
//     } else if (q < 2.0f) {
//         float t = 2.0f - q;
//         return sigma * (0.25f * t*t*t);
//     } else {
//         return 0.0f;
//     }
// }

// void compute_density_sph(Particles& P, float h) {
//     size_t N = P.N;
//     if (N == 0) return;
//     if (P.density.size() != N) P.density.assign(N, 0.0f);

//     // Precompute h inv if helpful
//     for (size_t i = 0; i < N; ++i) P.density[i] = 0.0f;

//     for (size_t i = 0; i < N; ++i) {
//         // self contribution (often included)
//         float rho = 0.0f;
//         for (size_t j = 0; j < N; ++j) {
//             float dx = P.x[i] - P.x[j];
//             float dy = P.y[i] - P.y[j];
//             float dz = P.z[i] - P.z[j];
//             float r = std::sqrt(dx*dx + dy*dy + dz*dz);
//             float w = cubic_spline_W(r, h);
//             rho += P.mass[j] * w;
//         }
//         P.density[i] = rho;
//     }
// }


#include "../include/density.hpp"
#include <algorithm>
#include <cmath>
#include <vector>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


void compute_density_kNN(Particles& P, int k) {
    size_t N = P.N;
    if (N == 0 || k <= 0) return;

    P.density.assign(N, 0.0f);

    // For each particle, compute distances to all others
    for (size_t i = 0; i < N; i++) {
        std::vector<float> dists;
        dists.reserve(N-1);

        for (size_t j = 0; j < N; j++) {
            if (i == j) continue;

            float dx = P.x[j] - P.x[i];
            float dy = P.y[j] - P.y[i];
            float dz = P.z[j] - P.z[i];
            float dist2 = dx*dx + dy*dy + dz*dz;

            dists.push_back(std::sqrt(dist2));
        }

        // Find the k-th neighbor distance
        std::nth_element(dists.begin(), dists.begin()+k, dists.end());
        float r_k = dists[k];

        // Estimate density: rho ≈ k / (4/3 π r_k³)
        float volume = (4.0f / 3.0f) * M_PI * r_k * r_k * r_k;
        P.density[i] = k / volume;
    }
}


// cubic spline kernel W(r, h) for 3D
// returns W (not derivative)
static inline float cubic_spline_W(float r, float h) {
    const float q = r / h;
    const float inv_h3 = 1.0f / (h*h*h);
    const float sigma = 1.0f / (M_PI) * inv_h3; // normalization for 3D
    if (q < 0.0f) return 0.0f;
    if (q < 1.0f) {
        return sigma * (1.0f - 1.5f*q*q + 0.75f*q*q*q);
    } else if (q < 2.0f) {
        float t = 2.0f - q;
        return sigma * (0.25f * t*t*t);
    } else {
        return 0.0f;
    }
}

void compute_density_sph(Particles& P, float h) {
    size_t N = P.N;
    if (N == 0) return;
    if (P.density.size() != N) P.density.assign(N, 0.0f);

    // zero densities
    for (size_t i = 0; i < N; ++i) P.density[i] = 0.0f;

    // naive O(N^2)
    for (size_t i = 0; i < N; ++i) {
        float rho_i = 0.0f;
        // include self-contribution if desired (commonly included)
        for (size_t j = 0; j < N; ++j) {
            float dx = P.x[i] - P.x[j];
            float dy = P.y[i] - P.y[j];
            float dz = P.z[i] - P.z[j];
            float r = std::sqrt(dx*dx + dy*dy + dz*dz);
            float w = cubic_spline_W(r, h);
            rho_i += P.mass[j] * w;
        }
        P.density[i] = rho_i;
    }
}
