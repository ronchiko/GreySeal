#pragma once

#include <cmath>

/**
 * \brief Multiplies 2 matrices with each other
 * \param out The output matrix
 * \param a left side matrix
 * \param b right side matrix
 */
inline void Seal_MatrixMul(float* out, float* a, float* b){
    float result[16];
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            result[4 * i + j] = a[j] * b[4 * i] + a[4 + j] * b[4 * i + 1] + a[8 + j] * b[4 * i + 2] + a[12 + j] * b[4 * i + 3];
    for(int i = 0; i < 16; i++)
        out[i] = result[i];
}

/**
 * \brief Sets a matrix to be an identity matrix
 * \param matrix The matrix to set as an identity matrix
 */
inline void Seal_IdentityMatrix(float* matrix){
    for(int i = 0; i < 16; i++)
        matrix[i] = i % 4 == i / 4 ? 1.0f : 0.0;
}

/**
 * \brief Translates (Moves) a matrix by a given amount in each direction
 * \param matrix matrix to move
 * \param x Move in x axis
 * \param y Move in y axis
 * \param z Move in z axis
 */
inline void Seal_TranslateMatrix(float* matrix, float x, float y, float z){
    float temp[16];
    Seal_IdentityMatrix(temp);
    temp[12] = x;
    temp[13] = y;
    temp[14] = z;
    Seal_MatrixMul(matrix, temp, matrix);
}

inline void Seal_ScaleMatrix(float* matrix , float x, float y, float z){
    float temp[16];
    Seal_IdentityMatrix(temp);
    temp[0] = x;
    temp[5] = y;
    temp[10] = z;
    Seal_MatrixMul(matrix, temp, matrix);
}

inline void Seal_MatrixFrustum(float* matrix, float left, float right, float bottom, float top, float zNear, float zFar){
    float temp = 2.0 * zNear, xDistance = right - left, yDistance = top - bottom, zDistance = zFar - zNear;
    Seal_IdentityMatrix(matrix);
    matrix[0] = temp / xDistance; matrix[5] = temp / yDistance;
    matrix[8] = (right + left) / xDistance;
    matrix[9] = (top + bottom) / yDistance;
    matrix[10] = (-zFar - zNear) / zDistance;
    matrix[11] = -1.0f;
    matrix[14] = (-temp * zFar) / zDistance;
    matrix[15] = 0.0f;
}

inline void Seal_CopyVector(float* _Dst, size_t _DstOff, float* _Src, size_t _SrcOff){
    for(size_t i = 0; i < 3; i++)
        _Dst[_DstOff + i] = _Src[_SrcOff + i];
}

inline void Seal_MatrixLookAt(float* matrix, const Seal_Vector3& zAxis, const Seal_Vector3& eye){
    matrix[3] = matrix[7] = matrix[11] = 0; matrix[15] = 1;

    Seal_Vector3 xAxis = Seal_Vector3::cross({0, -1, 0}, zAxis);
    Seal_Vector3 yAxis = Seal_Vector3::cross(xAxis, zAxis);

    matrix[0] = xAxis.x; matrix[1] = yAxis.x; matrix[2]  = zAxis.x;
    matrix[4] = xAxis.y; matrix[5] = yAxis.y; matrix[6]  = zAxis.y;
    matrix[8] = xAxis.z; matrix[9] = yAxis.z; matrix[10] = zAxis.z;

    matrix[12] = (-eye * xAxis);
    matrix[13] = (-eye * yAxis);
    matrix[14] = (-eye * zAxis);
}

inline void Seal_MatrixPerspective(float* matrix, float fov, float ratio, float zNear, float zFar){
    float yMax = zNear * tanf(fov * M_PI / 360.f), xMax = yMax * ratio;
    Seal_MatrixFrustum(matrix, -xMax, xMax, -yMax, yMax, zNear, zFar);
}

