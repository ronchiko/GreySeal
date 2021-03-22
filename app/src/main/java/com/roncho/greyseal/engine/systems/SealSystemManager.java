package com.roncho.greyseal.engine.systems;

import com.roncho.greyseal.engine.SealCamera;
import com.roncho.greyseal.engine.SealLog;
import com.roncho.greyseal.engine.systems.instructions.SealCallType;
import com.roncho.greyseal.engine.systems.instructions.SealCallsList;
import com.roncho.greyseal.engine.systems.stream.SealEngineFlags;
import com.roncho.greyseal.engine.systems.stream.SealEntity;
import com.roncho.greyseal.engine.systems.stream.SealObjectStream;

import java.nio.ByteBuffer;
import java.util.ArrayList;

public class SealSystemManager {

    private SealCallsList engineCalls;
    private final ArrayList<SealSystem> systems;
    private static SealSystemManager instance;

    public static void newManager(){
        instance = new SealSystemManager();
    }

    public SealSystemManager(){
        systems = new ArrayList<>();
        engineCalls = new SealCallsList();
    }

    public static void addSystem(SealSystem system){
        instance.systems.add(system);
    }
    public static void queue(SealCallType type, ByteBuffer bb){
        instance.engineCalls.queueCall(type, bb);
    }
    public static void runSystems(SealObjectStream stream){
        instance.engineCalls.reset();
        SealCamera.current = stream.next();
        for (SealSystem system : instance.systems) system.updateOnce();

        while(stream.hasNext()) {
            SealEntity entity = stream.next();

            for (SealSystem system : instance.systems) {
                // Do a system select here
                if(system.selectObject(entity)){
                    if(entity.check(SealEngineFlags.NEW)) system.onNewEntity(entity);
                    system.onUpdate(entity);
                    if(entity.check(SealEngineFlags.DESTROY)) system.onEntityDestroyed(entity);
                }
            }

            // Clean the new flag of the object, if it has one
            entity.deactivate(SealEngineFlags.NEW);
            stream.write(entity);
        }
        stream.write(SealCamera.current);
    }
    public static byte[] getEngineCalls(){
        return instance.engineCalls.compile();
    }
}
