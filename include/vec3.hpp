#include <cmath> 

#pragma once

// struct Vec3 {
//     float x, y, z;

//     Vec3(float x_=0, float y_=0, float z_=0)
//         : x(x_), y(y_), z(z_) {}

//     Vec3 operator-(const Vec3& rhs) const {
//         return Vec3(x - rhs.x, y - rhs.y, z - rhs.z);
//     }

//     float norm() const {
//         return std::sqrt(x*x + y*y + z*z);
//     }
// };


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
};
