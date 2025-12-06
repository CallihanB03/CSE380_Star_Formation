# Star Formation SPH Simulator

This project implements a particle-based hydrodynamics simulator using **Smoothed Particle Hydrodynamics (SPH)** and **self-gravity**. Particles represent fluid elements inside a molecular cloud. Their densities, pressures, and accelerations are computed via SPH kernels, and their motion is integrated using the Velocity-Verlet method.

This README explains **how the simulator works** and **how to run it**.

---

## How the Simulator Works

The simulator evolves a system of particles through repeated **time steps**. Each time step performs:

### 1. Density Computation
Each particle's density is estimated by summing contributions from nearby particles using the SPH smoothing kernel:

$$
\rho_i = \sum_j m_j W(r_{ij}, h)
$$

- Uses a **cubic spline kernel**
- Optionally uses a **cached neighbor list** for performance

### 2. Pressure Computation
Pressure is computed using an **equation of state**, typically:

$$
P = k \rho^\gamma
$$

This determines the magnitude of sound-speed–driven internal forces.

### 3. Pressure Force Calculation
The pressure gradient force on each particle is evaluated using SPH gradients:

$$
\mathbf{a}_i^{\text{pressure}} = -\sum_j m_j
\left(\frac{P_i}{\rho_i^2} + \frac{P_j}{\rho_j^2}\right)
\nabla W(r_{ij}, h)
$$

You may use:

- `compute_pressure_forces`
- `compute_pressure_forces_cached` (optimized)

### 4. Gravitational Acceleration
Each particle experiences Newtonian gravitational attraction:

$$
\mathbf{a}_i^{\text{grav}} = G \sum_j m_j \frac{\mathbf{r}_{ij}}{(|\mathbf{r}_{ij}|^2 + \epsilon^2)^{3/2}}
$$

The softening parameter `ε` prevents numerical blow-ups.

### 5. Time Integration (Velocity-Verlet)
Motion is advanced with a symplectic Velocity-Verlet integrator:

1. Half-step velocity update  
2. Full-step position update  
3. Recompute forces  
4. Second half-step velocity update

This conserves energy better than Euler or RK methods for long simulations.

---

## Simulation Loop (Summary)

Each iteration of the main loop does:

1. `compute_density(P, h)`
2. `compute_pressure(P)`
3. `compute_pressure_forces(P, h)`  
   or the cached version
4. `compute_gravity(P)`
5. `velocity_verlet(P, ax, ay, az, dt)`

Output is written or displayed depending on your main program setup.

---

## How to Build

Ensure you have a C++17-compatible compiler.

### Build with Make

