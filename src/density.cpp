#include "../include/density.hpp"
#include <algorithm>
#include <vector>
#include <cmath>

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
