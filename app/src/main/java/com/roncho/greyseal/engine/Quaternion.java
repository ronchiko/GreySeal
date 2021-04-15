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
}
