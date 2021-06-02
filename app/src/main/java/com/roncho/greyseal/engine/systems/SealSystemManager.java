package com.roncho.greyseal.engine.systems;

import androidx.annotation.NonNull;

import com.roncho.greyseal.engine.SealCamera;
import com.roncho.greyseal.engine.systems.instructions.SealCallType;
import com.roncho.greyseal.engine.systems.instructions.SealCallsList;
import com.roncho.greyseal.engine.systems.stream.SealEngineFlags;
import com.roncho.greyseal.engine.systems.stream.Entity;
import com.roncho.greyseal.engine.systems.stream.EntityStream;

import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Objects;

public final class SealSystemManager {

    private static SealSystemManager instance;

    private final SealCallsList engineCalls;
    private final ArrayList<SealSystem> systems;


    public static void newManager(){
        instance = new SealSystemManager();
    }

    public SealSystemManager(){
        systems = new ArrayList<>();
        engineCalls = new SealCallsList();
    }

    public static void addSystem(SealSystem system){
        instance.systems.add(system);
        system.onRegister();
    }
    public static void queue(SealCallType type, ByteBuffer bb){
        instance.engineCalls.queueCall(type, bb);
    }
    public static void runSystems(EntityStream stream){
        instance.engineCalls.reset();
        SealSystem.stream = stream;
        SealCamera.current = stream.next();

        for (SealSystem system : instance.systems) system.updateOnce();

        while(stream.hasNext()) {
            @NonNull Entity entity = stream.next();

            for (SealSystem system : instance.systems) {
                // Do a system select here
                if(system.selectObject(entity)){
                    if(entity.check(SealEngineFlags.DESTROYED)) {
                        // If the object is already destroyed
                        system.onEntityDestroyed(entity);
                        continue;
                    }

                    if(entity.check(SealEngineFlags.NEW)) system.onNewEntity(entity);
                    system.onUpdate(entity);

                }
            }

            // Clean the new flag of the object, if it has one
            entity.deactivate(SealEngineFlags.NEW);
            stream.write(entity);
        }

        stream.write(SealCamera.current);
        stream.invalidate();
    }

    public static byte[] getEngineCalls(){
        return instance.engineCalls.compile();
    }
}
