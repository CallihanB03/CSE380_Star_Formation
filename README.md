# Early Star Formation Simulation — C++ Project

A lightweight 3-dimensional particle-based numerical simulation of early star formation using simplified smoothed-particle–hydrodynamics (SPH)–style physics and gravitational clustering.

This project was developed for **CSE 380: Tools and Techniques in Computational Science** and focuses on **code structure, profiling, performance optimization, and modular scientific simulation design**.

---

## Overview

This simulation models a gas cloud represented by discrete particles, each carrying position, velocity, mass, density, pressure, and temperature. The system evolves under:

- Newtonian gravity  
- Simplified hydrodynamics  
- A minimal thermodynamic model  
- Cluster identification and **sink/star particle formation**

As particles collapse into gravitationally bound clusters above a mass threshold, they are converted into **stars**, which are stored and logged for analysis.

The simulation emphasizes:

- Clean modular C++ design  
- Efficient data layout (SoA arrays, Vec3 caching)  
- Profiling with `gprof` or `perf`  
- Optimization strategies (r² caching, avoiding repeated sqrt, neighbor loops, etc.)  
- Code coverage and testing  
- Input/output for scientific results  

---

## 🔧 Features

### Particle System
Each particle stores:

- Position: `x, y, z`  
- Velocity: `vx, vy, vz`  
- Thermodynamics: `temperature`, `density`, `pressure`  
- Mass (`float`)  
- Status flags (`alive`, `is_star`)  
- Cached accelerations (`acc_cache`)  
- Cached squared distances (`r2_cache`)

The structure-of-arrays layout improves data locality and computational efficiency.

---

### Physics Modules
The project is modular, with separate compilation units:

| Module            | Purpose                                           |
|------------------|--------------------------------------------------|
| `gravity.cpp`     | Gravitational acceleration computation (cached pairwise r²) |
| `density.cpp`     | Density estimation using local particle neighborhoods |
| `hydro.cpp`       | Simplified pressure and thermodynamic update    |
| `physics.cpp`     | Equation of state and combined physical logic   |
| `integrator.cpp`  | Time integration (Leapfrog / Velocity-Verlet)   |
| `starform.cpp`    | Cluster detection + star formation              |
| `simulation.cpp`  | Timestep loop controller                         |
| `init.cpp`        | Initial conditions (random spheres, disks, etc.) |
| `main.cpp`        | Runs the simulation                              |

---

## ⭐ Star Formation Model

### Cluster Detection
At each timestep, particles are grouped into clusters based on:

- A spherical neighbor radius (`cluster_radius`)  
- Particles must be alive (`alive == true`)  

### Star Formation Criteria
A cluster becomes a star when:

- The total cluster mass ≥ `mass_threshold`  

When a star forms:

- All gas particles in the cluster are marked `alive = false`  
- A `FormedStar` struct is appended to `Particles.stars`:

```cpp
struct FormedStar {
    float mass;
    float x, y, z;
    float vx, vy, vz;
    float formation_time;
};
