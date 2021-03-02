package com.roncho.greyseal.engine.systems;

import com.roncho.greyseal.engine.systems.stream.SealEntity;

public abstract class SealSystem {

    private final static short NEW_FLAG = 0x1;

    /**
     * Must be implemented for the system to be able to select object to work with
     * @param entity The entity to select
     * @return True if the object should be run by the system
     */
    public abstract boolean selectObject(SealEntity entity);

    /**
     * Called every frame for each entity that the system should run on
     * @param entity the entity to update
     */
    public abstract void onUpdate(SealEntity entity);

    /**
     * Called every frame once
     * <b>Override</b>
     */
    public void updateOnce() {}

    /**
     * Called when the system picks up a new entity
     * @param entity The new entity
     */
    public void onNewEntity(SealEntity entity) {}
}
