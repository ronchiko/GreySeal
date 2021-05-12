package com.roncho.greyseal.engine.physics;

import com.roncho.greyseal.engine.Time;
import com.roncho.greyseal.engine.Tuple;
import com.roncho.greyseal.engine.systems.SealStaticSystem;
import com.roncho.greyseal.engine.systems.SealSystemManager;
import com.roncho.greyseal.engine.systems.stream.Entity;

import java.util.ArrayList;
import java.util.List;

public class CollisionSystem extends SealStaticSystem {

    private static CollisionSystem instance;

    private final List<Tuple<Short, ICollisionReceiver>> receivers = new ArrayList<>();

    public static void addInstance(Entity e, ICollisionReceiver ipc){
        if(instance != null)
            instance.receivers.add(new Tuple<>(e.uid, ipc));
    }

    public static void removeInstance(Entity e){
        if(instance != null)
            instance.receivers.remove(e.uid);
    }

    @Override
    public void updateOnce(){
        // Loop on all objects registered for collision
        for(int i = 0; i < receivers.size(); i++){
            for(int j = i + 1; j < receivers.size(); j++){
                ICollisionReceiver o1 = receivers.get(i).getSecond();
                ICollisionReceiver o2 = receivers.get(j).getSecond();

                // Check if a collision happened
                if(o1.getAABB().collides(o2.getAABB(), o1.getPosition(), o2.getPosition())){
                    // Invoke collision handlers
                    o1.onCollision(receivers.get(j).getFirst());
                    o2.onCollision(receivers.get(i).getFirst());
                }
            }
        }
    }

    public static void enable(){
        instance = new CollisionSystem();
        SealSystemManager.addSystem(instance);
    }
}
