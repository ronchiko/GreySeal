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
        end = start.add(sizes);
    }
    public AABB(AABB o, Vector3 scale){
        start = new Vector3(o.start.scale(scale));
        sizes = new Vector3(o.sizes.scale(scale));
        end = start.add(sizes);
    }

    public boolean collides(AABB other, Vector3 position, Vector3 oPosition){
        float ex = end.x + position.x, sx = start.x + position.x, osx = other.start.x + oPosition.x, oex = other.end.x + oPosition.x;
        float ey = end.y + position.y, sy = start.y + position.y, osy = other.start.y + oPosition.y, oey = other.end.y + oPosition.y;
        float ez = end.z + position.z, sz = start.z + position.z, osz = other.start.z + oPosition.z, oez = other.end.z + oPosition.z;
        return oex >= sx && ex >= osx && oey >= sy && ey >= osy && oez >= sz && ez >= osz;
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

    public void rotate90Y(){
        float tsx = start.x, twx = sizes.x;
        start.x = start.z; sizes.x = sizes.z;
        start.z = tsx; sizes.z = twx;
        end = start.add(sizes);
    }
}
