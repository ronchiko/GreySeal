package com.roncho.greyseal.engine.physics;

import com.roncho.greyseal.engine.Vector3;

import java.util.HashMap;
import java.util.Objects;

/**
 * Area Collider used by the CollisionSystem to determine if 2 entities collide with each other.
 * This should be a 3 dimensional box that contains the object, and anything that touches it will
 * be considered a collision.
 */
public final class AABB {
    private final static HashMap<Integer, AABB> meshPrecompiledAABB;
    static {
        meshPrecompiledAABB = new HashMap<>();
    }

    private Vector3 start;
    private Vector3 sizes;

    private Vector3 end;

    public AABB(Vector3 origin, Vector3 size){
        start = new Vector3(origin);
        sizes = new Vector3(size);
        end = start.add(sizes);
    }
    public AABB(AABB o){
        start = new Vector3(o.start);
        sizes = new Vector3(o.sizes);
        end = new Vector3(o.end);
    }
    public AABB(AABB o, Vector3 scale){
        start = new Vector3(o.start.scale(scale));
        sizes = new Vector3(o.sizes.scale(scale));
        end = new Vector3(o.end);
    }

    public boolean collides(AABB other, Vector3 position, Vector3 oPosition){
        return  !(other.start.x + oPosition.x > end.x + position.x || other.end.x + oPosition.x < start.x + position.x) ||
                !(other.start.y + oPosition.y > end.y + position.y || other.end.y + oPosition.y < start.y + position.y) ||
                !(other.start.z + oPosition.z > end.z + position.z || other.end.z + oPosition.z < start.z + position.z);
    }
    public boolean contains(Vector3 point, Vector3 position){
        return  start.x + position.x <= point.x && point.x <= end.x + position.x &&
                start.y + position.y <= point.y && point.y <= end.y + position.y &&
                start.z + position.z <= point.z && point.z <= end.z + position.z;
    }

    public void setOrigin(Vector3 origin){
        start = new Vector3(origin);
        end = start.add(sizes);
    }
    public void setSize(Vector3 size){
        sizes = new Vector3(size);
        end = start.add(sizes);
    }

    public static AABB auto(int mesh){
        if(meshPrecompiledAABB.containsKey(mesh)){
            return new AABB(Objects.requireNonNull(meshPrecompiledAABB.get(mesh)));
        }
        return null;
    }
    public static AABB auto(int mesh, Vector3 scale){
        if(meshPrecompiledAABB.containsKey(mesh)){
            return new AABB(Objects.requireNonNull(meshPrecompiledAABB.get(mesh)), scale);
        }
        return null;
    }

    private static void registerMeshAABB(int meshId, float x, float y, float z, float w, float h, float d){
        meshPrecompiledAABB.put(meshId, new AABB(new Vector3(x, y, z), new Vector3(w, h, d)));
    }
}
