package com.roncho.greyseal.engine.physics;

import android.annotation.SuppressLint;

import com.roncho.greyseal.engine.SealLog;
import com.roncho.greyseal.engine.Time;
import com.roncho.greyseal.engine.Tuple;
import com.roncho.greyseal.engine.systems.SealStaticSystem;
import com.roncho.greyseal.engine.systems.SealSystemManager;
import com.roncho.greyseal.engine.systems.stream.Entity;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;
import java.util.logging.Logger;

public class CollisionSystem extends SealStaticSystem {

    private static CollisionSystem instance;

    private final List<Tuple<Short, ICollisionReceiver>> receivers = new ArrayList<>();
    private final Set<Short> toRemove = new HashSet<>();

    public static void addInstance(Entity e, ICollisionReceiver ipc) {
        if (instance != null) {
            instance.receivers.add(new Tuple<>(e.uid, ipc));
        }
    }

    public static void removeInstance(Entity e){
        if(instance != null)
            instance.toRemove.add(e.uid);
    }
    public static void removeInstance(short uid){
        if(instance != null)
            instance.toRemove.add(uid);
    }

    @SuppressLint("DefaultLocale")
    @Override
    public void updateOnce(){
        int nSize = receivers.size();
        // Loop on all objects registered for collision
        for(int i = 0; i < nSize; i++){
            Short uid = receivers.get(i).getFirst();
            ICollisionReceiver o1 = receivers.get(i).getSecond();
            // If the entity is destroyed, then don't invoke collisions on it
            Entity e = find(uid);

            // Check if the object should be removed, or it has already been removed
            if(e == null || toRemove.contains(uid) || e.queued()){
                receivers.remove(receivers.get(i));
                nSize--;
                i--;
                continue;
            }
            // Skip uninitialized bodies
            if(o1.getAABB() == null || o1.getPosition() == null) {
                SealLog.Error(String.format("[Entity %d] Missing collider details", uid));
                continue;
            }

            // Scan for collision, we start scanning from j = i + 1 and beyond because we
            // know that all collisions before i where already found and we i is the current collider
            for(int j = i + 1; j < nSize; j++){
                // If the entity is destroyed, then don't invoke collisions on it
                short uid2 = receivers.get(j).getFirst();
                Entity e2 = find(uid2);

                if(e2 == null || e2.queued())
                {
                    receivers.remove(receivers.get(j));
                    nSize--;
                    j--;
                    continue;
                }

                ICollisionReceiver o2 = receivers.get(j).getSecond();

                // Skip uninitialized bodies
                if(o2.getAABB() == null || o2.getPosition() == null) {
                    SealLog.Error(String.format("[Entity %d] Missing collider details", uid2));
                    continue;
                }

                // Check if a collision happened
                if(o1.getAABB().collides(o2.getAABB(), o1.getPosition(), o2.getPosition())){
                    // Invoke collision handlers
                    o1.onCollision(e2);
                    o2.onCollision(e);
                }
            }
        }
        toRemove.clear();
    }

    public static void enable(){
        instance = new CollisionSystem();
        SealSystemManager.addSystem(instance);
    }
}
