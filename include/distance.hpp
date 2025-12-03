#include <cstdlib> // For malloc and free
#include "particles.hpp"
#include <iostream>
#include <vector>


std::vector<float> compute_particle_distances(Particles& P){
    size_t N = P.N;

    int pos_arr_size = 0;
    for (int i=1; i<N; i++){
        pos_arr_size++;
    }

    std::vector<float> particle_dist_arr;

    for (size_t i = 0; i < N; i++){
        for (size_t j = i + 1; j < N; j++) {
                float dx = P.x[i] - P.x[j];
                float dy = P.y[i] - P.y[j];
                float dz = P.z[i] - P.z[j];
                particle_dist_arr.push_back(dx*dx + dy*dy + dz*dz);
            }
        }
    return particle_dist_arr;
    }