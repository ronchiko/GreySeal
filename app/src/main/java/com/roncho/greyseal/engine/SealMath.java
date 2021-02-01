package com.roncho.greyseal.engine;

public class SealMath {

    public static final float Pi = 3.14159f;
    public static final float Deg2Rad = Pi / 180f;
    public static final float Rad2Deg = 180f / Pi;

    public static int clamp(int x, int min, int max) {
        return x < min ? min : Math.min(x, max);
    }
    public static float clamp(float x, float min, float max){
        return x < min ? min : Math.min(x, max);
    }
    public static float clamp01(float x){
        return x < 0 ? 0 : Math.min(x, 1);
    }

    public static float sqrt(float x){
        return (float)Math.sqrt(x);
    }

    public static float invSqrt(float x){
        if(x <= 0) return Float.NaN;

        float xHalf = 0.5f * x;
        int i = Float.floatToIntBits(x);
        i = 0x5f3759df - (i >> 1);
        x = Float.intBitsToFloat(i);
        x *= (1.5f - xHalf * x * x);
        return x;
    }

    public static float sin(float x){
        return (float)Math.sin(x);
    }
    public static float cos(float x){
        return (float)Math.cos(x);
    }
}
