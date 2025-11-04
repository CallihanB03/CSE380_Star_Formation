#include "../include/config.hpp"
#include "../include/simulation.hpp"
#include <iostream>

int main() {
    Config cfg = load_config("data/parameters.txt");
    Simulation sim(cfg);
    sim.run();
    return 0;
}
