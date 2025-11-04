#include "../include/simulation.hpp"
#include <iostream>
#include <fstream>
#include <cmath>

Simulation::Simulation(const Config& config) : cfg(config) {
    initialize();
}

void Simulation::initialize() {
    density.resize(cfg.grid_points, cfg.initial_density);
    velocity.resize(cfg.grid_points, 0.0);
    potential.resize(cfg.grid_points, 0.0);
}

void Simulation::step() {
    // Placeholder for numerical update
    for (int i = 0; i < cfg.grid_points; ++i) {
        density[i] += 0.001 * std::sin(i * 0.1); // dummy update
    }
}

void Simulation::output(int step) {
    std::ofstream out("data/output_" + std::to_string(step) + ".csv");
    out << "i,density,velocity,potential\n";
    for (int i = 0; i < cfg.grid_points; ++i)
        out << i << "," << density[i] << "," << velocity[i] << "," << potential[i] << "\n";
}

void Simulation::run() {
    std::cout << "Running simulation with " << cfg.num_steps << " steps...\n";
    for (int iter = 0; iter < cfg.num_steps; ++iter) {
        this->step();  // call the member function
        if (iter % cfg.output_interval == 0) output(iter);
    }
    std::cout << "Simulation complete.\n";
}

