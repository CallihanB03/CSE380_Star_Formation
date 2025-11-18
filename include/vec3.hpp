#pragma once

struct Vec3 {
    float x, y, z;

    Vec3(float x_=0, float y_=0, float z_=0)
        : x(x_), y(y_), z(z_) {}

    Vec3 operator-(const Vec3& rhs) const {
        return Vec3(x - rhs.x, y - rhs.y, z - rhs.z);
    }

    float norm() const {
        return std::sqrt(x*x + y*y + z*z);
    }
};
