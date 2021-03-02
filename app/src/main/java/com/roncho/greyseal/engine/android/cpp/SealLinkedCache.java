package com.roncho.greyseal.engine.android.cpp;

import java.util.HashMap;

public class SealLinkedCache {
    private static HashMap<String, Integer> textures, meshes, materials;

    static {
        textures = new HashMap<>();
        meshes = new HashMap<>();
        materials = new HashMap<>();
    }

    // These are implemented and called from JNI
    public native static void addTexture(String str, int integer);
    public native static void addMesh(String str, int integer);
    public native static void addMaterial(String str, int integer);
}
