package com.roncho.greyseal.engine.systems;

import com.roncho.greyseal.engine.systems.stream.Entity;

/**
 * <p> A system that should run in the background, only implements update once </p>
 */
public abstract class SealStaticSystem extends SealSystem {
    @Override
    public final boolean selectObject(Entity entity) {
        return false;
    }

    @Override
    public final void onUpdate(Entity entity) {

    }

    @Override
    public final void onEntityDestroyed(Entity entity) {
        super.onEntityDestroyed(entity);
    }

    @Override
    public final void onNewEntity(Entity entity) {
        super.onNewEntity(entity);
    }

    @Override
    public abstract void updateOnce();
}
