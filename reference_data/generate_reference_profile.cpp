#include <iostream>
#include <fstream>
#include <cmath>

// Generate analytic isothermal sphere density profile:
//   rho(r) = rho0 / (1 + (r / r0)^2)

int main() {
    const float rho0   = 10.0f;   // central density
    const float r0     = 1.0f;   // core radius
    const float r_max  = 1.01f;   // outer radius of profile
    const int   N_bins = 50;    // number of radial samples

    std::ofstream out("reference_data/reference_profile.csv");
    if (!out.is_open()) {
        std::cerr << "ERROR: Could not write to data/reference_profile.csv\n";
        return 1;
    }

    out << "radius,density\n";

    for (int i = 0; i < N_bins; i++) {
        float r = r_max * (float)i / (float)(N_bins - 1);
        float rho = rho0 / (1.0f + (r*r) / (r0*r0));
        out << r << "," << rho << "\n";
    }

    out.close();

    std::cout << "Generated data/reference_profile.csv with "
              << N_bins << " samples.\n";

    return 0;
}
