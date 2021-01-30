#pragma once

#include "greyseal/math.h"

#include "sealog.h"

union Seal_Quaternion {
    struct {
        float x, y, z, w;
    };
    float raw[4];

    Seal_Quaternion(float w, float x, float y, float z) : w(w), x(x), y(y), z(z) {}
    Seal_Quaternion(const Seal_Quaternion& q) : w(q.w), x(q.x), y(q.y), z(q.z) {}
    Seal_Quaternion() : w(0), x(0), y(0), z(0) {}

    /**
     * \brief Normalizes this quaternion
     */
    inline Seal_Quaternion& normalize() {
        float inv = Seal_InvSqrtSafe(x * x + y * y + w * w + z * z);
        if(inv != NaN){
            x *= inv; y *= inv; z *= inv; w *= inv;
        }
        return *this;
    }
    /**
     * \brief returns the conjugate of this quaternion
     */
    inline Seal_Quaternion conjugate() const{
        return {w, -x, -y, -z};
    }

    /**
     * \brief add 2 quaternions
     * \param a
     */
    inline Seal_Quaternion operator+(const Seal_Quaternion& a) const {
        return Seal_Quaternion(a.w + w, a.x + x, y + a.y, z + a.z);
    }
    /**
     * \brief hamilton product
     * \param o
     */
    inline Seal_Quaternion operator*(const Seal_Quaternion& o) const {
        return Seal_Quaternion(w * o.w - x * o.x - y * o.y - z * o.z,
                          w * o.x + x * o.w + y * o.z - z * o.y,
                          w * o.y - x * o.z + y * o.w + z * o.x,
                          w * o.z + x * o.y - y * o.x + z * o.w);
    }
    /**
     * \brief scale quaternion
     * \param n
     * */
    inline Seal_Quaternion operator*(float n) const{
        return Seal_Quaternion(w * n, x * n, y * n, z * n);
    }
    /**
     * \brief put the quaternion into a matrix
     * \param matrix
     */
    inline void toMatrix(float* matrix) const {
        matrix[3] = matrix[7] = matrix[11] = matrix[12] = matrix[13] = matrix[14] = 0;
        matrix[15] = 1;
        matrix[0] = 2 * (w * w + x * x) - 1;
        matrix[1] = 2 * x * y - 2 * w * z;
        matrix[2] = 2 * x * z + 2 * w * y;
        matrix[4] = 2 * x * y + 2 * w * z;
        matrix[5] = 2 * (w * w + y * y) - 1;
        matrix[6] = 2 * y * z - 2 * x * w;
        matrix[8] = 2 * x * z - 2 * w * y;
        matrix[9] = 2 * y * z + 2 * x * w;
        matrix[10] = 2 * (w * w + z * z) - 1;
    }
    /**
     * \brief create a quaternion for an angle axis
     */
    static Seal_Quaternion fromAngleAxis(float angle, float x, float y, float z){
        float invSqrt = Seal_InvSqrtSafe(x * x + y * y + z * z);
        // No rotations
        if(invSqrt == NaN) return Seal_Quaternion(1, 0, 0, 0);

        x *= invSqrt; y *= invSqrt; z *= invSqrt;
        float sinA = sinf(Deg2Rad * angle * .5f);

        return Seal_Quaternion(cosf(Deg2Rad * angle * .5f), x * sinA, y * sinA, z * sinA).normalize();
    }

    void setRotation(float angle, float xx, float yy, float zz) {
        float invSqrt = Seal_InvSqrtSafe(xx * xx + yy * yy + zz * zz);

        // If invSqrt == NaN the the angle magnitude is 0 which means is no rotations
        if(invSqrt == NaN) return;
        xx *= invSqrt; yy *= invSqrt; zz *= invSqrt;

        float sinA = sinf(Deg2Rad * angle * .5f);

        x = xx * sinA; y = yy * sinA; z = zz * sinA;
        w = cosf(angle * Deg2Rad * 0.5f);
        normalize();
    }
};