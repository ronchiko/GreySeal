package com.roncho.greyseal.engine.physics;

import com.roncho.greyseal.engine.Vector3;
import com.roncho.greyseal.engine.systems.stream.Entity;

/**
 * Interface to implement components that interact with the builtin CollisionSystem.
 */
public interface ICollisionReceiver {
    /** The position of the object */
    Vector3 getPosition();
    /** The AABB of the object (Area Collider) */
    AABB getAABB();
    /** Callback for when a collision hits */
    void onCollision(Entity other);
}
