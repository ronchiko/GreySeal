package com.roncho.greyseal.engine.android.cpp;

import com.roncho.greyseal.engine.physics.AABB;

import java.util.HashMap;

public final class SealLinkedCache {
    private final static HashMap<String, Integer> textures, meshes, materials;

    static {
        textures = new HashMap<>();
        meshes = new HashMap<>();
        materials = new HashMap<>();
    }

    // These are implemented and called from JNI
    public static void addTexture(String str, int integer){
        textures.put(str, integer);
    }
    private native static void addMesh(String str, int integer);
    private native static void addMaterial(String str, int integer);

    public static int getTexture(String name) {
        if(textures.containsKey(name))
            return (int)textures.get(name);
        return 0;
    }

    public static int getMaterial(String vertex, String frag) {
        String name = String.format("%s;%s;", vertex, frag);
        if(materials.containsKey(name))
            return (int)materials.get(name);
        return -1;
    }

    public static int getMesh(String name) {
        if(meshes.containsKey(name))
            return (int)meshes.get(name);
        return -1;
    }


}
