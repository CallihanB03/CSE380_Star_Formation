#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include "../include/particles.hpp"
#include "../include/vec3.hpp"

// -------------------------------------------------------
// Load reference 2-column CSV (radius, density)
// -------------------------------------------------------
bool load_reference_csv(const std::string& filename,
                        std::vector<float>& radius,
                        std::vector<float>& density)
{
    std::ifstream f(filename);
    if (!f.is_open()) {
        std::cerr << "ERROR: Could not open reference file: " << filename << "\n";
        return false;
    }

    std::string line;
    while (std::getline(f, line)) {
        if (line.size() == 0) continue;

        std::stringstream ss(line);
        float r, rho;
        char comma;

        if (ss >> r >> comma >> rho) {
            radius.push_back(r);
            density.push_back(rho);
        }
    }

    return true;
}

// -------------------------------------------------------
// Compute radial density from your particle data
// -------------------------------------------------------
std::vector<float> compute_radial_profile(const Particles& P,
                                           int bins,
                                           float max_r)
{
    std::vector<float> mass_in_bin(bins, 0.0);

    // Accumulate mass in each radial bin
    for (size_t i = 0; i < P.N; ++i) {
        if (!P.alive[i]) continue;

        float r = std::sqrt(P.x[i]*P.x[i] +
                             P.y[i]*P.y[i] +
                             P.z[i]*P.z[i]);

        int b = std::min(int((r / max_r) * bins), bins - 1);

        // If you have equal-mass particles:
        mass_in_bin[b] += P.mass[i];  // or 1.0f if equal mass
    }

    // Convert mass in each shell to density
    std::vector<float> rho_shell(bins, 0.0);

    for (int k = 0; k < bins; ++k) {
        float r_lo = (float)k     / bins * max_r;
        float r_hi = (float)(k+1) / bins * max_r;

        float vol = (4.0 * M_PI / 3.0) *
                     (r_hi*r_hi*r_hi - r_lo*r_lo*r_lo);

        if (vol > 0.0)
            rho_shell[k] = mass_in_bin[k] / vol;
    }

    return rho_shell;
}


// -------------------------------------------------------
// Compute L1 / L2 / Linf error values
// -------------------------------------------------------
struct ErrorMetrics {
    float L1, L2, Linf;
};

ErrorMetrics compute_error_metrics(const std::vector<float>& sim,
                                   const std::vector<float>& ref)
{
    int n = std::min(sim.size(), ref.size());
    float s1 = 0.f, s2 = 0.f, s_inf = 0.f;

    for (int i = 0; i < n; i++) {
        float diff = std::abs(sim[i] - ref[i]);
        s1 += diff;
        s2 += diff * diff;
        s_inf = std::max(s_inf, diff);
    }

    return {
        s1 / n,
        std::sqrt(s2 / n),
        s_inf
    };
}

// -------------------------------------------------------
// Top-level verification function
// -------------------------------------------------------
void verify_against_reference(const Particles& P,
                              const std::string& reference_csv)
{
    std::vector<float> r_ref, rho_ref;

    if (!load_reference_csv(reference_csv, r_ref, rho_ref)) {
        std::cerr << "Verification aborted — reference file missing.\n";
        return;
    }

    int bins = r_ref.size();
    float max_r = r_ref.back();

    std::vector<float> rho_sim =
        compute_radial_profile(P, bins, max_r);

    ErrorMetrics E =
        compute_error_metrics(rho_sim, rho_ref);

    std::cout << "\n=== Verification Report ===\n";
    std::cout << "Reference: " << reference_csv << "\n";
    std::cout << "Particles: " << P.N << "\n";
    std::cout << "Bins:      " << bins << "\n\n";
    std::cout << "L1 Error:   " << E.L1 << "\n";
    std::cout << "L2 Error:   " << E.L2 << "\n";
    std::cout << "Linf Error: " << E.Linf << "\n\n";

}
