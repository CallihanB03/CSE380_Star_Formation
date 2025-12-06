#pragma once
#include <vector>
#include<iostream>
#include "particles.hpp"

bool load_reference_csv(const std::string& filename,
                        std::vector<float>& radius,
                        std::vector<float>& density);


std::vector<float> compute_radial_profile(const Particles& P,
                                          int bins,
                                          float max_r);




void verify_against_reference(const Particles& P,
                              const std::string& reference_csv);



struct ErrorMetrics {
    float L1, L2, Linf;
};

ErrorMetrics compute_error_metrics(const std::vector<float>& sim,
                                   const std::vector<float>& ref);