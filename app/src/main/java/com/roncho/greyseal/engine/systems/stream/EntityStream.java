package com.roncho.greyseal.engine.systems.stream;

import com.roncho.greyseal.engine.SealLog;
import com.roncho.greyseal.engine.android.cpp.SealCppHandler;
import com.roncho.greyseal.engine.api.EntityQuery;
import com.roncho.greyseal.engine.systems.SealSystem;
import com.roncho.greyseal.game.activity.EnemySystem;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

public final class EntityStream {

    static long g_CurrentStreamId = 0;

    // Cache for already generate objects
    private final HashMap<Short, Entity> generated = new HashMap<>();

    // The size (in bytes) of a single object
    final static int SIZEOF_OBJECT = getSizeofNativeObject();

    private int current;
    private final int count;
    private final byte[] stream;
    private boolean valid;
    private final long m_StreamId;

    public EntityStream(byte[] stream) {
        current = 0;
        count = stream.length / SIZEOF_OBJECT;
        valid = true;
        this.stream = stream;

        m_StreamId = ++g_CurrentStreamId;
    }

    public boolean hasNext(){
        return current < count;
    }

    public Entity next(){
        int offset = (current++) * SIZEOF_OBJECT;
        short uid = SealCppHandler.readShort(stream, offset + 6);
        if(generated.containsKey(uid)){
            return generated.get(uid);
        }
        Entity e = new Entity(stream, offset);
        generated.put(uid, e);
        return e;
    }
    public Entity get(int i){
        if(i < 0 || i >= count) return null;

        short uid = SealCppHandler.readShort(stream, i * SIZEOF_OBJECT + 6);
        if(generated.containsKey(uid)) return generated.get(uid);

        Entity e = new Entity(stream, i * SIZEOF_OBJECT);
        generated.put(uid, e);
        return e;
    }

    public byte[] getRaw() { return stream; }

    public void write(Entity e){
        // If the stream is invalid or the entity that whats to be written to the stream
        // was not generated on this stream, don't allow to write it
        if(!valid || e.m_GeneratedOnStreamId != m_StreamId) return;
        e.write(stream);
    }

    public Entity getByUid(short uid){
        if(generated.containsKey(uid)){
            return generated.get(uid);
        }

        for(int i = 1; i < count; i++){
            short _uid = SealCppHandler.readShort(stream, i * SIZEOF_OBJECT + 6);
            if(_uid == uid) {
                Entity result =  new Entity(stream, i * SIZEOF_OBJECT);
                generated.put(_uid, result);
                return result;
            }
        }
        return null;
    }
    public Entity search(EntityQuery query){
        for(int i = 1; i < count; i++){
            Entity e = get(i);
            if(query.query(e)) return e;
        }
        return null;
    }

    public Entity[] searchAll(EntityQuery query){
        // Search entity list for entities matching the query
        List<Entity> entities = new ArrayList<>();
        for(int i = 1; i < count; i++){
            Entity e = get(i);
            if(query.query(e)) entities.add(e);
        }
        // Copy the list to an array
        Entity[] finals = new Entity[entities.size()];
        int i = 0;
        for(Entity e : entities) finals[i++] = e;
        return finals;
    }
    public void invalidate() { valid = false; }
    public boolean isInvalid() { return !valid; }
    private static native int getSizeofNativeObject();


}
