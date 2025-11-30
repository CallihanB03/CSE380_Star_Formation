#include <cmath> 

#pragma once

struct Vec3 {
    float x, y, z;

    Vec3() : x(0), y(0), z(0) {}
    Vec3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}

    Vec3& operator+=(const Vec3& other) {
        x += other.x; y += other.y; z += other.z;
        return *this;
    }

    Vec3& operator-=(const Vec3& other) {
        x -= other.x; y -= other.y; z -= other.z;
        return *this;
    }

        // Multiply Vec3 by scalar
    Vec3 operator*(float s) const {
        return Vec3{x * s, y * s, z * s};
    }

    // Add this operator
    Vec3 operator/(float s) const {
        return Vec3(x/s, y/s, z/s);
    }

    Vec3 operator-(const Vec3& v) const {
    return Vec3(x - v.x, y - v.y, z - v.z);
}

};
