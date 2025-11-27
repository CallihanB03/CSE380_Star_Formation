#include "../include/physics.hpp"


// void update_physics(Particles& P, float dt) {
//     // 1. Density
//     compute_density_kNN(P, 16);

//     // 2. Pressure (simple EOS)
//     for (size_t i = 0; i < P.N; ++i) {
//         float gamma = 5.0f / 3.0f;
//         float K = 1.0f; // constant, can tune
//         P.pressure[i] = K * std::pow(P.density[i], gamma);
//     }

//     // 3. Temperature (optional, simple scaling)
//     for (size_t i = 0; i < P.N; ++i) {
//         P.temperature[i] = P.pressure[i] / P.density[i];
//     }
// }
