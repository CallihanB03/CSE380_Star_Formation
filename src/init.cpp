// init.cpp
#include "../include/init.hpp"
#include <random>
#include <cmath>
#include <iostream>

static std::mt19937 gen(123);

void init_uniform(Particles& P) {
    size_t N = P.N;
    float spacing = 1.0f / std::cbrt(N);

    for (size_t i = 0; i < N; ++i) {
        float t = i * spacing;
        P.x[i] = t;
        P.y[i] = t;
        P.z[i] = t;
        P.vx[i] = P.vy[i] = P.vz[i] = 0.0f;
    }
}

void init_clustered(Particles& P) {
    std::normal_distribution<float> dist(0.0f, 0.05f);
    std::vector<Vec3> centers = {
        {0.3f,0.3f,0.3f},
        {0.6f,0.5f,0.4f},
        {0.8f,0.2f,0.7f}
    };

    for (size_t i = 0; i < P.N; ++i) {
        Vec3 c = centers[i % centers.size()];
        P.x[i] = c.x + dist(gen);
        P.y[i] = c.y + dist(gen);
        P.z[i] = c.z + dist(gen);
        P.vx[i] = P.vy[i] = P.vz[i] = 0.0f;
    }
}

void init_spherical(Particles& P) {
    std::uniform_real_distribution<float> r_dist(0.0f, 0.3f);
    std::uniform_real_distribution<float> th_dist(0.0f, 2*M_PI);
    std::uniform_real_distribution<float> ph_dist(0.0f, M_PI);

    for (size_t i = 0; i < P.N; ++i) {
        float r = r_dist(gen);
        float th = th_dist(gen);
        float ph = ph_dist(gen);
        P.x[i] = r * std::sin(ph) * std::cos(th) + 0.5;
        P.y[i] = r * std::sin(ph) * std::sin(th) + 0.5;
        P.z[i] = r * std::cos(ph) + 0.5;
        P.vx[i] = P.vy[i] = P.vz[i] = 0.0f;
    }
}


void identity_for_debugging(Particles& P){
    P.x[0] = 1.0;
    P.y[0] = 0.0;
    P.z[0] = 0.0;
    P.vx[0] = P.vy[0] = P.vz[0] = 0.0f;

    P.x[1] = 0.0;
    P.y[1] = 1.0;
    P.z[1] = 0.0;
    P.vx[1] = P.vy[1] = P.vz[1] = 0.0f;

    P.x[2] = 0.0;
    P.y[2] = 0.0;
    P.z[2] = 1.0;
    P.vx[2] = P.vy[2] = P.vz[2] = 0.0f;
}


void init_particles(Particles& P, int version_type) {
    switch (version_type) {
        case 1:
            std::cout << "Using uniform initialization\n";
            init_uniform(P);
            break;

        case 2:
            std::cout << "Using clustered initialization\n";
            init_clustered(P);
            break;

        case 3:
            std::cout << "Using spherical cloud initialization\n";
            init_spherical(P);
            break;

        case 4:
            std::cout << "Using identity initialization for debugging\n";
            identity_for_debugging(P);
            break;

        default:
            std::cout << "Unknown version, defaulting to uniform\n";
            init_uniform(P);
            break;
    }
}
