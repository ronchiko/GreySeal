package com.roncho.greyseal.engine;

import java.nio.ByteBuffer;

public final class Quaternion {

    public static final int SIZE_IN_BYTE = 16;

    public float w, x, y, z;

    public Quaternion(float w, float x, float y, float z){
        this.w = w;
        this.x = x;
        this.y = y;
        this.z = z;
    }
    public Quaternion(final Quaternion q){
        this(q.w, q.x, q.y, q.z);
    }

    public void writeToBuffer(ByteBuffer buffer){
        buffer.putFloat(x);
        buffer.putFloat(y);
        buffer.putFloat(z);
        buffer.putFloat(w);
    }

    public static Quaternion readFromBuffer(ByteBuffer buffer){
        Quaternion q = new Quaternion(1, 0, 0, 0);
        q.x = buffer.getFloat();
        q.y = buffer.getFloat();
        q.z = buffer.getFloat();
        q.w = buffer.getFloat();
        return q;
    }

    public void set(float angle, float xx, float yy, float zz){
        float invSqrt = SealMath.invSqrt(xx * xx + yy * yy + zz * zz);
        if(Float.isNaN(invSqrt)) return;

        xx *= invSqrt; yy *= invSqrt; zz *= invSqrt;

        float sin = SealMath.sin(angle * 0.5f * SealMath.Deg2Rad);
        x = xx * sin; y = yy * sin; z = zz * sin;
        w = SealMath.cos(angle * 0.5f * SealMath.Deg2Rad);

        normalize();
    }

    public void normalize(){
        float invSqrt = SealMath.invSqrt(x * x + y * y + z * z + w * w);
        if(Float.isNaN(invSqrt)) return;

        x *= invSqrt; y *= invSqrt; z *= invSqrt; w *= invSqrt;
    }

    public Quaternion mul(Quaternion o){
        return new Quaternion(w * o.w - x * o.x - y * o.y - z * o.z,
                w * o.x + x * o.w + y * o.z - z * o.y,
                w * o.y - x * o.z + y * o.w + z * o.x,
                w * o.z + x * o.y - y * o.x + z * o.w);
    }

    public static Quaternion euler(float x, float y, float z){
         x *= SealMath.Deg2Rad;
         y *= SealMath.Deg2Rad;
         z *= SealMath.Deg2Rad;

         float cy = SealMath.cos(y * 0.5f);
         float sy = SealMath.sin(y * 0.5f);
         float cx = SealMath.cos(x * 0.5f);
         float sx = SealMath.sin(x * 0.5f);
         float cz = SealMath.cos(z * 0.5f);
         float sz = SealMath.sin(z * 0.5f);

         return new Quaternion(cx * cy * cz + sx * sy * sz,
                 sx * cy * cz - cx * sy * sz,
                 cx * sy * cz + sx * cy * sz,
                 cx * cy * sz - sx * sy * cz);
    }

    public static Quaternion slerp(Quaternion a, Quaternion b, float t){
        float cosHalfTheta = a.w * b.w + a.x * b.x + a.y * b.y + a.z * b.z;

        // If a == b or a == -b, then the angle is 0
        if(SealMath.abs(cosHalfTheta) >= 1.0f){
            return new Quaternion(a);
        }

        float sinHalfTheta = SealMath.sqrt(1 - cosHalfTheta * cosHalfTheta);
        // if theta == 180 then the result is not fully defined
        // So rotating around any axis normal to a or b is possible
        if(SealMath.abs(sinHalfTheta) < 0.001f){
            return new Quaternion((a.w + b.w) * 0.5f, (a.x + b.x) * 0.5f,
                    (a.y + b.y) * 0.5f, (a.z + b.z) * 0.5f);
        }

        // Compute the ratios from a and b
        float halfTheta = SealMath.acos(cosHalfTheta);
        float ratioA = SealMath.sin((1 - t) * halfTheta) / sinHalfTheta;
        float ratioB = SealMath.sin(t * halfTheta) / sinHalfTheta;

        return new Quaternion(a.w * ratioA + b.w * ratioB,
                a.x * ratioA + b.x * ratioB,
                a.y * ratioA + b.y * ratioB,
                a.z * ratioA + b.z * ratioB);
    }
}
