package com.roncho.greyseal.game.activity;

import com.roncho.greyseal.engine.SealLog;
import com.roncho.greyseal.engine.SealMath;
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

        private final short uid;
        private final AABB m_AABB;
        private final float m_Speed;
        // To create a slide in effect for the enemy ships, we keep track of 2 position
        // m_Position is the actual position of the ship, and m_PositionOnGrid is the
        // position of the spaceship on the track, and m_Position moves towards it until
        // they merge
        private final Vector3 m_Position, m_PositionOnGrid;

        public Enemy(Entity e){
            uid = e.uid;
            m_PositionOnGrid = new Vector3(e.position);
            m_AABB = AABB.auto(e.mesh, e.scale);

            float l_ScatterX = SealMath.random(-40, 40);
            m_Position = new Vector3(l_ScatterX, 10, m_PositionOnGrid.z);
            m_Speed = 10;

            m_EnemyStats.add(e, this);
            CollisionSystem.addInstance(e, this);
        }

        public void move(Entity entity){
            // Move imaginary position
            m_PositionOnGrid.z += m_Speed * Time.deltaTime();

            // Linearly interpolate the real position to the imaginary one
            m_Position.z = m_PositionOnGrid.z;
            m_Position.x = SealMath.lerp(m_Position.x, m_PositionOnGrid.x, Time.deltaTime());
            if(m_Position.y > -3 && SealMath.abs(m_Position.x - m_PositionOnGrid.x) <= 5){
                m_Position.y = SealMath.lerp(m_Position.y, m_PositionOnGrid.y, Time.deltaTime());
            }

            // If the object passed the horizon, destroy it and end the game.
            if(m_Position.z >= 0){
                destroy(uid);
                GameManagerSystem.endGame();
            }

            entity.position = m_Position;
        }

        @Override
        public Vector3 getPosition() {
            return m_Position;
        }

        @Override
        public AABB getAABB() {
            return m_AABB;
        }

        @Override
        public void onCollision(Entity other) {
            if(other.matches(GameFlags.BULLET.getMask())) {
                GameManagerSystem.addScore(1);
                destroy(uid);
            }
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
        if(e == null){
            e = new Enemy(entity);
        }
        e.move(entity);
    }

}
