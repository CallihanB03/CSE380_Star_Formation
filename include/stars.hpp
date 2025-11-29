#pragma once
#include "vec3.hpp"

struct Star {
    float mass;
    Vec3 position;
    Vec3 velocity;
    float formation_time;

    Star(float m, const Vec3& pos, const Vec3& vel, float t)
        : mass(m), position(pos), velocity(vel), formation_time(t) {}
};
