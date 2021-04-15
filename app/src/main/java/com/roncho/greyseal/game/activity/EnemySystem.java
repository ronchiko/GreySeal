package com.roncho.greyseal.game.activity;

import com.roncho.greyseal.engine.Time;
import com.roncho.greyseal.engine.systems.SealSystem;
import com.roncho.greyseal.engine.systems.stream.Entity;
import com.roncho.greyseal.engine.systems.stream.UniqueForEntity;

public class EnemySystem extends SealSystem {

    private static EnemySystem instance;

    private class Enemy {
        private float m_Health;
        private float m_Speed;

        public Enemy(Entity e){
            m_Speed = 5;
            m_Health = 1;

            m_EnemyStats.add(e, this);
        }

        public void move(Entity entity){
            entity.position.z += Time.deltaTime() * m_Speed;
            if(entity.position.z >= 0){
                destroy(entity);
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
    public void onUpdate(Entity entity) {

        Enemy e = m_EnemyStats.get(entity);
        if(e == null){
            e = new Enemy(entity);
        }
        e.move(entity);
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
