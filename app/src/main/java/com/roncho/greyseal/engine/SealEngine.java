package com.roncho.greyseal.engine;

import com.roncho.greyseal.engine.android.SealTexturePipeline;
import com.roncho.greyseal.engine.android.cpp.SealCppHandler;
import com.roncho.greyseal.engine.api.HasStringValue;
import com.roncho.greyseal.engine.systems.SealSystem;
import com.roncho.greyseal.engine.systems.SealSystemManager;
import com.roncho.greyseal.engine.systems.instructions.SealCallType;
import com.roncho.greyseal.engine.systems.stream.SealEngineFlags;
import com.roncho.greyseal.engine.systems.stream.Entity;

import java.nio.ByteBuffer;
import java.util.HashMap;

public class SealEngine {

    private final static HashMap<String, Integer> m_LoadedFonts = new HashMap<>();

    public static void destroy(Entity e){
        // If the entity is not already destroyed, destroy it
        if(!e.check(SealEngineFlags.DESTROYED) && !e.check(SealEngineFlags.DESTROY)) {
            e.activate(SealEngineFlags.DESTROY);
            SealSystem.modify(e);
        }
    }

    public static void loadTexture(String path){
        ByteBuffer bb = SealCppHandler.allocateJava(path.length() + 1);
        SealCppHandler.putString(bb, path);
        SealSystemManager.queue(SealCallType.LOAD_TEXTURE, bb);
    }
    public static void loadTexture(HasStringValue path) { loadTexture(path.getStringValue());}

    public static void loadMesh(String path){
        ByteBuffer bb = SealCppHandler.allocateJava(path.length() + 1);
        SealCppHandler.putString(bb, path);
        SealSystemManager.queue(SealCallType.LOAD_MESH, bb);
    }
    public static void loadMesh(HasStringValue path) { loadMesh(path.getStringValue());}

    public static int loadFont(String path){
        if(!m_LoadedFonts.containsKey(path)) {
            int index = SealTexturePipeline.loadFont(path, 48);
            m_LoadedFonts.put(path, index);
            return index;
        }
        return m_LoadedFonts.get(path);
    }
    public static int loadFont(HasStringValue path) { return loadFont(path.getStringValue()); }

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
        SealSystemManager.queue(SealCallType.INSTANTIATE_0, bb);
    }

    public static void cloneInstance(Entity e, Vector3 pos){
        ByteBuffer bb = SealCppHandler.allocateJava(2 + Vector3.SIZE_IN_BYTES);
        bb.putShort(e.uid);
        pos.writeToBuffer(bb);
        SealSystemManager.queue(SealCallType.CLONE_3, bb);
    }
    public static void cloneInstance(Entity e){
        ByteBuffer bb = SealCppHandler.allocateJava(2);
        bb.putShort(e.uid);
        SealSystemManager.queue(SealCallType.CLONE_1, bb);
    }
    public static void cloneInstance(Entity e, Vector3 pos, Quaternion rot, Vector3 scl){
        ByteBuffer bb = SealCppHandler.allocateJava(2 + Quaternion.SIZE_IN_BYTE + 2 * Vector3.SIZE_IN_BYTES);
        bb.putShort(e.uid);
        pos.writeToBuffer(bb);
        rot.writeToBuffer(bb);
        scl.writeToBuffer(bb);
        SealSystemManager.queue(SealCallType.CLONE_2, bb);
    }
}
