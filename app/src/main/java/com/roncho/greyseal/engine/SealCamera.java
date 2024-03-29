package com.roncho.greyseal.engine;

import com.roncho.greyseal.engine.systems.stream.Entity;

public final class SealCamera {

    public static Entity current;

    public Vector3 position() { return current.position; }
    public void position(Vector3 v3) { current.position = v3; }
    public Quaternion rotation() { return current.rotation; }
    public void rotation(Quaternion qt) { current.rotation = qt; }
}
