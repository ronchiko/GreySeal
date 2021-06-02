package com.roncho.greyseal.game.activity;

import com.roncho.greyseal.engine.SealLog;
import com.roncho.greyseal.engine.Time;
import com.roncho.greyseal.engine.Vector3;
import com.roncho.greyseal.engine.physics.AABB;
import com.roncho.greyseal.engine.physics.CollisionSystem;
import com.roncho.greyseal.engine.physics.ICollisionReceiver;
import com.roncho.greyseal.engine.systems.SealSystem;
import com.roncho.greyseal.engine.systems.stream.Entity;
import com.roncho.greyseal.engine.systems.stream.UniqueForEntity;

public class ProjectileSystem extends SealSystem {

    public static class ProjectileComponent implements ICollisionReceiver {

        private final short m_Uid;
        private Vector3 position;
        private final AABB aabb;

        public ProjectileComponent(Entity e){
            aabb = AABB.auto(e.mesh, e.scale);
            if(aabb != null) {
                aabb.rotate90Y();
            }
            position = e.position;

            CollisionSystem.addInstance(e, this);
            m_Uid = e.uid;
        }

        @Override
        public Vector3 getPosition() {
            return position;
        }

        @Override
        public AABB getAABB() {
            return aabb;
        }

        @Override
        public void onCollision(Entity other) {
            if(other.matches(GameFlags.ENEMY.getMask())) {
                destroy(m_Uid);
                CollisionSystem.removeInstance(m_Uid);
            }
        }

        public void update(Entity e){
            e.position.z -= 5 * Time.deltaTime();
            this.position = e.position;
            if(e.position.z <= -50){
                destroy(e);
            }
        }
    }

    private final UniqueForEntity<ProjectileComponent> projectiles = new UniqueForEntity<>();

    @Override
    public boolean selectObject(Entity entity) {
        return entity.matches(GameFlags.BULLET);
    }

    @Override
    public void onNewEntity(Entity entity) {
        projectiles.add(entity, new ProjectileComponent(entity));
    }

    @Override
    public void onEntityDestroyed(Entity entity) {
        CollisionSystem.removeInstance(entity);
        projectiles.remove(entity);
    }

    @Override
    public void onUpdate(Entity entity) {
        ProjectileComponent projectile = projectiles.get(entity);
        if(projectile != null)
            projectile.update(entity);
    }
}
