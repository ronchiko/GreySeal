package com.roncho.greyseal.game.activity;

import com.roncho.greyseal.engine.Time;
import com.roncho.greyseal.engine.Vector3;
import com.roncho.greyseal.engine.physics.AABB;
import com.roncho.greyseal.engine.physics.CollisionSystem;
import com.roncho.greyseal.engine.physics.ICollisionReceiver;
import com.roncho.greyseal.engine.systems.SealSystem;
import com.roncho.greyseal.engine.systems.stream.Entity;
import com.roncho.greyseal.engine.systems.stream.UniqueForEntity;

public class EnemySystem extends SealSystem {

    private static EnemySystem instance;

    private class Enemy implements ICollisionReceiver {
        private float m_Health;
        private float m_Speed;
        private AABB aabb;
        private Vector3 position;

        public Enemy(Entity e){
            m_Speed = 5;
            m_Health = 1;

            aabb = AABB.auto(e.mesh, e.scale);
            position = e.position;

            CollisionSystem.addInstance(e, this);
            m_EnemyStats.add(e, this);
        }

        public void move(Entity entity){
            position = entity.position;
            entity.position.z += Time.deltaTime() * m_Speed;
            if(entity.position.z >= 0){
                destroy(entity);
            }
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
        public void onCollision(int otherIndex) {

        }
    }

    private final UniqueForEntity<Enemy> m_EnemyStats;

    public EnemySystem(){
        instance = this;
        m_EnemyStats = new UniqueForEntity<>();
    }

    @Override
    public boolean selectObject(Entity entity) {
        return (entity.userFlags & GameFlags.ENEMY.getMask()) != 0;
    }

    @Override
    public void onNewEntity(Entity entity) {
        new Enemy(entity);
    }

    @Override
    public void onUpdate(Entity entity) {
        Enemy e = m_EnemyStats.get(entity);
        e.move(entity);
    }

    @Override
    public void onEntityDestroyed(Entity entity) {
        CollisionSystem.removeInstance(entity);
    }

    public static void damageEnemy(Entity e, float dmg){
        Enemy enemy;
        if(instance != null && (enemy = instance.m_EnemyStats.get(e)) != null){
            enemy.m_Health -= dmg;
            if(enemy.m_Health <= 0) {
                destroy(e);
            }
        }
    }
}
