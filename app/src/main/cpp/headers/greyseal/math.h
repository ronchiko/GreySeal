#pragma once

#include <cmath>

#define PI 3.14159f

/**
 * \brief Preforms a bit cast
 * \tparam _Ret The result type
 * \tparam _In The input type
 * \param n the value to cast
 */
template<typename _Ret, typename _In>
constexpr inline _Ret bit_cast(const _In& n) { return *(_Ret*)&n; }

/**
 * Not a number
 */
const float NaN = bit_cast<float>(0x7fe00000);
const float Deg2Rad = PI / 180.f, Rad2Deg = 180.f / PI;
/**
 * \brief Quake 2 inverse sqrt algorithm (only for n > 0)
 * \param n the number to compute
 * \return 1 / sqrt(n) for n > 0 else returns nonsense
 */
inline float Seal_InvSqrt(float n){
    long i;
    const float x = n * 0.5f, threeHalves = 1.5f;

    i = *(long*)&n;
    i = 0x5f375a86 - (i >> 1);
    n = *(float*)&i;

    return n * (threeHalves - n * n * x);
}

/**
 * \brief Safe Quake 2 inverse sqrt algorithm (if n <= 0 returns NaN)
 * \param n the number to compute
 * \return 1 / sqrt(n)
 */
inline float Seal_InvSqrtSafe(float n){
    if(n <= 0) return NaN;
    return Seal_InvSqrt(n);
}
