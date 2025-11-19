// tests/test_momentum.cpp
#include "../include/integrator.hpp"
#include <iostream>
#include <cmath>

int main() {
    Integrator integ(0.0001);

    Body p1({-1,0,0}, {1,0,0}, 1);
    Body p2({1,0,0}, {-1,0,0}, 1);
    std::vector<Body> bodies = {p1, p2};

    auto initial_P = integ.total_momentum(bodies);

    for (int i = 0; i < 10000; ++i)
        integ.step(bodies);

    auto final_P = integ.total_momentum(bodies);

    std::cout << "Initial P: " << initial_P.x << "\n";
    std::cout << "Final   P: " << final_P.x << "\n";

    if (std::abs(final_P.x - initial_P.x) < 1e-4) {
        std::cout << "PASS\n"; return 0;
    } else {
        std::cout << "FAIL\n"; return 1;
    }
}
