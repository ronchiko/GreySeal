package com.roncho.greyseal.engine;

import java.nio.ByteBuffer;

public class Vector3 {

    public static final int SIZE_IN_BYTES = 12;

    public float x, y, z;

    public Vector3(float x, float y, float z){
        this.x = x;
        this.y = y;
        this.z = z;
    }

    public void writeToBuffer(ByteBuffer buffer){
        buffer.putFloat(x);
        buffer.putFloat(y);
        buffer.putFloat(z);
    }

    public static Vector3 readFromBuffer(ByteBuffer buffer){
        Vector3 v = new Vector3(0, 0, 0);
        v.x = buffer.getFloat();
        v.y = buffer.getFloat();
        v.z = buffer.getFloat();
        return v;
    }

    @Override
    public String toString() {
        return "Vector3{" +
                "x=" + x +
                ", y=" + y +
                ", z=" + z +
                '}';
    }
}
