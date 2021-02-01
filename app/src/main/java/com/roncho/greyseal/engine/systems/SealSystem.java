package com.roncho.greyseal.engine.systems;

import com.roncho.greyseal.engine.systems.stream.SealEntity;

public abstract class SealSystem {

    private final static int NEW_FLAG = 1 << 30, DESTROYED_FLAG = 1 << 31, ONLY_USER_FLAGS = 0x0FFFFFFF;

    /**
     * Must be implemented for the system to be able to select object to work with
     * @param entity
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
}
