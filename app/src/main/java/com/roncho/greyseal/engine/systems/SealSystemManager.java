package com.roncho.greyseal.engine.systems;

import com.roncho.greyseal.engine.systems.instructions.SealCallsList;
import com.roncho.greyseal.engine.systems.stream.SealEntity;
import com.roncho.greyseal.engine.systems.stream.SealObjectStream;

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
    }

    public static void addSystem(SealSystem system){
        instance.systems.add(system);
    }

    public static void runSystems(SealObjectStream stream){
        for (SealSystem system : instance.systems) system.updateOnce();

        while(stream.hasNext()) {
            SealEntity entity = stream.next();

            for (SealSystem system : instance.systems) {
                // Do a system select here
                if(system.selectObject(entity)){
                    system.onUpdate(entity);
                }
            }

            stream.write(entity);
        }
    }
    public static byte[] getEngineCalls(){
        return instance.engineCalls.compile();
    }
}
