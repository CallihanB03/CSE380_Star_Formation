#include <vector>
#include <queue>
#include <cmath>
#include <iostream>
#include "../include/vec3.hpp"
#include "../include/particles.hpp"

/// squared distance between particle i and j
static inline float dist2(const Particles& P, int i, int j) {
    float dx = P.x[i] - P.x[j];
    float dy = P.y[i] - P.y[j];
    float dz = P.z[i] - P.z[j];
    return dx*dx + dy*dy + dz*dz;
}

/// ----------------------------------------------------------
/// FIND CLUSTERS USING BFS
/// ----------------------------------------------------------
std::vector<std::vector<int>>
find_clusters(const Particles& P, float cluster_radius)
{
    float r2 = cluster_radius * cluster_radius;
    int N = P.N;

    std::vector<bool> visited(N, false);
    std::vector<std::vector<int>> clusters;

    for (int i = 0; i < N; i++) {
        if (!P.alive[i] || visited[i]) continue;

        std::queue<int> q;
        q.push(i);
        visited[i] = true;

        std::vector<int> cluster;
        cluster.push_back(i);

        while (!q.empty()) {
            int p = q.front();
            q.pop();

            for (int j = 0; j < N; j++) {
                if (!P.alive[j] || visited[j]) continue;

                if (dist2(P, p, j) < r2) {
                    visited[j] = true;
                    q.push(j);
                    cluster.push_back(j);
                }
            }
        }

        clusters.push_back(cluster);
    }

    return clusters;
}

/// ----------------------------------------------------------
/// STAR FORMATION FROM MASSIVE CLUSTERS
/// ----------------------------------------------------------
struct FormedStar {
    float x, y, z;
    float mass;
};

// std::vector<FormedStar>
// form_stars_from_clusters(Particles& P,
//                          float cluster_radius,
//                          float mass_threshold)
// {
//     std::vector<FormedStar> stars;
//     auto clusters = find_clusters(P, cluster_radius);

//     for (const auto& cluster : clusters) {
//         float total_mass = 0.0f;

//         float wx = 0.0f;
//         float wy = 0.0f;
//         float wz = 0.0f;

//         for (int idx : cluster) {
//             float m = P.mass[idx];
//             total_mass += m;
//             wx += m * P.x[idx];
//             wy += m * P.y[idx];
//             wz += m * P.z[idx];
//         }

//         if (total_mass >= mass_threshold) {
//             float cx = wx / total_mass;
//             float cy = wy / total_mass;
//             float cz = wz / total_mass;

//             stars.push_back({cx, cy, cz, total_mass});

//             // Mark all cluster particles as merged/dead
//             for (int idx : cluster) {
//                 P.alive[idx] = false;
//             }
//         }
//     }

//     return stars;
// }

std::vector<size_t> form_stars_from_clusters(
    Particles& P,
    float cluster_radius,
    float mass_threshold,
    float time)
{
    size_t N = P.N;
    std::vector<size_t> new_stars;

    std::vector<bool> visited(N, false);

    for (size_t i = 0; i < N; ++i) {
        if (!P.alive[i] || visited[i]) continue;

        // Build the cluster
        std::vector<size_t> cluster;
        std::vector<size_t> stack = {i};

        while (!stack.empty()) {
            size_t j = stack.back();
            stack.pop_back();

            if (visited[j] || !P.alive[j]) continue;

            visited[j] = true;
            cluster.push_back(j);

            for (size_t k = 0; k < N; ++k) {
                if (!P.alive[k] || visited[k]) continue;

                float dx = P.x[j] - P.x[k];
                float dy = P.y[j] - P.y[k];
                float dz = P.z[j] - P.z[k];

                if (dx*dx + dy*dy + dz*dz < cluster_radius*cluster_radius)
                    stack.push_back(k);
            }
        }

        // Compute cluster mass
        float total_mass = 0;
        Vec3 pos_sum(0,0,0);
        Vec3 vel_sum(0,0,0);

        for (size_t p : cluster) {
            total_mass += P.mass[p];
            pos_sum += Vec3(P.x[p], P.y[p], P.z[p]) * P.mass[p];
            vel_sum += Vec3(P.vx[p], P.vy[p], P.vz[p]) * P.mass[p];
        }

        if (total_mass >= mass_threshold) {
            // Create a new star
            Vec3 star_pos = pos_sum / total_mass;
            Vec3 star_vel = vel_sum / total_mass;

            P.stars.emplace_back(total_mass, star_pos, star_vel, time);
            new_stars.push_back(P.stars.size() - 1);

            // Remove cluster particles
            for (size_t p : cluster)
                P.alive[p] = false;
        }
    }

    return new_stars;
}