#include "../include/hydro.hpp"
#include <cmath>
#include <algorithm>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// cubic spline kernel derivative dW/dr for 3D; returns dWdr
static inline float cubic_spline_dWdr(float r, float h) {
    const float q = r / h;
    const float inv_h4 = 1.0f / (h*h*h*h);
    const float sigma = 1.0f / (M_PI) * inv_h4; // normalization for derivative in 3D
    if (r <= 0.0f) return 0.0f;
    if (q < 1.0f) {
        // d/dr [1 - 1.5 q^2 + 0.75 q^3] = (-3 q + (9/4) q^2) * (1/h)
        return sigma * (-3.0f * q + 2.25f * q * q);
    } else if (q < 2.0f) {
        float t = 2.0f - q;
        // d/dr[0.25 t^3] = 0.25 * 3 * t^2 * (-1/h) = -0.75 * t^2 / h
        return sigma * (-0.75f * t * t);
    } else {
        return 0.0f;
    }
}

void compute_pressure(Particles& P, float K, float gamma) {
    size_t N = P.N;
    if (P.pressure.size() != N) P.pressure.assign(N, 0.0f);
    for (size_t i = 0; i < N; ++i) {
        // polytropic EOS (robust for collapse tests)
        float rho = std::max(P.density[i], 1e-12f);
        P.pressure[i] = K * std::pow(rho, gamma);
    }
}

// a_ij contribution = - m_j * (P_i / rho_i^2 + P_j / rho_j^2) * grad W_ij
void compute_pressure_forces(Particles& P, float h,
                             std::vector<float>& ax,
                             std::vector<float>& ay,
                             std::vector<float>& az) {
    size_t N = P.N;
    if (ax.size() != N) ax.assign(N, 0.0f);
    if (ay.size() != N) ay.assign(N, 0.0f);
    if (az.size() != N) az.assign(N, 0.0f);

    // pairwise loop
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = i + 1; j < N; ++j) {
            float dx = P.x[i] - P.x[j];
            float dy = P.y[i] - P.y[j];
            float dz = P.z[i] - P.z[j];
            float r2 = dx*dx + dy*dy + dz*dz;
            float r = std::sqrt(r2);
            if (r > 2.0f * h) continue;

            float dWdr = cubic_spline_dWdr(r, h); // dW/dr
            if (dWdr == 0.0f) continue;

            float invr = (r > 1e-12f) ? 1.0f / r : 0.0f;
            float gradWx = dWdr * (dx * invr);
            float gradWy = dWdr * (dy * invr);
            float gradWz = dWdr * (dz * invr);

            float rho_i = std::max(P.density[i], 1e-12f);
            float rho_j = std::max(P.density[j], 1e-12f);

            float term = - (P.mass[j]) * (P.pressure[i] / (rho_i * rho_i) + P.pressure[j] / (rho_j * rho_j));

            // symmetric update
            float ax_con = term * gradWx;
            float ay_con = term * gradWy;
            float az_con = term * gradWz;

            ax[i] += ax_con;
            ay[i] += ay_con;
            az[i] += az_con;

            // opposite sign for j (Newton's 3rd law)
            ax[j] -= ax_con * (P.mass[i] / P.mass[j]); // note: term included m_j; compensate for symmetry
            ay[j] -= ay_con * (P.mass[i] / P.mass[j]);
            az[j] -= az_con * (P.mass[i] / P.mass[j]);
            // The above mass compensation keeps momentum consistent if masses differ.
            // If all masses equal, simpler: ax[j] -= ax_con; etc.
        }
    }
}
