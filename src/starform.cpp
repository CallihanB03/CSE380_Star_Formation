// #include "../include/starform.hpp"
// #include <queue>
// #include <cmath>

// std::vector<std::vector<size_t>> find_star_clusters(const Particles& P,
//                                                     const std::vector<size_t>& candidates,
//                                                     float linking_length) {
//     std::vector<std::vector<size_t>> clusters;
//     size_t M = candidates.size();
//     if (M == 0) return clusters;

//     std::vector<bool> visited(P.N, false);
//     float L2 = linking_length * linking_length;

//     for (size_t idx = 0; idx < M; ++idx) {
//         size_t i = candidates[idx];
//         if (visited[i]) continue;

//         std::vector<size_t> cluster;
//         std::queue<size_t> q;
//         q.push(i);
//         visited[i] = true;

//         while (!q.empty()) {
//             size_t u = q.front(); q.pop();
//             cluster.push_back(u);

//             // search through candidates (fine because candidate list << N typically)
//             for (size_t jj = 0; jj < M; ++jj) {
//                 size_t v = candidates[jj];
//                 if (visited[v]) continue;
//                 float dx = P.x[u] - P.x[v];
//                 float dy = P.y[u] - P.y[v];
//                 float dz = P.z[u] - P.z[v];
//                 float dist2 = dx*dx + dy*dy + dz*dz;
//                 if (dist2 <= L2) {
//                     visited[v] = true;
//                     q.push(v);
//                 }
//             }
//         }
//         clusters.push_back(cluster);
//     }
//     return clusters;
// }

// void form_sinks_from_clusters(Particles& P, const std::vector<std::vector<size_t>>& clusters) {
//     // Very simple behavior: mark the first particle in each cluster as a star
//     // and mark all cluster members as is_star = true.
//     // (Later you can remove cluster particles and create sink objects.)
//     for (const auto &c : clusters) {
//         if (c.empty()) continue;
//         size_t seed = c[0];
//         P.is_star[seed] = true;
//         for (size_t k = 1; k < c.size(); ++k) {
//             P.is_star[c[k]] = true; // or mark as accreted/removed later
//         }
//     }
// }

// src/starformation.cpp
#include "../include/starform.hpp"
#include <cmath>
#include <vector>

// simple neighbor-merge sink creation
void check_and_create_sinks(Particles& P, float rho_threshold, float r_merge) {
    size_t N = P.N;

    // visited prevents double-processing of merged groups
    std::vector<bool> visited(N, false);

    for (size_t i = 0; i < N; ++i) {
        if (!P.alive[i] || visited[i]) continue;
        if (P.density[i] < rho_threshold) continue;

        // gather neighbors within r_merge
        std::vector<size_t> neighbors;
        neighbors.push_back(i);
        visited[i] = true;

        for (size_t j = 0; j < N; ++j) {
            if (j==i) continue;
            if (!P.alive[j] || visited[j]) continue;
            float dx = P.x[j] - P.x[i];
            float dy = P.y[j] - P.y[i];
            float dz = P.z[j] - P.z[i];
            float r2 = dx*dx + dy*dy + dz*dz;
            if (r2 <= r_merge * r_merge) {
                neighbors.push_back(j);
                visited[j] = true;
            }
        }

        if (neighbors.size() <= 1) {
            // single particle above threshold -> mark as star but don't merge
            size_t idx = neighbors[0];
            P.is_star[idx] = true;
            continue;
        }

        // merge neighbors into one sink at index 'keep'
        size_t keep = neighbors[0];
        double total_m = 0.0;
        double cmx=0.0, cmy=0.0, cmz=0.0;
        double momx=0.0, momy=0.0, momz=0.0;

        for (size_t id : neighbors) {
            total_m += P.mass[id];
            cmx += double(P.x[id]) * P.mass[id];
            cmy += double(P.y[id]) * P.mass[id];
            cmz += double(P.z[id]) * P.mass[id];

            momx += double(P.vx[id]) * P.mass[id];
            momy += double(P.vy[id]) * P.mass[id];
            momz += double(P.vz[id]) * P.mass[id];
        }

        if (total_m <= 0.0) continue;
        cmx /= total_m; cmy /= total_m; cmz /= total_m;

        // set keep particle to be the sink/star
        P.mass[keep] = float(total_m);
        P.x[keep] = float(cmx);
        P.y[keep] = float(cmy);
        P.z[keep] = float(cmz);

        P.vx[keep] = float(momx / total_m);
        P.vy[keep] = float(momy / total_m);
        P.vz[keep] = float(momz / total_m);

        P.is_star[keep] = true;
        P.alive[keep] = true;

        // mark others dead
        for (size_t id : neighbors) {
            if (id == keep) continue;
            P.alive[id] = false;
            P.is_star[id] = false; // merged into keep
            // optionally zero out data
            P.mass[id] = 0.0f;
            P.vx[id] = P.vy[id] = P.vz[id] = 0.0f;
        }
    }
}
