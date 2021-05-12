package com.roncho.greyseal.engine;

import java.nio.ByteBuffer;

public final class Vector3 {

    public static final int SIZE_IN_BYTES = 12;

    public float x, y, z;

    public Vector3(float x, float y, float z){
        this.x = x;
        this.y = y;
        this.z = z;
    }
    public Vector3(Vector3 v3){
        this(v3.x, v3.y, v3.z);
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

    public Vector3 add(Vector3 v){
        return new Vector3(x + v.x, y + v.y, z + v.z);
    }

    public Vector3 sub(Vector3 v){
        return new Vector3(x - v.x, y - v.y, z - v.z);
    }

    public Vector3 scale(float t){ return new Vector3(x * t, y * t, z * t);}
    public Vector3 scale(Vector3 t){ return new Vector3(x * t.x, y * t.y, z * t.z);}

    @Override
    public String toString() {
        return "Vector3{" +
                "x=" + x +
                ", y=" + y +
                ", z=" + z +
                '}';
    }
}
