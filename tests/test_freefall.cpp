#include <iostream>
#include <iomanip>

// Simple 3D vector
struct Vec3 {
    float x, y, z;

    Vec3() : x(0), y(0), z(0) {}
    Vec3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}

    Vec3 operator+(const Vec3& v) const { return Vec3(x+v.x, y+v.y, z+v.z); }
    Vec3 operator-(const Vec3& v) const { return Vec3(x-v.x, y-v.y, z-v.z); }
    Vec3 operator*(float s) const { return Vec3(x*s, y*s, z*s); }
};

// Particle struct
struct Particle {
    Vec3 position;
    Vec3 velocity;
    float mass;
};

// Velocity Verlet integration step
void velocity_verlet_step(Particle &p, const Vec3 &F, float dt) {
    Vec3 a = F * (1.0f / p.mass);       // acceleration
    p.position = p.position + p.velocity * dt + a * (0.5f * dt * dt);
    p.velocity = p.velocity + a * dt;
}

int main() {
    const float dt = 0.01f;  // timestep
    const int steps = 100;   // number of steps
    const float g = 9.81f;   // gravity

    // Initialize particle
    Particle p1;
    p1.position = Vec3(0.0f, 10.0f, 0.0f);  // 10 units high
    p1.velocity = Vec3(0.0f, 0.0f, 0.0f);
    p1.mass = 1.0f;

    std::cout << std::fixed << std::setprecision(5);

    for (int step = 0; step <= steps; ++step) {
        std::cout << "Step " << step 
                  << ": P1=(" << p1.position.x << ", " << p1.position.y << ", " << p1.position.z << ")"
                  << " V=(" << p1.velocity.x << ", " << p1.velocity.y << ", " << p1.velocity.z << ")"
                  << std::endl;

        // Force due to gravity
        Vec3 F(0.0f, -p1.mass * g, 0.0f);

        // Advance one step
        velocity_verlet_step(p1, F, dt);
    }

    return 0;
}
