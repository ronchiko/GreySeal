package com.roncho.greyseal.engine.systems;

import com.roncho.greyseal.engine.SealEngine;
import com.roncho.greyseal.engine.SealLog;
import com.roncho.greyseal.engine.Vector3;
import com.roncho.greyseal.engine.systems.stream.Entity;

public abstract class SealSystem {

    private final static short NEW_FLAG = 0x1;

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

    public static void instantiate(String path){
        SealEngine.makePreset(path);
    }
    public static void instantiate(String path, Vector3 position){
        SealEngine.makePreset(path, position);
    }

    public static void destroy(Entity e){
        SealEngine.destroy(e);
    }
    public void registerSignal(String name, SealSignalHandler signal){
        SealSystemManager.registerSignal(name, signal);
    }
    public void callSignal(String name, Entity e, Object... params){
        SealSystemManager.invoke(name, e, params);
    }
}
