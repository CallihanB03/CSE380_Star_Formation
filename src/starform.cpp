// #include "starform.hpp"
// #include <algorithm>
// #include <iostream>

// inline Vec3 get_pos(const Particles& P, size_t i) {
//     return Vec3(P.x[i], P.y[i], P.z[i]);
// }

// inline Vec3 get_vel(const Particles& P, size_t i) {
//     return Vec3(P.vx[i], P.vy[i], P.vz[i]);
// }

// int StarFormation::count_neighbors(const Particles& P, int idx) const {
//     if (!P.alive[idx]) return 0;

//     Vec3 xi = get_pos(P, idx);
//     float R2 = neighbor_radius * neighbor_radius;
//     int count = 0;

//     for (size_t j = 0; j < P.N; j++) {
//         if (j == (size_t)idx) continue;
//         if (!P.alive[j]) continue;

//         Vec3 xj = get_pos(P, j);
//         float dist2 = (xj - xi).length2();
//         if (dist2 < R2) count++;
//     }
//     return count;
// }

// float StarFormation::compute_local_density(const Particles& P, int idx) const {
//     if (!P.alive[idx]) return 0.0f;

//     Vec3 xi = get_pos(P, idx);
//     float R2 = neighbor_radius * neighbor_radius;

//     float rho = 0.0f;
//     for (size_t j = 0; j < P.N; j++) {
//         if (!P.alive[j]) continue;

//         Vec3 xj = get_pos(P, j);
//         float dist2 = (xj - xi).length2();
//         if (dist2 < R2) rho += P.mass[j];
//     }
//     return rho;
// }

// std::vector<int> StarFormation::detect_star_candidates(const Particles& P) const {
//     std::vector<int> candidates;
//     candidates.reserve(P.N);

//     for (size_t i = 0; i < P.N; i++) {
//         if (!P.alive[i]) continue;

//         int nn = count_neighbors(P, i);
//         float rho = compute_local_density(P, i);

//         if (nn >= (int)min_neighbors && rho >= min_density)
//             candidates.push_back(i);
//     }

//     return candidates;
// }

// void StarFormation::form_stars(
//     Particles& P,
//     std::vector<int>& idxs,
//     float current_time
// ) {
//     std::sort(idxs.begin(), idxs.end(), std::greater<int>());

//     for (int idx : idxs) {
//         if (!P.alive[idx]) continue;

//         float mass = P.mass[idx];
//         Vec3 pos = get_pos(P, idx);
//         Vec3 vel = get_vel(P, idx);

//         // build star directly
//         Star s(mass, pos, vel, current_time);
//         P.stars.push_back(s);

//         // kill particle
//         P.alive[idx] = false;
//         P.is_star[idx] = true;
//     }
// }



#include "starform.hpp"
#include <algorithm>
#include <iostream>

inline Vec3 get_pos(const Particles& P, size_t i) {
    return Vec3(P.x[i], P.y[i], P.z[i]);
}

inline Vec3 get_vel(const Particles& P, size_t i) {
    return Vec3(P.vx[i], P.vy[i], P.vz[i]);
}

int StarFormation::count_neighbors(const Particles& P, int idx) const {
    if (!P.alive[idx]) return 0;

    Vec3 xi = get_pos(P, idx);
    float R2 = neighbor_radius * neighbor_radius;
    int count = 0;

    for (size_t j = 0; j < P.N; j++) {
        if (j == (size_t)idx) continue;
        if (!P.alive[j]) continue;

        Vec3 xj = get_pos(P, j);
        float dist2 = (xj - xi).length2();
        if (dist2 < R2) count++;
    }
    return count;
}

// float StarFormation::compute_local_density(const Particles& P, int idx) const {
//     if (!P.alive[idx]) return 0.0f;

//     Vec3 xi = get_pos(P, idx);
//     float R2 = neighbor_radius * neighbor_radius;

//     float rho = 0.0f;
//     for (size_t j = 0; j < P.N; j++) {
//         if (!P.alive[j]) continue;

//         Vec3 xj = get_pos(P, j);
//         float dist2 = (xj - xi).length2();
//         if (dist2 < R2) rho += P.mass[j];
//     }
//     return rho;
// }

float StarFormation::compute_local_density(const Particles& P, int idx) const {
    if (!P.alive[idx]) return 0.0f;

    Vec3 xi = get_pos(P, idx);
    float R2 = neighbor_radius * neighbor_radius;

    float rho = 0.0f;
    int neighbor_count = 0;

    for (size_t j = 0; j < P.N; j++) {
        if (!P.alive[j]) continue;

        Vec3 xj = get_pos(P, j);
        float dist2 = (xj - xi).length2();
        if (dist2 < R2) {
            rho += P.mass[j];
            neighbor_count++;
        }
    }

    // Avoid division by zero / too low density
    if (neighbor_count == 0) {
        rho = 1e-5f;  // tiny floor density
    }

    return rho;
}


std::vector<int> StarFormation::detect_star_candidates(const Particles& P) const {
    std::vector<int> candidates;
    candidates.reserve(P.N);

    for (size_t i = 0; i < P.N; i++) {
        if (!P.alive[i]) continue;

        int nn = count_neighbors(P, i);
        float rho = compute_local_density(P, i);

        if (nn >= (int)min_neighbors && rho >= min_density)
            candidates.push_back(i);
    }

    return candidates;
}

void StarFormation::form_stars(
    Particles& P,
    const std::vector<int>& idxs,
    float current_time
) {
    for (int idx : idxs) {
        if (!P.alive[idx]) continue;

        // mark particle as star
        P.is_star[idx] = true;

        // add to star catalog (still keep particle in simulation)
        float mass = P.mass[idx];
        Vec3 pos = get_pos(P, idx);
        Vec3 vel = get_vel(P, idx);

        Star s(mass, pos, vel, current_time);
        P.stars.push_back(s);
    }
}
