package com.roncho.greyseal.engine;

import com.roncho.greyseal.engine.android.cpp.SealCppHandler;
import com.roncho.greyseal.engine.systems.SealSystemManager;
import com.roncho.greyseal.engine.systems.instructions.SealCallType;
import com.roncho.greyseal.engine.systems.stream.SealEntity;

import java.nio.ByteBuffer;

public class SealEngine {

    public static void destroy(SealEntity e){
        short index = e.localIndex();
        ByteBuffer bb = SealCppHandler.allocateJava(2).putShort(index);
        SealSystemManager.queue(SealCallType.DESTROY, bb);
    }

    public static void loadTexture(String path){
        ByteBuffer bb = SealCppHandler.allocateJava(path.length() + 1);
        SealCppHandler.putString(bb, path);
        SealSystemManager.queue(SealCallType.LOAD_TEXTURE, bb);
    }

    public static void loadMesh(String path){
        ByteBuffer bb = SealCppHandler.allocateJava(path.length() + 1);
        SealCppHandler.putString(bb, path);
        SealSystemManager.queue(SealCallType.LOAD_MESH, bb);
    }

    public static void loadMaterial(String vertex, String fragment){
        ByteBuffer bb = SealCppHandler.allocateJava(vertex.length() + 2 + fragment.length());
        SealCppHandler.putString(bb, vertex);
        SealCppHandler.putString(bb, fragment);
        SealSystemManager.queue(SealCallType.LOAD_MATERIAL, bb);
    }
}
