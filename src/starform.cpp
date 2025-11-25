#include "../include/starform.hpp"
#include <queue>
#include <cmath>

std::vector<std::vector<size_t>> find_star_clusters(const Particles& P,
                                                    const std::vector<size_t>& candidates,
                                                    float linking_length) {
    std::vector<std::vector<size_t>> clusters;
    size_t M = candidates.size();
    if (M == 0) return clusters;

    std::vector<bool> visited(P.N, false);
    float L2 = linking_length * linking_length;

    for (size_t idx = 0; idx < M; ++idx) {
        size_t i = candidates[idx];
        if (visited[i]) continue;

        std::vector<size_t> cluster;
        std::queue<size_t> q;
        q.push(i);
        visited[i] = true;

        while (!q.empty()) {
            size_t u = q.front(); q.pop();
            cluster.push_back(u);

            // search through candidates (fine because candidate list << N typically)
            for (size_t jj = 0; jj < M; ++jj) {
                size_t v = candidates[jj];
                if (visited[v]) continue;
                float dx = P.x[u] - P.x[v];
                float dy = P.y[u] - P.y[v];
                float dz = P.z[u] - P.z[v];
                float dist2 = dx*dx + dy*dy + dz*dz;
                if (dist2 <= L2) {
                    visited[v] = true;
                    q.push(v);
                }
            }
        }
        clusters.push_back(cluster);
    }
    return clusters;
}

void form_sinks_from_clusters(Particles& P, const std::vector<std::vector<size_t>>& clusters) {
    // Very simple behavior: mark the first particle in each cluster as a star
    // and mark all cluster members as is_star = true.
    // (Later you can remove cluster particles and create sink objects.)
    for (const auto &c : clusters) {
        if (c.empty()) continue;
        size_t seed = c[0];
        P.is_star[seed] = true;
        for (size_t k = 1; k < c.size(); ++k) {
            P.is_star[c[k]] = true; // or mark as accreted/removed later
        }
    }
}
