package com.roncho.greyseal.engine;

import java.nio.ByteBuffer;

public class Quaternion {
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
}
