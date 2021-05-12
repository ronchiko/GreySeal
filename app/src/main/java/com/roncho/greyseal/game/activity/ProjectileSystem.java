package com.roncho.greyseal.game.activity;

import com.roncho.greyseal.engine.Vector3;
import com.roncho.greyseal.engine.physics.AABB;
import com.roncho.greyseal.engine.physics.ICollisionReceiver;
import com.roncho.greyseal.engine.systems.SealSystem;
import com.roncho.greyseal.engine.systems.stream.Entity;
import com.roncho.greyseal.engine.systems.stream.UniqueForEntity;

public class ProjectileSystem extends SealSystem {

    public static class ProjectileComponent implements ICollisionReceiver {

        private Vector3 position;
        private AABB aabb;

        @Override
        public Vector3 getPosition() {
            return null;
        }

        @Override
        public AABB getAABB() {
            return null;
        }

        @Override
        public void onCollision(int otherIndex) {

        }
    }

    private final UniqueForEntity<ProjectileComponent> projectiles = new UniqueForEntity<>();

    @Override
    public boolean selectObject(Entity entity) {
        return (entity.userFlags & 0x4) != 0;
    }

    @Override
    public void onNewEntity(Entity entity) {
        super.onNewEntity(entity);

    }

    @Override
    public void onUpdate(Entity entity) {

    }
}
