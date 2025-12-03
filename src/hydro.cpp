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

// Optimization 3
// Computes SPH pressure forces: a_i = sum_j -m_j (P_i/rho_i^2 + P_j/rho_j^2) grad W_ij
void compute_pressure_forces(
    Particles& P,
    float h,
    std::vector<float>& ax,
    std::vector<float>& ay,
    std::vector<float>& az
) {
    size_t N = P.N;

    // resize acceleration vectors if needed
    if (ax.size() != N) ax.assign(N, 0.0f);
    if (ay.size() != N) ay.assign(N, 0.0f);
    if (az.size() != N) az.assign(N, 0.0f);

    // small epsilon to avoid division by zero
    const float eps = 1e-12f;

    // pairwise loop
    for (size_t i = 0; i < N; ++i) {
        if (!P.alive[i]) continue;

        float rho_i = std::max(P.density[i], eps);
        float rho_i2_inv = 1.0f / (rho_i * rho_i);
        float P_i_rho2 = P.pressure[i] * rho_i2_inv;

        float xi = P.x[i]; float yi = P.y[i]; float zi = P.z[i];

        // Vec3 xi(P.x[i], P.y[i], P.z[i]);

        for (size_t j = i + 1; j < N; ++j) {
            if (!P.alive[j]) continue;

            // Vec3 xj(P.x[j], P.y[j], P.z[j]);

            float xj = P.x[j]; float yj = P.y[j]; float zj = P.z[j];

            // Vec3 dx = xj - xi;
            float dx = (xj - xi); float dy = (yj - yi); float dz = (zj - zi);
            float r = std::sqrt( dx*dx + dy*dy + dz*dz );
            // float r2 = dx.length2();
            // float r = std::sqrt(r2);

            if (r > 2.0f * h || r < eps) continue;

            float dWdr = cubic_spline_dWdr(r, h);
            if (dWdr == 0.0f) continue;

            float rho_j = std::max(P.density[j], eps);
            float rho_j2_inv = 1.0f / (rho_j * rho_j);
            float P_j_rho2 = P.pressure[j] * rho_j2_inv;

            float term = -P.mass[j] * (P_i_rho2 + P_j_rho2);

            // Vec3 gradW = dx * (dWdr / r); // grad W_ij
            // Vec3 a = gradW * term;

            float grad_scalar = (dWdr / r);
            float grad_x = dx * grad_scalar;
            float grad_y = dy * grad_scalar;
            float grad_z = dz * grad_scalar;

            float a_x = grad_x * term;
            float a_y = grad_y * term;
            float a_z = grad_z * term;

            // symmetric update (momentum conserved)
            // ax[i] += a.x;
            // ay[i] += a.y;
            // az[i] += a.z;

            // ax[j] -= a.x;
            // ay[j] -= a.y;
            // az[j] -= a.z;

            // symmetric update (momentum conserved)
            ax[i] += a_x;
            ay[i] += a_y;
            az[i] += a_z;

            ax[j] -= a_x;
            ay[j] -= a_y;
            az[j] -= a_z;
        }
    }
}

// This is where the thermodynamics and kinetics kiss
void compute_pressure_forces_cached(
    Particles& P,
    float h
) {
    size_t N = P.N;

    // small epsilon to avoid division by zero
    const float eps = 1e-12f;

    // pairwise loop
    for (size_t i = 0; i < N; ++i) {
        if (!P.alive[i]) continue;

        float rho_i = std::max(P.density[i], eps);
        float rho_i2_inv = 1.0f / (rho_i * rho_i);
        float P_i_rho2 = P.pressure[i] * rho_i2_inv;

        float xi = P.x[i]; float yi = P.y[i]; float zi = P.z[i];

        for (size_t j = i + 1; j < N; ++j) {
            if (!P.alive[j]) continue;

            float xj = P.x[j]; float yj = P.y[j]; float zj = P.z[j];

            float dx = (xj - xi); float dy = (yj - yi); float dz = (zj - zi);
            float r = std::sqrt( dx*dx + dy*dy + dz*dz );

            if (r > 2.0f * h || r < eps) continue;

            float dWdr = cubic_spline_dWdr(r, h);
            if (dWdr == 0.0f) continue;

            float rho_j = std::max(P.density[j], eps);
            float rho_j2_inv = 1.0f / (rho_j * rho_j);
            float P_j_rho2 = P.pressure[j] * rho_j2_inv;

            float term = -P.mass[j] * (P_i_rho2 + P_j_rho2);

            float grad_scalar = (dWdr / r);
            float grad_x = dx * grad_scalar;
            float grad_y = dy * grad_scalar;
            float grad_z = dz * grad_scalar;

            float a_x = grad_x * term;
            float a_y = grad_y * term;
            float a_z = grad_z * term;

            // symmetric update (momentum conserved)
            P.ax[i] += a_x;
            P.ay[i] += a_y;
            P.az[i] += a_z;

            P.ax[j] -= a_x;
            P.ay[j] -= a_y;
            P.az[j] -= a_z;
        }
    }
}