#pragma once
#include <string>
#include <unordered_map>

struct Config {
    double timestep;
    int num_steps;
    int grid_points;
    double initial_density;
    double initial_temperature;
    double G;
    int output_interval;
};

Config load_config(const std::string &filename);
