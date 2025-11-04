#include "../include/config.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

Config load_config(const std::string &filename) {
    Config cfg;
    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        std::istringstream iss(line);
        std::string key;
        if (std::getline(iss, key, '=')) {
            std::string value;
            if (std::getline(iss, value)) {
                if (key.find("timestep") != std::string::npos)
                    cfg.timestep = std::stod(value);
                else if (key.find("num_steps") != std::string::npos)
                    cfg.num_steps = std::stoi(value);
                else if (key.find("grid_points") != std::string::npos)
                    cfg.grid_points = std::stoi(value);
                else if (key.find("initial_density") != std::string::npos)
                    cfg.initial_density = std::stod(value);
                else if (key.find("initial_temperature") != std::string::npos)
                    cfg.initial_temperature = std::stod(value);
                else if (key.find("gravitational_constant") != std::string::npos)
                    cfg.G = std::stod(value);
                else if (key.find("output_interval") != std::string::npos)
                    cfg.output_interval = std::stoi(value);
            }
        }
    }

    return cfg;
}
