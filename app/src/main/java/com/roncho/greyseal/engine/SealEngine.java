package com.roncho.greyseal.engine;

import com.roncho.greyseal.engine.android.SealTexturePipeline;
import com.roncho.greyseal.engine.android.cpp.SealCppHandler;
import com.roncho.greyseal.engine.systems.SealSystemManager;
import com.roncho.greyseal.engine.systems.instructions.SealCallType;
import com.roncho.greyseal.engine.systems.stream.SealEngineFlags;
import com.roncho.greyseal.engine.systems.stream.Entity;

import java.nio.ByteBuffer;

public class SealEngine {

    public static void destroy(Entity e){
          e.activate(SealEngineFlags.DESTROY);
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

    public static void loadFont(String path){
        SealTexturePipeline.loadFont(path, 24);
    }

    public static void loadMaterial(String vertex, String fragment){
        ByteBuffer bb = SealCppHandler.allocateJava(vertex.length() + 2 + fragment.length());
        SealCppHandler.putString(bb, vertex);
        SealCppHandler.putString(bb, fragment);
        SealSystemManager.queue(SealCallType.LOAD_MATERIAL, bb);
    }

    public static void makePreset(String path){
        ByteBuffer bb = SealCppHandler.allocateJava(path.length() + 1);
        SealCppHandler.putString(bb, path);
        SealSystemManager.queue(SealCallType.INSTANTIATE_2, bb);
    }

    public static void makePreset(String path, Vector3 position){
        ByteBuffer bb = SealCppHandler.allocateJava(path.length() + 1 + Vector3.SIZE_IN_BYTES);
        SealCppHandler.putString(bb, path);
        position.writeToBuffer(bb);
        SealSystemManager.queue(SealCallType.INSTANTIATE_1, bb);
    }

    public static void makePreset(String path, Vector3 pos, Quaternion qt, Vector3 scl){
        ByteBuffer bb = SealCppHandler.allocateJava(path.length() + 1 + Vector3.SIZE_IN_BYTES * 2 + Quaternion.SIZE_IN_BYTE);
        SealCppHandler.putString(bb, path);
        pos.writeToBuffer(bb);
        qt.writeToBuffer(bb);
        scl.writeToBuffer(bb);
        SealSystemManager.queue(SealCallType.INSTANTIATE_1, bb);
    }
}
