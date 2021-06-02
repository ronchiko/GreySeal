package com.roncho.greyseal.engine.systems;

import android.util.Log;

import com.roncho.greyseal.engine.Quaternion;
import com.roncho.greyseal.engine.SealEngine;
import com.roncho.greyseal.engine.SealLog;
import com.roncho.greyseal.engine.Vector3;
import com.roncho.greyseal.engine.api.EntityQuery;
import com.roncho.greyseal.engine.api.HasStringValue;
import com.roncho.greyseal.engine.systems.stream.Entity;
import com.roncho.greyseal.engine.systems.stream.EntityStream;

public abstract class SealSystem {

    static EntityStream stream;

    /**
     * Must be implemented for the system to be able to select object to work with
     * @param entity The entity to select
     * @return True if the object should be run by the system
     */
    public abstract boolean selectObject(Entity entity);

    /**
     * Called every frame for each entity that the system should run on
     * @param entity the entity to update
     */
    public abstract void onUpdate(Entity entity);

    /**
     * Called every frame once
     * <b>Override</b>
     */
    public void updateOnce() {}

    /**
     * Called when the system picks up a new entity
     * @param entity The new entity
     */
    public void onNewEntity(Entity entity) {}
    /**
     * Called when the system picks up an entity marked for destruction
     * @param entity The marked entity
     */
    public void onEntityDestroyed(Entity entity) {}

    /**
     *  Called when the system is first registered
     */
    public void onRegister() {}

    public static void create(String path){
        SealEngine.makePreset(path);
    }
    public static void create(HasStringValue path){
        SealEngine.makePreset(path.getStringValue());
    }
    public static void create(String path, Vector3 position){
        SealEngine.makePreset(path, position);
    }
    public static void create(HasStringValue path, Vector3 position){
        SealEngine.makePreset(path.getStringValue(), position);
    }
    public static void create(String path, Vector3 position, Quaternion rotation, Vector3 scale){
        SealEngine.makePreset(path, position, rotation, scale);
    }
    public static void create(HasStringValue path, Vector3 position, Quaternion rotation, Vector3 scale){
        SealEngine.makePreset(path.getStringValue(), position, rotation, scale);
    }

    public static void clone(Entity e){
        SealEngine.cloneInstance(e);
    }
    public static void clone(Entity e, Vector3 pos){
        SealEngine.cloneInstance(e, pos);
    }
    public static void clone(Entity e, Vector3 pos, Quaternion rot, Vector3 scl){
        SealEngine.cloneInstance(e, pos, rot, scl);
    }

    public static void destroy(Entity e){
        destroy(e.uid);
    }
    public static void destroy(short uid){
        Entity e = find(uid);
        if(e == null) {
            SealLog.Error("Failed to find object to remove! " + uid);
        }
        if(e != null) {
            SealEngine.destroy(e);
        }
    }

    public static Entity find(short uid){
        if(stream == null) {
            SealLog.Error("No stream reference on systems");
            return null;
        }
        return stream.getByUid(uid);
    }

    public static void print(String msg){
        Log.d("Seal Game", msg);
    }
    public static void print(HasStringValue msg){
        Log.d("Seal Game", msg.getStringValue());
    }

    public static void modify(Entity e){
        stream.write(e);
    }

    public static Entity searchFor(EntityQuery query){
        if(stream == null) {
            SealLog.Error("No stream reference on systems");
            return null;
        }
        return stream.search(query);
    }
    public static Entity[] searchForAll(EntityQuery query){
        if(stream == null) {
            SealLog.Error("No stream reference on systems");
            return new Entity[0];
        }
        return stream.searchAll(query);
    }
}
