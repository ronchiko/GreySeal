#pragma once

#include "greyseal/quaternion.h"

struct Seal_Vector3 {
public:
    float x, y, z;

    Seal_Vector3() : x(0), y(0), z(0) {}
    Seal_Vector3(const Seal_Vector3& v) : x(v.x), y(v.y), z(v.z) {}
    Seal_Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

    inline Seal_Vector3 operator+(const Seal_Vector3& o) const {
        return Seal_Vector3(x + o.x, y + o.y, z + o.z);
    }
    inline Seal_Vector3 operator-(const Seal_Vector3& o) const {
        return Seal_Vector3(x - o.x, y - o.y, z - o.z);
    }
    inline Seal_Vector3 operator*(float f) const {
        return Seal_Vector3(x * f, y * f, z * f);
    }
    inline Seal_Vector3 operator-(void) const{
        Seal_Vector3 v3(-x, -y, -z);
        return v3;
    }

    inline Seal_Vector3& operator+=(const Seal_Vector3& o) {
        x += o.x; y += o.y; z += o.z;
        return *this;
    }
    inline Seal_Vector3& operator-=(const Seal_Vector3& o){
        x -= o.x; y -= o.y; z -= o.z;
        return *this;
    }
    inline Seal_Vector3& operator*=(float f){
        x *= f; y *= f; z *= f;
        return *this;
    }

    inline float operator *(const Seal_Vector3& o) const{
        return x * o.x + y * o.y + z * o.z;
    }
    inline Seal_Vector3 operator*(const Seal_Quaternion& q) const {
        Seal_Vector3 u = {q.x, q.y, q.z};
        const Seal_Vector3& v = *this;
        return u * (v * u * 2) + v * (q.w * q.w - u * u) + (cross(u, v) * 2 * q.w);
    }

    inline static Seal_Vector3 cross(const Seal_Vector3& a, const Seal_Vector3& b){
        return {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x};
    }

    inline float& operator[](int index) { return *(&x + index); }
};